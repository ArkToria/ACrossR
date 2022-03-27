#include "apitools.h"
#include <iostream>

using namespace across;
using namespace across::core;
using namespace acolors;

APITools::APITools(const std::shared_ptr<Channel> &channel) {
    p_channel = channel;

    // create thread
    p_worker = new APIWorker(p_channel);

    // control signals
    connect(this, &APITools::operate, p_worker, &APIWorker::start);
    connect(p_worker, &APIWorker::trafficChanged, this,
            &APITools::handleTrafficResult);
}

APITools::~APITools() {

    if (p_worker) {
        p_worker->stop();
        p_worker->deleteLater();
    }
}

void APITools::startMonitoring() { emit operate(); }

void APITools::stopMonitoring() { p_worker->stop(); }

void APITools::restartMonitoring() { startMonitoring(); }

std::pair<bool, std::string> APITools::isOk() const {
    auto p_stub = acolors::CoreManager::NewStub(p_channel);

    ClientContext context;
    GetTrafficInfoRequest request;
    CompletionQueue cq;
    Status status;
    acolors::TrafficInfo response;

    // check response stats
    std::unique_ptr<ClientAsyncResponseReader<acolors::TrafficInfo>> rpc(
        p_stub->AsyncGetTrafficInfo(&context, request, &cq));

    rpc->Finish(&response, &status, (void *)1);

    void *got_tag;
    bool ok = cq.Next(&got_tag, &ok);

    // GPR_ASSERT(ok);
    // GPR_ASSERT(got_tag == (void *)1);
    // GPR_ASSERT(ok);

    if (status.ok()) {
        return {true, ""};
    } else {
        std::string err =
            std::to_string(status.error_code()) + ": " + status.error_message();
        return {false, err};
    }
}

void APITools::handleTrafficResult(const QVariant &data) {
    emit trafficChanged(data);
}

APIWorker::APIWorker(const std::shared_ptr<grpc::Channel> &channel) {
    p_stub = acolors::CoreManager::NewStub(channel);
}

void APIWorker::start() {
    stop();
    this->m_stop = false;
    this->future = QtConcurrent::run([&] {
        while (!m_stop) {
            GetTrafficInfoRequest request;
            ClientContext context;
            acolors::TrafficInfo info;
            Status status;
            status = p_stub->GetTrafficInfo(&context, request, &info);

            auto traffic_info = TrafficInfo{
                .upload = info.upload(),
                .download = info.download(),
            };

            emit trafficChanged(QVariant::fromValue<TrafficInfo>(traffic_info));

            if (!status.ok())
                qDebug() << status.error_message().c_str();

            if (traffic_info.upload < 0 || traffic_info.download < 0)
                break;
            else {
                QTimer timer;
                QEventLoop loop;
                connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
                timer.start(1000);
                loop.exec();
            }
        }
    });
}

void APIWorker::stop() {
    this->m_stop = true;
    this->future.waitForFinished();
}

void across::core::TrafficInfo::clear() { download = upload = 0; }
