import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

ColumnLayout {
    RowLayout {
        Layout.fillWidth: true
        Layout.preferredHeight: 300
        spacing: Kirigami.Units.largeSpacing

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            color: "#444"
            radius: 8
            border.color: "#888"
            Text {
                anchors.centerIn: parent
                text: "Video Source 1"
                color: "white"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            color: "#444"
            radius: 8
            border.color: "#888"
            Text {
                anchors.centerIn: parent
                text: "Video Source 2"
                color: "white"
            }
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
}