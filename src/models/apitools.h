#ifndef APITOOLS_H
#define APITOOLS_H

#include <QEventLoop>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QVariant>
#include <QtConcurrent>

#include <atomic>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <vector>

#include "acolors.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

namespace across::core {
struct TrafficInfo {
    qint64 upload = 0;
    qint64 download = 0;

    void clear();
};

class APIWorker : public QObject {
    Q_OBJECT
  public:
    explicit APIWorker(const std::shared_ptr<grpc::Channel> &channel);

  public slots:
    void start();

    void stop();

  signals:
    void trafficChanged(const QVariant &data);

  public:
    bool m_stop = false;

  private:
    std::unique_ptr<acolors::CoreManager::Stub> p_stub;

    QFuture<void> future;
};

class APITools : public QObject {
    Q_OBJECT
  public:
    explicit APITools(const std::shared_ptr<Channel> &channel);

    ~APITools() override;

    void startMonitoring();

    void stopMonitoring();

    void restartMonitoring();

    [[nodiscard]] std::pair<bool, std::string> isOk() const;

    static inline QString unitConvert(double bytes) {
        if (bytes == 0.0)
            return {"0 B"};

        const QString sizes[] = {
            "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB",
        };

        int index = int(floor(log(bytes) / log(1024)));

        return QString("%1 %2")
            .arg(bytes / pow(1024, index), 0, 'f', 2)
            .arg(sizes[index]);
    };

  public slots:
    void handleTrafficResult(const QVariant &data);

  signals:
    void operate();

    void trafficChanged(const QVariant &data);

  private:
    std::shared_ptr<Channel> p_channel;
    APIWorker *p_worker = nullptr;
};
} // namespace across::core

#endif // APITOOLS_H
