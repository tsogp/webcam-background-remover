#include "virtualcameramanager.h"
#include <filesystem>
#include <system_error>

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
            if (is_capture && strcmp(reinterpret_cast<char *>(caps.driver), "v4l2 loopback") != 0) {
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


virtual_camera_data::virtual_camera_data(const std::string &devicePath, const std::string &label, int width, int height, int fps)
    : dev_path(devicePath), label(label), width(width), height(height), fps(fps) {
    open_device();
    configure_device();
}

virtual_camera_data::~virtual_camera_data() {
    if (fd >= 0) {
        close(fd);
    }
}

// Send OpenCV Mat frame (BGR24 → YUYV conversion)
void virtual_camera_data::sendFrame(const cv::Mat &frame) const {
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
    if (fs::exists(dev_path)) {
        throw std::logic_error("Device path already exists: " + dev_path);
    }

    std::string cmd = std::format("pkexec v4l2loopback-ctl add -n \"{}\" {}", label, dev_path);
    int ret = std::system(cmd.c_str());

    if (ret != 0) {
        throw std::runtime_error("Failed to create v4l2loopback device with modprobe");
    }
}

void virtual_camera_data::destroy() {
    std::string cmd = std::format("pkexec v4l2loopback-ctl remove {}", dev_path);
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        throw std::runtime_error("Failed to remove v4l2loopback device with modprobe");
    }
}

void virtual_camera_data::open_device() {
    fd = open(dev_path.c_str(), O_RDWR);
    if (fd < 0) {
        throw std::runtime_error(
            "Failed to open device: " + dev_path +
            " (errno " + std::to_string(errno) + ": " + std::strerror(errno) + ")"
        );
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