cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME camera-background-remover)
set(CMAKE_CXX_STANDARD 23)
project(${PROJECT_NAME})

add_definitions(-fexceptions)

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Test
    Gui
    QuickControls2
    Widgets
)

find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    I18n
    CoreAddons
    QQC2DesktopStyle
    IconThemes
)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

add_subdirectory(src)

install(PROGRAMS org.kde.cbr.desktop DESTINATION ${KDE_INSTALL_APPDIR})

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR}/bin)

install(FILES assets/background.jpg DESTINATION ${KDE_INSTALL_APPDIR}/bin/assets)
install(FILES models/SINet_Softmax.onnx DESTINATION ${KDE_INSTALL_APPDIR}/bin/models)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
