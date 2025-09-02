import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import org.kde.cbr.components

Kirigami.ApplicationWindow {
    id: root

    width: 1200
    height: 900

    title: i18nc("@title:window", "Camera Background Remover")

    property var currentCam: currentCameraModel.currentIndex.valid ? virtualCamerasModel.get(currentCameraModel.currentIndex.row) : null

    function updateCurrectCam() {
        if (currentCameraModel.currentIndex.valid) {
            currentCam = virtualCamerasModel.get(currentCameraModel.currentIndex.row);
        } else {
            currentCam = null;
        }
    }

    Connections {
        target: virtualCamerasModel

        onDataChanged: function () {
            updateCurrectCam();
        }
    }

    Connections {
        target: currentCameraModel

        onCurrentChanged: function () {
            updateCurrectCam();
        }
    }

    Component {
        id: aboutPage

        Kirigami.AboutPage {
            aboutData: About
        }
    }

    globalDrawer: CamerasDrawer {}

    Kirigami.Page {
        id: contextDrawerPage

        title: currentCameraModel.currentIndex.valid ? currentCam.name : i18nc("@title", "Camera Background Remover")

        Kirigami.Theme.colorSet: Kirigami.Theme.View

        actions: [
            Kirigami.Action {
                visible: currentCameraModel.currentIndex.valid
                icon.name: currentCam && currentCam.isActive ? "media-playback-stop-symbolic" : "media-playback-start-symbolic"
                text: currentCam && currentCam.isActive ? i18nc("@action:button", "Turn Off") : i18nc("@action:button", "Turn On")
                onTriggered: virtualCamerasModel.setActive(currentCameraModel.currentIndex.row, !currentCam.isActive)
            },
            Kirigami.Action {
                visible: currentCameraModel.currentIndex.valid
                icon.name: "edit-delete-symbolic"
                text: i18nc("@action:button", "Delete Camera")
                onTriggered: {
                    virtualCamerasModel.removeCamera(currentCameraModel.currentIndex.row);
                }
            },
            Kirigami.Action {
                id: showAboutPageAction
                icon.name: "documentinfo-symbolic"
                text: i18nc("@action:button", "About")
                onTriggered: pageStack.layers.push(aboutPage)
            }
        ]

        ColumnLayout {
            anchors.fill: parent
            spacing: Kirigami.Units.largeSpacing

            Kirigami.Heading {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: !currentCameraModel.currentIndex.valid
                text: i18n("Welcome! Please choose a camera to get started.")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }

            // Top row: video sources
            VideoSourcesView {
                visible: currentCameraModel.currentIndex.valid
                isActive: currentCameraModel.currentIndex.valid
            }

            // BackgroundImageGallery {
            //     visible: currentCameraModel.currentIndex.valid
            //     Kirigami.Theme.inherit: false
            // }
        }
    }

    pageStack.initialPage: [contextDrawerPage]
}
