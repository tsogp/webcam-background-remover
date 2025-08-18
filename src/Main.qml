import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

import org.kde.cbr 1.0

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

    globalDrawer: Kirigami.GlobalDrawer {}

    Kirigami.Page {
        title: "Empty page"
        id: emptyPage
    }

    Kirigami.Page {
        id: contextDrawerPage
        title: i18nc("@title", "Camera Background Remover")

        actions: [
            Kirigami.Action {
                id: showAboutPageAction
                icon.name: "documentinfo-symbolic"
                text: i18nc("@action:button", "About")
                onTriggered: pageStack.layers.push(aboutPage)
            }
        ]
    }

    pageStack.initialPage: [ emptyPage, contextDrawerPage ]
}
