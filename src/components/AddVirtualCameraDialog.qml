import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

Kirigami.PromptDialog {
    id: addVirtualCameraDialog
    title: i18n("New Virtual Camera")

    preferredWidth: Kirigami.Units.gridUnit * 32

    standardButtons: Kirigami.Dialog.NoButton
    customFooterActions: [
        Kirigami.Action {
            id: createVirtualCameraButton
            text: qsTr("Create Virtual Camera")
            icon.name: "dialog-ok-symbolic"
            enabled: cameraSourceCombo.currentIndex > 0
            onTriggered: {
                addVirtualCameraDialog.close();
            }
        },
        Kirigami.Action {
            id: cancelVirtualCameraCreationButton
            text: qsTr("Cancel")
            icon.name: "dialog-cancel-symbolic"
            onTriggered: {
                addVirtualCameraDialog.close();
            }
        }
    ]
    ColumnLayout {
        FormCard.FormComboBoxDelegate {
            id: cameraSourceCombo
            text: i18n("Camera Source")
            description: i18n("Camera from where you want to remove background.")
            displayMode: FormCard.FormComboBoxDelegate.ComboBox
            model: ["None", "Camera Source #1", "Camera Source #2"]
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormTextFieldDelegate {
            label: i18n("Name of the virtual camera")
            enabled: cameraSourceCombo.currentIndex > 0
            placeholderText: i18n("Enter name of the virtual camera")
        }
    }
}
