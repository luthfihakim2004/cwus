#pragma once

#include <QObject>
#include <QTimer>

class Controller : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString title READ title NOTIFY dataChanged)
  Q_PROPERTY(QString artist READ artist NOTIFY dataChanged)
  Q_PROPERTY(QString album READ album NOTIFY dataChanged)
  Q_PROPERTY(QString timeStart READ timeStart NOTIFY dataChanged)
  Q_PROPERTY(QString timeEnd READ timeEnd NOTIFY dataChanged)
  Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY dataChanged)
  Q_PROPERTY(int position READ position NOTIFY dataChanged)
  Q_PROPERTY(int duration READ duration NOTIFY dataChanged)

public:
  explicit Controller(QObject *parent = nullptr);

  Q_INVOKABLE void playPause();
  Q_INVOKABLE void nextTrack();
  Q_INVOKABLE void prevTrack();
  Q_INVOKABLE void progressTrack(int val);

  QString title() const;
  QString artist() const;
  QString album() const;
  QString timeStart() const;
  QString timeEnd() const;
  bool isPlaying() const;
  int position() const;
  int duration() const;

signals:
  void dataChanged();

private:
  bool cmusAvailable = false;
  QTimer *tmuxCheckTimer;
  QTimer *statusTimer;
  QString m_title, m_artist, m_album, m_timeStart, m_timeEnd;
  bool m_isPlaying = false;
  int m_position = 0, m_duration = 0;

  bool sendToCmus(const QString &command);
  QString runCommand(const QString &cmd, const QStringList &args);
  void updateStatus();
  void checkTmux();
};
