import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

Kirigami.PromptDialog {
    id: addVirtualCameraDialog
    title: i18n("New Virtual Camera")

    preferredWidth: Kirigami.Units.gridUnit * 32

    property bool nameError: false

    standardButtons: Kirigami.Dialog.NoButton

    customFooterActions: [
        Kirigami.Action {
            id: createVirtualCameraButton
            text: qsTr("Create Virtual Camera")
            icon.name: "dialog-ok-symbolic"
            enabled: cameraSourceCombo.currentIndex > 0 && cameraNameInput.text.length !== 0
            onTriggered: {
                // Construct the device path
                let devPath = "/dev/video" + cameraNumberChooser.value;

                // Call into model
                let cameraIdx = virtualCamerasModel.addCamera(
                    cameraNameInput.text,
                    cameraSourceCombo.currentValue,
                    devPath,
                    false
                );

                if (cameraIdx === -1) {
                    addVirtualCameraDialog.nameError = true;
                    return;
                }
                addVirtualCameraDialog.nameError = false;

                currentCameraModel.setCurrentIndex(
                    virtualCamerasModel.index(cameraIdx, 0),
                    ItemSelectionModel.ClearAndSelect
                );

                cameraSourceCombo.currentIndex = 0;
                cameraNameInput.text = "";
                cameraNumberChooser.value = 10; // reset to default

                addVirtualCameraDialog.close();
            }
        },
        Kirigami.Action {
            id: cancelVirtualCameraCreationButton
            text: qsTr("Cancel")
            icon.name: "dialog-cancel-symbolic"
            onTriggered: addVirtualCameraDialog.close()
        }
    ]

    ColumnLayout {
        FormCard.FormComboBoxDelegate {
            id: cameraSourceCombo
            leftPadding: 0
            rightPadding: 0
            currentIndex: 0
            text: i18n("Camera Source")
            description: i18n("Camera from where you want to remove background.")
            displayMode: FormCard.FormComboBoxDelegate.ComboBox
            model: ["None"].concat(cameraManager.listPhysicalCaptureDevices())
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormTextFieldDelegate {
            id: cameraNameInput
            leftPadding: 0
            rightPadding: 0
            label: i18n("Name of the virtual camera")
            enabled: cameraSourceCombo.currentIndex > 0
            placeholderText: i18n("Enter name of the virtual camera")

            onTextChanged: function (text) {
                nameError = false;
            }
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormSpinBoxDelegate {
            enabled: cameraSourceCombo.currentIndex > 0
            id: cameraNumberChooser
            leftPadding: 0
            rightPadding: 0
            label: i18n("Device number (will be used to construct the path like /dev/videoX)")
            from: 1
            to: 64
            value: 10
        }

        Kirigami.InlineMessage {
            Layout.fillWidth: true
            visible: addVirtualCameraDialog.nameError
            type: Kirigami.MessageType.Error
            text: i18n("Camera with name %1 already exists. Choose another one.", cameraNameInput.text)
        }
    }
}
