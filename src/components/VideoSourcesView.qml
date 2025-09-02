import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

ColumnLayout {
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

    FormCard.FormComboBoxDelegate {
        id: cameraSourceCombo
        text: i18n("Camera Source")
        description: i18n("Camera from where you want to remove background.")
        displayMode: FormCard.FormComboBoxDelegate.ComboBox
        model: ["None", "Camera Source #1", "Camera Source #2"]
    }

    FormCard.FormComboBoxDelegate {
        id: backgroundTypeCombo
        text: i18n("Background Type")
        description: i18n("Select background type for replacement.")
        displayMode: FormCard.FormComboBoxDelegate.ComboBox
        model: ["Color", "Image", "Video"]
    }

    QQC2.Button {
        text: i18nc("@action:button", "Start")
        icon.name: "list-add"
        onClicked: videoFrameProvider.start()
    }
}
