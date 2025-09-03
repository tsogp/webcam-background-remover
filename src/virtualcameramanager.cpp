#include "virtualcameramanager.h"
#include <filesystem>
#include <opencv2/highgui.hpp>
#include <qdebug.h>
#include <string>

namespace fs = std::filesystem;

namespace vc {
QStringList CameraManager::listPhysicalCaptureDevices() {
    QStringList result;

    for (int i = 0; i < 64; i++) {
        std::string dev = "/dev/video" + std::to_string(i);
        int fd = open(dev.c_str(), O_RDONLY);
        if (fd < 0) {
            continue;
        }

        v4l2_capability caps{};
        if (ioctl(fd, VIDIOC_QUERYCAP, &caps) == 0) {
            bool is_capture = caps.capabilities & V4L2_CAP_VIDEO_CAPTURE;
            if (is_capture) {
                result << QString::fromStdString(dev);
            }
        }
        close(fd);
    }
    return result;
}

QStringList CameraManager::listVirtualCaptureDevices() {
    QStringList result;

    for (int i = 0; i < 64; i++) {
        std::string dev = "/dev/video" + std::to_string(i);
        int fd = open(dev.c_str(), O_RDONLY);
        if (fd < 0) {
            continue;
        }

        v4l2_capability caps{};
        if (ioctl(fd, VIDIOC_QUERYCAP, &caps) == 0) {
            if (strcmp(reinterpret_cast<char *>(caps.driver), "v4l2 loopback") == 0) {
                result << QString::fromStdString(dev + " (" + reinterpret_cast<char *>(caps.card) + ")");
            }
        }
        close(fd);
    }
    return result;
}


virtual_camera_data::virtual_camera_data(const std::string &devicePath,
                                         const std::string &label,
                                         int width,
                                         int height,
                                         int fps)
    : dev_path(devicePath), label(label), width(width), height(height), fps(fps) {
    create();
    open_device();
    configure_device();
}

virtual_camera_data::~virtual_camera_data() {
    if (fd >= 0) {
        close(fd);
    }
}

std::string fd_to_path(int fd) {
    std::ostringstream link;
    link << "/proc/" << getpid() << "/fd/" << fd;

    char buf[PATH_MAX];
    ssize_t len = readlink(link.str().c_str(), buf, sizeof(buf) - 1);
    if (len == -1) {
        throw std::runtime_error("Failed to resolve fd path");
    }
    buf[len] = '\0';
    return std::string(buf);
}

// TODO: fix this horror (virtual camera is not created)
void virtual_camera_data::sendFrame(const cv::Mat& frame) const {
    if (frame.cols != width || frame.rows != height) {
        throw std::runtime_error("Frame size mismatch");
    }
    cv::Mat yuyv;
    cv::cvtColor(frame, yuyv, cv::COLOR_BGR2YUV_YUY2);

    ssize_t written = write(fd, yuyv.data, yuyv.total() * yuyv.elemSize());
    if (written < 0) {
        throw std::runtime_error("Failed to write frame");
    }
}

void virtual_camera_data::create() {
    // Check if device exists
    if (access(dev_path.c_str(), F_OK) == 0) {
        std::cout << dev_path << " already exists, skipping creation.\n";
        return;
    }

    std::string cmd = "pkexec modprobe -r v4l2loopback";
    std::system(cmd.c_str());

    // get the number from /dev/video10
    int dev_num = std::stoi(dev_path.substr(10));

    cmd =
        "pkexec modprobe v4l2loopback devices=1 video_nr=" + std::to_string(dev_num) + " card_label=\"" + label + "\"";
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        throw std::runtime_error("Failed to create v4l2loopback device with modprobe");
    }

    usleep(200000);
    if (access(dev_path.c_str(), F_OK) != 0) {
        throw std::runtime_error("Device still not found after modprobe: " + dev_path);
    }
}

void virtual_camera_data::destroy() {
    std::string cmd = std::format("pkexec v4l2loopback-ctl delete {}", dev_path);
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        throw std::runtime_error("Failed to remove v4l2loopback device with modprobe");
    }
}

void virtual_camera_data::open_device() {
    qDebug() << dev_path;
    fd = open(dev_path.c_str(), O_WRONLY);
    qDebug() << "FD " << fd << " points to " << fd_to_path(fd);
    if (fd < 0) {
        throw std::runtime_error("Failed to open device: " + dev_path + " (errno " + std::to_string(errno) + ": " +
                                 std::strerror(errno) + ")");
    }
}

void virtual_camera_data::configure_device() const {
    v4l2_format fmt{};
    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
        throw std::runtime_error("Failed to set device format");
    }

    v4l2_streamparm parm{};
    parm.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    parm.parm.output.timeperframe.numerator = 1;
    parm.parm.output.timeperframe.denominator = fps;
    if (ioctl(fd, VIDIOC_S_PARM, &parm) < 0) {
        throw std::runtime_error("Failed to set stream parameters");
    }
}

} // namespace vc