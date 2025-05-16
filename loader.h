#pragma once

#include <QObject>
#include <QColor>
#include <QJsonObject>
#include <QFileSystemWatcher>

class ThemeLoader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString accent READ accent NOTIFY themeChanged)
    Q_PROPERTY(QString darkaccent READ darkaccent NOTIFY themeChanged)
    Q_PROPERTY(QString background READ background NOTIFY themeChanged)
    Q_PROPERTY(QString foreground READ foreground NOTIFY themeChanged)

public:
    explicit ThemeLoader(QObject *parent = nullptr);
    QString accent() const;
    QString darkaccent() const;
    QString background() const;
    QString foreground() const;

signals:
    void themeChanged();

private:
    QString m_accent = "#ff00ff";
    QString m_darkaccent = "#ff00ff";
    QString m_background = "#000000";
    QString m_foreground = "#ffffff";

    QFileSystemWatcher m_watcher;
    QString m_themeFilePath;

    void loadTheme();
};
