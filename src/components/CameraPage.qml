import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

ColumnLayout {
    property int chosenCameraIdx: -1

    onChosenCameraIdxChanged: function (newValue) {
        console.log("chosenCameraIdxChanged: ", chosenCameraIdx)
    }

    anchors.fill: parent
    spacing: Kirigami.Units.largeSpacing

    Loader {
        id: mainLoader
        Layout.fillWidth: true
        Layout.fillHeight: true

        sourceComponent: {
            if (chosenCameraIdx === -1) {
                return welcomeView
            } else {
                return cameraView
            }
        }

        onLoaded: console.log("MainLoader loaded:", item)
        onSourceComponentChanged: console.log("Switched view, chosenCameraIdx:", chosenCameraIdx)
    }

    Component {
        id: welcomeView
        Kirigami.Heading {
            text: i18n("Welcome! Please choose a camera to get started.")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap

            Component.onCompleted: console.log("Welcome view created")
        }
    }

    Component {
        id: cameraView
        ColumnLayout {
            spacing: Kirigami.Units.largeSpacing

            VideoSourcesView {
                Component.onCompleted: console.log("VideoSourcesView created for idx:", chosenCameraIdx)
            }

            BackgroundImageGallery {
                Kirigami.Theme.inherit: false
                Component.onCompleted: console.log("BackgroundImageGallery created for idx:", chosenCameraIdx)
            }
        }
    }
}
