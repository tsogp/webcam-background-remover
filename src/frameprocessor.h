#ifndef __FRAME_PROCESSOR_H__
#define __FRAME_PROCESSOR_H__

// ImageProvider.h
#include <QStringLiteral>
#include <QImage>
#include <QMutex>
#include <QString>
#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider {
public:
    ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {
    }

    void updateImage(const QString &id, const QImage &image) {
        QMutexLocker locker(&mutex);
        images[id] = image;
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        QMutexLocker locker(&mutex);

        QString cleanId = id;
        int queryIndex = cleanId.indexOf(QStringLiteral("?"));
        if (queryIndex != -1) {
            cleanId = cleanId.left(queryIndex);
        }

        if (images.contains(cleanId)) {
            QImage img = images[cleanId];
            if (size) {
                *size = img.size();
            }
            if (requestedSize.isValid()) {
                return img.scaled(requestedSize, Qt::KeepAspectRatio);
            }
            return img;
        }
        return QImage();
    }

private:
    QMutex mutex;
    QHash<QString, QImage> images;
};


#endif // __FRAME_PROCESSOR_H__