// BackgroundImageGallery.qml

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami

ColumnLayout {
    id: imageGallery
    spacing: Kirigami.Units.largeSpacing
    Layout.fillWidth: true
    Layout.fillHeight: true

    Kirigami.Separator {}

    Rectangle {
        Layout.fillWidth: true
        color: Kirigami.Theme.backgroundColor
        Kirigami.Theme.colorSet: Kirigami.Theme.Complementary

        RowLayout {
            id: row
            anchors.fill: parent
            anchors.margins: Kirigami.Units.smallSpacing

            QQC2.Label {
                text: i18nc("@title", "Images")
                font.bold: true
                color: Kirigami.Theme.textColor
            }

            Item { Layout.fillWidth: true }

            QQC2.Button {
                text: i18nc("@action:button", "Add New")
                icon.name: "list-add"
            }
        }

        // Make rectangle automatically match row content
        height: row.implicitHeight + 2 * Kirigami.Units.smallSpacing
    }


    Kirigami.Separator {}

    GridView {
        id: imageGrid
        Layout.fillWidth: true
        Layout.fillHeight: true
        cellWidth: 220
        cellHeight: 180
        clip: true

        // TODO: fix paths rendering
        model: ListModel {
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Fallen Leaf"; author: "Martin Klapetek" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Flow"; author: "Sandra Smukaste" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Kite"; author: "Risto Saukonpää" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Fallen Leaf"; author: "Martin Klapetek" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Flow"; author: "Sandra Smukaste" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Kite"; author: "Risto Saukonpää" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Fallen Leaf"; author: "Martin Klapetek" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Flow"; author: "Sandra Smukaste" }
            ListElement { source: "file:///~/.local/share/wallpapers/Vivid-Line-Wallpaper-With-Plasma-Logo.png"; title: "Kite"; author: "Risto Saukonpää" }
        }

        delegate: GalleryImageCard {
            source: model.source
            title: model.title
            author: model.author
        }
    }
}
