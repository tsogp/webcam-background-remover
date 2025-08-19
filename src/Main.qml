import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import org.kde.cbr.components

Kirigami.ApplicationWindow {
    id: root

    width: 1200
    height: 900

    title: i18nc("@title:window", "Camera Background Remover")

    Component {
        id: aboutPage

        Kirigami.AboutPage {
            aboutData: About
        }
    }

    globalDrawer: CamerasDrawer {}

    Kirigami.Page {
        id: contextDrawerPage
        title: i18nc("@title", "Camera Background Remover")
        Kirigami.Theme.colorSet: Kirigami.Theme.View

        actions: [
            Kirigami.Action {
                id: showAboutPageAction
                icon.name: "documentinfo-symbolic"
                text: i18nc("@action:button", "About")
                onTriggered: pageStack.layers.push(aboutPage)
            }
        ]

        ColumnLayout {
            anchors.fill: parent
            spacing: Kirigami.Units.largeSpacing

            // Top row: video sources
            VideoSourcesView {}

            BackgroundImageGallery {
                Kirigami.Theme.inherit: false
            }
        }
    }

    pageStack.initialPage: [ contextDrawerPage ]
}
