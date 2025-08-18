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
    Kirigami.Theme.colorSet: Kirigami.Theme.Window

    handleClosedIcon.source: modal ? null : "sidebar-expand-left-symbolic"
    handleOpenIcon.source: modal ? null : "sidebar-collapse-left-symbolic"
    handleVisible: modal

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

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
                }
            }
        }

        QQC2.ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {
                spacing: Kirigami.Units.smallSpacing
                // camera cards will go here
            }
        }
    }
}