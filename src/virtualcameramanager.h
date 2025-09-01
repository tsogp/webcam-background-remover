#ifndef __VIRTUAL_CAMERA_MANAGER_H__
#define __VIRTUAL_CAMERA_MANAGER_H__

#include <QObject>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

namespace vc {
class CameraManager : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE QStringList listPhysicalCaptureDevices();
    Q_INVOKABLE QStringList listVirtualCaptureDevices();
};

class virtual_camera {
public:
    virtual_camera(const std::string &devicePath, int width, int height, int fps);
    ~virtual_camera();

    void sendFrame(const cv::Mat &frame) const;
    void create(const std::string &label);
    void destroy();

private:
    std::string dev_path;
    int fd{-1};
    int width;
    int height;
    int fps;

    void open_device();
    void configure_device() const;
};
} // namespace vc


#endif // __VIRTUAL_CAMERA_MANAGER_H__