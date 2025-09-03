#ifndef __VIDEO_FRAME_PROVIDER_H__
#define __VIDEO_FRAME_PROVIDER_H__

#include "frameprocessor.h"
#include "virtualcameramanager.h"
#include <QImage>
#include <QMutex>
#include <QTimer>
#include <QQuickImageProvider>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <optional>
#include <qtmetamacros.h>
#include <thread>

// TODO:
// Currently the class works as a singleton, which is not correct
// Needs to be refactored the way that each virtual camera owns an instance of it
class VideoFrameProvider : public QObject {
    Q_OBJECT

public:
    VideoFrameProvider(const std::string &model, const std::string &backgroundPath, ImageProvider *provider, QObject *parent = nullptr);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setBackgroundImage(const QString &path);
    Q_INVOKABLE void setOutputDevice(const QString &path);
    Q_INVOKABLE void setOutputDevice(const vc::virtual_camera_data &virtualCam);
    Q_INVOKABLE void clearOutputDevice();
Q_SIGNALS:
    void frameReady(const QString &beforeKey, const QString &afterKey);

private Q_SLOTS:
    void processFrame();

private:
    QString currentPath;
    QTimer *timer;
    cv::VideoCapture cap;
    cv::dnn::Net net;
    cv::Mat background;
    ImageProvider *imgProvider;
    std::optional<vc::virtual_camera_data> virtualCamData;
};

#endif // __VIDEO_FRAME_PROVIDER_H__