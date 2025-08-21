#include "virtualcameramodel.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KIconTheme>
#include <KAboutData>
#include <QString>

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("cbr");
    QApplication::setOrganizationName(QStringLiteral("KDE"));
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QApplication::setApplicationName(QStringLiteral("Camera Background Remover"));
    QApplication::setDesktopFileName(QStringLiteral("org.kde.cbr"));

    QApplication::setStyle(QStringLiteral("breeze"));
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    KAboutData aboutData(
        QStringLiteral("cbr"),
        i18nc("@title", "Camera Background Remover"),
        QStringLiteral("1.0"),
        i18n("Application to remove background from webcam in real time"),
        KAboutLicense::CC0_V1, // TODO: check the licence when including the model
        i18n("(c) 2025"));

    aboutData.addAuthor(
        i18nc("@info:credit", "Pavel Potemkin"),
        i18nc("@info:credit", "Developer"),
        QStringLiteral("potemkinpavel3@gmail.com"),
        QStringLiteral("https://github.com/tsogp"));

    KAboutData::setApplicationData(aboutData);

    qmlRegisterSingletonType(
        "org.kde.cbr",
        1, 0, // TODO: Major and minor versions of the import
        "About",
        [](QQmlEngine* engine, QJSEngine *) -> QJSValue {
            return engine->toScriptValue(KAboutData::applicationData());
        }
    );

    VirtualCameraModel cameraModel;
    // TODO: remove fake cameras after testing is done
    cameraModel.addCamera(
        QStringLiteral("Main Camera"),
        QUrl(QStringLiteral("file:///dev/video0")),
        QUrl(QStringLiteral("file:///dev/video00")),
        false
    );
    cameraModel.addCamera(
        QStringLiteral("Main Camera1"), 
        QUrl(QStringLiteral("file:///dev/video1")), 
        QUrl(QStringLiteral("file:///dev/video11")), 
        true
    );
    cameraModel.addCamera(
        QStringLiteral("Main Camera1"), 
        QUrl(QStringLiteral("file:///dev/video1")), 
        QUrl(QStringLiteral("file:///dev/video11")), 
        true
    );
    cameraModel.addCamera(
        QStringLiteral("Main Camera2"), 
        QUrl(QStringLiteral("file:///dev/video2")), 
        QUrl(QStringLiteral("file:///dev/video22")), 
        false
    );


    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.rootContext()->setContextProperty(QStringLiteral("virtualCamerasModel"), &cameraModel);
    engine.loadFromModule("org.kde.cbr", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
