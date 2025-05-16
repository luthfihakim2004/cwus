#include "controller.h"
#include <QProcess>
#include <QRegularExpression>

Controller::Controller(QObject *parent)
    : QObject(parent), statusTimer(new QTimer(this)) {

  checkTmux();
  tmuxCheckTimer = new QTimer(this);
  connect(tmuxCheckTimer, &QTimer::timeout, this, &Controller::checkTmux);
  tmuxCheckTimer->start(1000); // check tmux every 3 seconds
  connect(statusTimer, &QTimer::timeout, this, &Controller::updateStatus);
  statusTimer->start(1000); // check status every 1 second
  updateStatus();
}

QString Controller::runCommand(const QString &cmd, const QStringList &args) {
  QProcess proc;
  proc.start(cmd, args);
  proc.waitForFinished();
  return proc.readAllStandardOutput();
}

void Controller::updateStatus() {
  QString output = runCommand("cmus-remote", {"-Q"});
  if (output.isEmpty()) {
    //qDebug() << "cmus-remote returned empty output, retrying...";
    QTimer::singleShot(1000, this, &Controller::updateStatus);
    return;
  }

  QStringList lines = output.split('\n');
  for (const QString &line : lines) {
    if (line.startsWith("tag title ")) {
      m_title = line.section(' ', 2);
    } else if (line.startsWith("tag artist ")) {
      m_artist = line.section(' ', 2);
    } else if (line.startsWith("tag album ")) {
      m_album = line.section(' ', 2);
    } else if (line.startsWith("duration ")) {
      m_duration = line.section(' ', 1).toInt();
    } else if (line.startsWith("position ")) {
      m_position = line.section(' ', 1).toInt();
    } else if (line.startsWith("status ")) {
      m_isPlaying = (line.section(' ', 1) == "playing");
    }
  }

  m_timeStart = QString("%1:%2")
                    .arg(m_position / 60, 2, 10, QLatin1Char('0'))
                    .arg(m_position % 60, 2, 10, QLatin1Char('0'));
  m_timeEnd = QString("%1:%2")
                  .arg(m_duration / 60, 2, 10, QLatin1Char('0'))
                  .arg(m_duration % 60, 2, 10, QLatin1Char('0'));

  emit dataChanged();
}

void Controller::checkTmux() {
  QString output = runCommand("tmux", {"list-sessions"});
  //qDebug() << "TMUX output:" << output; // Add this line
  bool sessionExists = output.contains("cmus");

  if (!sessionExists) {
    cmusAvailable = sessionExists;
    // emit cmusAvailabilityChanged(); // Optional signal if UI should react
    //qDebug() << "cmusAvailable changed:" << cmusAvailable; // Add this too
    runCommand("tmux", {"new-session", "-d", "-s", "cmus", "cmus"});
    QTimer::singleShot(1000, this, &Controller::updateStatus);
  }
  if (sessionExists != cmusAvailable) {
    cmusAvailable = sessionExists;
    //qDebug() << "cmusAvailable changed:" << cmusAvailable;
    if (cmusAvailable) {
      int retryCount = 0;
      QTimer::singleShot(1000, this, &Controller::updateStatus);
    }
  }
}

bool Controller::sendToCmus(const QString &command) {
  if (!cmusAvailable)
    return false;

  runCommand("tmux", {"send-keys", "-t", "cmus", command});
  return true;
}

void Controller::playPause() {
  this->sendToCmus("c");
  updateStatus();
}

void Controller::nextTrack() {
  this->sendToCmus("b");
  updateStatus();
}

void Controller::prevTrack() {
  this->sendToCmus("z");
  updateStatus();
}

void Controller::progressTrack(int val) {
  if (!cmusAvailable)
    return;
  runCommand("cmus-remote", {"-k", QString::number(val)});
  updateStatus();
}

// Getters
QString Controller::title() const { return m_title; }
QString Controller::artist() const { return m_artist; }
QString Controller::album() const { return m_album; }
QString Controller::timeStart() const { return m_timeStart; }
QString Controller::timeEnd() const { return m_timeEnd; }
bool Controller::isPlaying() const { return m_isPlaying; }
int Controller::position() const { return m_position; }
int Controller::duration() const { return m_duration; }
