set(GUI_TARGET_COMPONENTS_NAME camera-background-remover-gui-components)
add_library(${GUI_TARGET_COMPONENTS_NAME})

ecm_add_qml_module(${GUI_TARGET_COMPONENTS_NAME}
    URI "org.kde.cbr.components"
    GENERATE_PLUGIN_SOURCE
)

ecm_target_qml_sources(${GUI_TARGET_COMPONENTS_NAME}
    SOURCES
    CamerasDrawer.qml
    AddVirtualCameraDialog.qml
    BackgroundImageGallery.qml
    VideoSourcesView.qml
    GalleryImageCard.qml
    CameraPage.qml
)

ecm_finalize_qml_module(${GUI_TARGET_COMPONENTS_NAME})

install(TARGETS ${GUI_TARGET_COMPONENTS_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
