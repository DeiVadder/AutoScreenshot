#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "settings.h"

#include <QDateTime>
#include <QTimer>

class Screenshot : public QObject
{
    Q_OBJECT
public:
    explicit Screenshot(QObject *parent = nullptr);

public slots:
    void start();
    void stop();


signals:
    void allDone();


private slots:
    void dateTimeCheck();
    void shootScreen();

private:
    QDateTime m_startTime;
    QDateTime m_endTime;


    QTimer m_screenshotTimer;
    QTimer m_dateTimeCheckTimer;

    Settings m_settings;

    qint64 m_index{0};
};

#endif // SCREENSHOT_H
