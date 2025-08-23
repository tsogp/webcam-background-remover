import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami

// Heavily inspired by KDEConnect's Main page impelementation
// https://invent.kde.org/network/kdeconnect-kde/-/blob/master/app/qml/Main.qml?ref_type=heads
Kirigami.OverlayDrawer {
    id: drawer
    edge: Qt.application.layoutDirection === Qt.RightToLeft ? Qt.RightEdge : Qt.LeftEdge
    modal: applicationWindow().width < Kirigami.Units.gridUnit * 50 && !collapsed
    width: Kirigami.Units.gridUnit * 16
    onModalChanged: drawerOpen = !modal

    Behavior on width {
        NumberAnimation {
            duration: Kirigami.Units.longDuration
            easing.type: Easing.InOutQuad
        }
    }
    Kirigami.Theme.colorSet: Kirigami.Theme.View

    handleClosedIcon.source: modal ? null : "sidebar-expand-left-symbolic"
    handleOpenIcon.source: modal ? null : "sidebar-collapse-left-symbolic"
    handleVisible: modal

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    AddVirtualCameraDialog {
        id: virtualCameraDialog
    }

    contentItem: ColumnLayout {
        spacing: 0

        QQC2.ToolBar {
            Layout.fillWidth: true
            Layout.preferredHeight: pageStack.globalToolBar.preferredHeight

            contentItem: RowLayout {
                spacing: 0

                Kirigami.Heading {
                    text: i18n("Virtual Cameras")
                    elide: Qt.ElideRight
                    Layout.fillWidth: true
                    Layout.leftMargin: Kirigami.Units.largeSpacing
                }

                QQC2.ToolButton {
                    text: i18nc("@action:button", "Add")
                    icon.name: "list-add-symbolic"
                    onClicked: virtualCameraDialog.open()
                }
            }
        }

        QQC2.ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: cameras
                Layout.fillWidth: true
                clip: true
                activeFocusOnTab: true
                keyNavigationEnabled: true
                model: virtualCamerasModel

                // TODO: think of the way to have a sorted view for this to work
                // section.property: "isActive"
                // section.delegate: Kirigami.ListSectionHeader {
                //     width: ListView.view.width
                //     text: section == "true" ? "Active" : "Inactive"
                // }

                Kirigami.PlaceholderMessage {
                    text: i18n("Virtual cameras will appear here")
                    icon.name: 'documentinfo-symbolic'
                    anchors.centerIn: parent
                    width: parent.width - (Kirigami.Units.largeSpacing * 4)
                    visible: cameras.count === 0
                }

                delegate: QQC2.ItemDelegate {
                    id: delegate
                    width: ListView.view.width
                    highlighted: false

                    property string cameraName: model.name
                    property url sourceUrl: model.sourceUrl
                    property url cameraUrl: model.cameraUrl
                    property bool cameraActive: model.isActive

                    contentItem: RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: delegate.leftPadding
                        anchors.rightMargin: delegate.rightPadding
                        anchors.topMargin: delegate.topPadding
                        anchors.bottomMargin: delegate.bottomPadding
                        spacing: Kirigami.Units.smallSpacing

                        Kirigami.IconTitleSubtitle {
                            Layout.fillWidth: true
                            title: cameraName
                            subtitle: cameraUrl.toString()
                            icon.name: "camera-video-symbolic"
                        }

                        Rectangle {
                            width: Kirigami.Units.iconSizes.small
                            height: Kirigami.Units.iconSizes.small
                            radius: width / 2
                            color: cameraActive ? "green" : "red"
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }

                    onClicked: {
                        cameras.currentIndex = index
                        currentCameraModel.setCurrentIndex(
                            virtualCamerasModel.index(index, 0),
                            ItemSelectionModel.ClearAndSelect
                        )
                        console.log(currentCameraModel.currentIndex);
                    }
                }
            }
        }
    }
}
