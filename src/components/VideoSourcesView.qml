import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import QtQuick.Dialogs
import org.kde.kirigamiaddons.formcard as FormCard

ColumnLayout {
    property string backgroundFileName: ""
    property bool isActive: false

    onIsActiveChanged: {
        if (isActive) {
            videoFrameProvider.start();
        } else {
            videoFrameProvider.stop();
        }
    }

    Connections {
        target: videoFrameProvider
        function onFrameReady(before, after) {
            beforeImg.source = before;
            afterImg.source = after;
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.preferredHeight: 300
        spacing: Kirigami.Units.largeSpacing

        Image {
            id: beforeImg
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: afterImg
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            fillMode: Image.PreserveAspectFit
        }
    }

    Kirigami.Heading {
        text: i18n("Camera Settings")
        level: 2
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        padding: Kirigami.Units.smallSpacing
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: Kirigami.Units.largeSpacing

        QQC2.Button {
            text: i18n("Choose Background Image")
            icon.name: "folder-image"
            onClicked: backgroundFileDialog.open()
        }

        Item {
            Layout.fillWidth: true
        }

        QQC2.Label {
            text: backgroundFileName === "" ? i18n("No background selected") : i18n("Selected: %1", backgroundFileName)
            wrapMode: Text.NoWrap
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    FileDialog {
        id: backgroundFileDialog
        title: i18n("Select Background Image")
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp)"]
        onAccepted: {
            videoFrameProvider.setBackgroundImage(backgroundFileDialog.selectedFile);
            backgroundFileName = backgroundFileDialog.selectedFile.toString().split("/").pop();
        }
    }

    Item {
        Layout.fillHeight: true
    }
}
