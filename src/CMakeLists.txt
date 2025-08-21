set(GUI_TARGET_NAME camera-background-remover-gui)
add_executable(${GUI_TARGET_NAME})

ecm_add_qml_module(${GUI_TARGET_NAME}
    URI
    org.kde.cbr # cbr = camera-background-remover
)

target_sources(${GUI_TARGET_NAME}
    PRIVATE
    main.cpp
    virtualcameramodel.cpp
    virtualcameramodel.h
)

ecm_target_qml_sources(${GUI_TARGET_NAME}
    SOURCES
    Main.qml
)

target_link_libraries(${GUI_TARGET_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
    camera-background-remover-gui-components
)

install(TARGETS ${GUI_TARGET_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(components)