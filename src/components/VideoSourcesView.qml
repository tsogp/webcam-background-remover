import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import QtQuick.Dialogs
import org.kde.kirigamiaddons.formcard as FormCard

ColumnLayout {
    property string backgroundFileName: ""

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

    QQC2.Button {
        text: i18n("Choose Background Image")
        icon.name: "folder-image"
        onClicked: backgroundFileDialog.open()
    }

    QQC2.Label {
        text: backgroundFileName === "" ? i18n("No background selected") : i18n("Selected: %1", backgroundFileName)
        wrapMode: Text.Wrap
        Layout.fillWidth: true
    }

    FileDialog {
        id: backgroundFileDialog
        title: i18n("Select Background Image")
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp)"]
        onAccepted: {
            videoFrameProvider.setBackgroundImage(backgroundFileDialog.selectedFile)
            backgroundFileName = backgroundFileDialog.selectedFile.toString().split("/").pop() // just filename
        }
    }

    QQC2.Button {
        text: i18nc("@action:button", "Start")
        icon.name: "list-add"
        onClicked: videoFrameProvider.start()
    }
}
