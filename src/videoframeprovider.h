#ifndef __VIDEO_FRAME_PROVIDER_H__
#define __VIDEO_FRAME_PROVIDER_H__

#include "frameprocessor.h"
#include <QImage>
#include <QMutex>
#include <QTimer>
#include <QQuickImageProvider>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

class VideoFrameProvider : public QObject {
    Q_OBJECT

public:
    VideoFrameProvider(const std::string &model, const std::string &backgroundPath, ImageProvider *provider, QObject *parent = nullptr);

    Q_INVOKABLE void start();
Q_SIGNALS:
    void frameReady(const QString &beforeKey, const QString &afterKey);

private Q_SLOTS:
    void processFrame();

private:
    QTimer *timer;
    cv::VideoCapture cap;
    cv::dnn::Net net;
    cv::Mat background;
    ImageProvider *imgProvider;
};

#endif // __VIDEO_FRAME_PROVIDER_H__