import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami

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