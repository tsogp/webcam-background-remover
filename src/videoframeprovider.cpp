#include "videoframeprovider.h"
#include <qstringliteral.h>

VideoFrameProvider::VideoFrameProvider(const std::string &model,
                                       const std::string &backgroundPath,
                                       ImageProvider *provider,
                                       QObject *parent)
    : QObject(parent), cap(0), imgProvider(provider) {
    // Load ONNX model
    net = cv::dnn::readNetFromONNX(model);
    net.setPreferableBackend(0);
    net.setPreferableTarget(1);

    // Load background image
    background = cv::imread(backgroundPath, cv::IMREAD_COLOR);
    if (background.empty()) {
        throw std::runtime_error("Failed to load background image");
    }

    // Prepare background size
    cv::resize(background, background, cv::Size(320, 320));
    background.convertTo(background, CV_32FC3);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &VideoFrameProvider::processFrame);
}

void VideoFrameProvider::start() {
    if (!timer->isActive()) {
        timer->start(30); // 30 ms ≈ 33 FPS
    }

    if (!cap.isOpened()) {
        cap.open(0);
    }
}

void VideoFrameProvider::stop() {
    if (timer->isActive()) {
        timer->stop();
    }

    if (cap.isOpened()) {
        cap.release();
    }
}


void VideoFrameProvider::setBackgroundImage(const QString &path) {
    QString localPath = path;

    if (path.startsWith(QStringLiteral("file://"))) {
        QUrl url(path);
        localPath = url.toLocalFile();
    }

    background = cv::imread(localPath.toStdString(), cv::IMREAD_COLOR);
    if (background.empty()) {
        qWarning() << "Failed to load background image:" << path;
        return;
    }
    cv::resize(background, background, cv::Size(320, 320));
    background.convertTo(background, CV_32FC3);
}

void VideoFrameProvider::processFrame() {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        return;
    }

    cv::Mat before = frame.clone(); // Original frame for preview
    cv::resize(frame, frame, cv::Size(320, 320));
    frame.convertTo(frame, CV_32FC3);

    cv::Mat orig_frame = frame.clone();

    cv::subtract(frame, cv::Scalar(102.89, 111.25, 126.91), frame);
    cv::multiply(frame, cv::Scalar(1.0 / 62.93, 1.0 / 62.82, 1.0 / 66.36), frame);

    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0, cv::Size(320, 320));
    net.setInput(blob);
    cv::Mat score = net.forward();
    cv::Mat bgval(score.size[2], score.size[3], CV_32F, score.data);

    cv::Mat mask;
    cv::threshold(bgval, mask, 0.3, 255, cv::THRESH_BINARY_INV);
    mask.convertTo(mask, CV_8U);

    bool enableThreshold   = true;
    double contourFilter   = 0.005; // relative size threshold (0.0 - 1.0)
    double smoothContour   = 0.3;   // 0.0 - 1.0
    double feather         = 0.2;   // 0.0 - 1.0

    if (enableThreshold) {
        // Filter small contours
        if (contourFilter > 0.0 && contourFilter < 1.0) {
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask, contours,
                             cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            std::vector<std::vector<cv::Point>> filteredContours;
            double contourSizeThreshold =
                (double)mask.total() * contourFilter;

            for (auto &contour : contours) {
                if (cv::contourArea(contour) > contourSizeThreshold) {
                    filteredContours.push_back(contour);
                }
            }

            mask.setTo(0);
            cv::drawContours(mask, filteredContours, -1,
                             cv::Scalar(255), -1);
        }

        // Smooth contour edges
        if (smoothContour > 0.0) {
            int k_size = (int)(3 + 11 * smoothContour);
            if (k_size % 2 == 0) k_size++;
            cv::stackBlur(mask, mask,
                          cv::Size(k_size, k_size));
            mask = mask > 128;
        }

        // Feathering (blur edges for soft transition)
        if (feather > 0.0) {
            int k_size = (int)(5 * feather);
            if (k_size % 2 == 0) k_size++;
            cv::dilate(mask, mask, cv::Mat(),
                       cv::Point(-1, -1), k_size / 3);
            cv::boxFilter(mask, mask, -1,
                          cv::Size(k_size, k_size));
        }
    }

    cv::resize(mask, mask, orig_frame.size());

    // --- Apply mask ---
    orig_frame.convertTo(orig_frame, CV_8UC3);
    background.convertTo(background, CV_8UC3);
    cv::resize(background, background, orig_frame.size());

    cv::Mat fg, bg, result;
    orig_frame.copyTo(fg, mask);
    cv::Mat invMask;
    cv::bitwise_not(mask, invMask);
    background.copyTo(bg, invMask);
    cv::add(fg, bg, result);

    // Resize it back
    cv::resize(result, result, before.size());

    QImage qBefore(before.data, before.cols, before.rows, before.step, QImage::Format_BGR888);
    QImage qAfter(result.data, result.cols, result.rows, result.step, QImage::Format_BGR888);

    imgProvider->updateImage(QStringLiteral("before"), qBefore.copy());
    imgProvider->updateImage(QStringLiteral("after"), qAfter.copy());

    static qint64 frameCounter = 0;
    frameCounter++;

    // The frame counter is to view trigger refresh on the view
    QString beforeUrl = QString(QStringLiteral("image://frames/before?%1")).arg(frameCounter);
    QString afterUrl  = QString(QStringLiteral("image://frames/after?%1")).arg(frameCounter);

    Q_EMIT frameReady(beforeUrl, afterUrl);
}