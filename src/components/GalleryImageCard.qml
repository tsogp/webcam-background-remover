import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Item {
    id: card
    width: 200
    height: 160

    property alias source: preview.source
    property string title: ""
    property string author: ""

    Rectangle {
        id: frame
        anchors.fill: parent
        radius: 6
        color: Kirigami.Theme.backgroundColor

        ColumnLayout {
            anchors.fill: parent
            spacing: 4

            // TODO: fix border not showing
            Rectangle {
                id: imageContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 110
                border.color: Kirigami.Theme.hoverColor
                border.width: cardMouse.containsMouse ? 3 : 0
                clip: true

                Image {
                    id: preview
                    source: card.source
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    anchors.fill: parent
                }

                // TODO: fix border not showing
                QQC2.Button {
                    id: actionButton
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: Kirigami.Units.smallSpacing
                    icon.name: "folder-open"
                    visible: cardMouse.containsMouse
                }
            }

            // Title and author
            ColumnLayout {
                Layout.fillWidth: true

                QQC2.Label {
                    Layout.fillWidth: true
                    text: card.title
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                }

                QQC2.Label {
                    Layout.fillWidth: true
                    text: card.author
                    font.pointSize: 9
                    color: Kirigami.Theme.disabledTextColor
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    visible: text.length > 0
                    wrapMode: Text.Wrap
                }
            }
        }
    }

    MouseArea {
        id: cardMouse
        anchors.fill: parent
        hoverEnabled: true
    }
}
