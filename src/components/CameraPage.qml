import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

ColumnLayout {
    property int chosenCameraIdx

    anchors.fill: parent
    spacing: Kirigami.Units.largeSpacing

    Loader {
        Layout.fillWidth: true
        Layout.fillHeight: true
        active: chosenCameraIdx === -1
        sourceComponent: Kirigami.Heading {
            text: i18n("Welcome! Please choose a camera to get started.")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap
        }
    }

    Loader {
        id: cameraLoader
        Layout.fillWidth: true
        Layout.fillHeight: true
        active: chosenCameraIdx !== -1
        sourceComponent: ColumnLayout {
            spacing: Kirigami.Units.largeSpacing

            // Top row: video sources
            VideoSourcesView {}

            BackgroundImageGallery {
                Kirigami.Theme.inherit: false
            }
        }
    }
}
