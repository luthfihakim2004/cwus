#include "loader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDebug>
#include <QTimer>

ThemeLoader::ThemeLoader(QObject *parent) : QObject(parent) {
    m_themeFilePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/cwus/colors.json";

    // Load theme for the first time
    loadTheme();

    // Set up file watching
    if (QFile::exists(m_themeFilePath)) {
        m_watcher.addPath(m_themeFilePath);

        connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
            qDebug() << "Theme file changed:" << path;

            QTimer::singleShot(200, this, [this, path]() {
                if (!QFile::exists(path)) {
                    qWarning() << "File doesn't exist, will retry later:" << path;
                    QTimer::singleShot(500, this, [this]() {
                        if (QFile::exists(m_themeFilePath)) {
                            qDebug() << "Re-adding theme file path to watcher";
                            m_watcher.addPath(m_themeFilePath);
                            loadTheme();
                        }
                    });
                } else {
                    // File exists, but may be temporarily locked or incomplete
                    QFile file(path);
                    if (!file.open(QIODevice::ReadOnly)) {
                        qWarning() << "Failed to open theme file (retrying):" << path;
                        QTimer::singleShot(300, this, [this]() {
                            loadTheme(); // Retry once more
                        });
                    } else {
                        file.close();
                        loadTheme();
                    }

                    // Re-add in case it was removed
                    if (!m_watcher.files().contains(path)) {
                        qDebug() << "Re-watching theme file";
                        m_watcher.addPath(path);
                    }
                }
            });
        });
    }
}

QString ThemeLoader::accent() const { return m_accent; }
QString ThemeLoader::darkaccent() const { return m_darkaccent; }
QString ThemeLoader::background() const { return m_background; }
QString ThemeLoader::foreground() const { return m_foreground; }

void ThemeLoader::loadTheme() {
    QFile file(m_themeFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open theme file:" << m_themeFilePath;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in theme file";
        return;
    }

    QJsonObject obj = doc.object();

    m_accent     = obj.value("accent").toString(m_accent);
    m_darkaccent = obj.value("darkaccent").toString(m_darkaccent);
    m_background = obj.value("background").toString(m_background);
    m_foreground = obj.value("foreground").toString(m_foreground);

    emit themeChanged(); // Trigger QML to update bindings
}
