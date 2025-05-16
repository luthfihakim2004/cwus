#include "controller.h"
#include "loader.h"
#include <LayerShellQt/shell.h>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <LayerShellQt/Shell>
#include <QQmlContext>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // This must go first
    LayerShellQt::Shell::useLayerShell();

    QQmlApplicationEngine engine;
    ThemeLoader themeLoader;

    qmlRegisterType<Controller>("Cwus", 1, 0, "Controller");
    engine.rootContext()->setContextProperty("themeLoader", &themeLoader);

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Get the root QQuickWindow
    QObject *rootObject = engine.rootObjects().first();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject);

    if (!window) {
        qWarning("Root object is not a QQuickWindow!");
        return -1;
    }

    // Apply LayerShell settings BEFORE show()
    auto *layerWindow = LayerShellQt::Window::get(window);
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);
    layerWindow->setAnchors(
        QFlags<LayerShellQt::Window::Anchor>(
            LayerShellQt::Window::Anchor::AnchorTop |
            LayerShellQt::Window::Anchor::AnchorLeft |
            LayerShellQt::Window::Anchor::AnchorRight
            )
        );
    layerWindow->setExclusiveZone(-1);
    layerWindow->setMargins(QMargins(110,3,1390,0));

    window->show(); // Always call show() after setting up LayerShell

    return app.exec();
}
