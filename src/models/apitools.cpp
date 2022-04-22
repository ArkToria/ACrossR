#include "apitools.h"
#include <iostream>

using namespace across;
using namespace across::core;
using namespace acolors;

APITools::APITools(const std::shared_ptr<Channel> &channel) {
    p_channel = channel;

    // create thread
    p_thread = new QThread(this);
    p_worker = new APIWorker(p_channel);
    p_worker->moveToThread(p_thread);

    // control signals
    connect(this, &APITools::operate, p_worker, &APIWorker::start);
    connect(p_worker, &APIWorker::trafficChanged, this,
            &APITools::handleTrafficResult);
}

APITools::~APITools() {
    if (p_worker) {
        emit p_worker->aboutToQuit();
        p_worker->deleteLater();
    }
    if (p_thread) {
        p_thread->quit();
        p_thread->wait();
    }
}

void APITools::startMonitoring() {
    // qDebug() << "StartMonitoring";
    p_thread->start();
    emit operate();
}

void APITools::stopMonitoring() { emit p_worker->aboutToQuit(); }

void APITools::restartMonitoring() { startMonitoring(); }

void APITools::setChannel(const std::shared_ptr<Channel> &channel) {
    p_channel = channel;
    p_worker->setChannel(channel);
}

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
    connect(this, &APIWorker::aboutToQuit, this, &APIWorker::stop);
}

void APIWorker::start() {

    // qDebug() << "StartLoop";
    this->m_stop = false;
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
        // qDebug() << "Traffic:" << traffic_info.upload <<
        // traffic_info.download;

        emit trafficChanged(QVariant::fromValue<TrafficInfo>(traffic_info));

        if (!status.ok())
            qDebug() << status.error_message().c_str();

        if (traffic_info.upload < 0 || traffic_info.download < 0)
            break;
        else {
            QTimer timer;
            QEventLoop loop;
            connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
            connect(this, &APIWorker::aboutToQuit, &loop, &QEventLoop::quit);
            timer.start(1000);
            loop.exec();
        }
    }
}

void APIWorker::stop() { this->m_stop = true; }

void APIWorker::setChannel(const std::shared_ptr<Channel> &channel) {
    qDebug() << "setchannel";
    this->p_stub = acolors::CoreManager::NewStub(channel);
    emit channelChanged();
}

void across::core::TrafficInfo::clear() { download = upload = 0; }
