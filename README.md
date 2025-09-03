# Real-Time Video Background Remover

![demo](https://github.com/tsogp/webcam-background-remover/blob/main/assets/video.gif)

A **real-time video background removal application** built with **Qt6** and **OpenCV**. This app captures video from your webcam, applies a deep learning-based background segmentation model (ONNX), and replaces the background with a custom image or video in real time. **In future** the processed video can be streamed to virtual camera devices using **v4l2loopback**.

---

## Features

- Real-time background removal using ONNX deep learning models.
- Supports custom background images.
- Preview of original and processed frames.

---

## Dependencies

To build and run this project, you need:

- **Qt6** (Base, Declarative, Kirigami)
- **CMake**
- **OpenCV** (with DNN module)
- **v4l2loopback** (Linux kernel module to create virtual cameras)
- KDE Frameworks modules: `ki18n`, `kcoreaddons`, `breeze`, `kiconthemes`
- QQC2 Desktop Style for Qt Quick Controls 2

On **Arch Linux / Manjaro**, you can install the dependencies with:

```bash
sudo pacman -S base-devel extra-cmake-modules cmake kirigami ki18n kcoreaddons breeze kiconthemes qt6-base qt6-declarative qqc2-desktop-style
sudo pacman -S v4l2loopback-dkms
```

## Build

Clone the repository:

```bash
git clone https://github.com/tsogp/webcam-background-remover
cd camera-background-remover
```

Bulid with CMake:
```bash
cmake -B build -S .
cmake --build build --parallel --config Release
sudo cmake --install build --config Release
```

```bash
./camera-background-remover
```

## Usage

Start the application. Select your webcam as the input device. Optionally, set a custom background image. Start the virtual camera output. The original and processed frames are displayed in the UI.
