#include "screenshot.h"

#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QDir>
#include <QMessageBox>

Screenshot::Screenshot(QObject *parent)
    : QObject{parent}
{
    connect(&m_screenshotTimer, &QTimer::timeout, this, &Screenshot::shootScreen);
    connect(&m_dateTimeCheckTimer, &QTimer::timeout, this, &Screenshot::dateTimeCheck);
    m_dateTimeCheckTimer.setInterval(1000);
}

void Screenshot::start()
{
    m_index = 0;
    m_screenshotTimer.setInterval(m_settings.interval());

    QDir dir;
    //Just in case, attempt to create the folder/path, if it doesn't exist
    dir.mkpath(m_settings.targetFolder());

    if(m_settings.useStartEndTime()){
        m_startTime = m_settings.startTime();
        m_endTime = m_settings.endTime();

        //we ignore seconds and use the minutes as start time
        m_startTime.setTime(QTime(m_startTime.time().hour(), m_startTime.time().minute(), 0));
        m_endTime.setTime(QTime(m_endTime.time().hour(), m_endTime.time().minute(), 0));

        //Check every second, if start time reached
        m_dateTimeCheckTimer.start();
    } else {
        m_startTime = QDateTime();
        m_endTime = QDateTime();
        m_dateTimeCheckTimer.stop();

        shootScreen();
        m_screenshotTimer.start();
    }
}

void Screenshot::stop()
{
    m_screenshotTimer.stop();
    m_dateTimeCheckTimer.stop();
    emit allDone();
}

void Screenshot::dateTimeCheck()
{
    auto cDateTime = QDateTime::currentDateTime();

    if(!m_screenshotTimer.isActive() && cDateTime > m_startTime ){
        shootScreen();
        m_screenshotTimer.start();
    }

    if(m_screenshotTimer.isActive() && cDateTime > m_endTime ) {
        m_screenshotTimer.stop();
        m_dateTimeCheckTimer.stop();

        emit allDone();
    }
}

void Screenshot::shootScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    auto pixmap = screen->grabWindow(0);

    auto path = m_settings.targetFolder();
    auto name = m_settings.nameTemplate();
    auto suffix = QStringLiteral("_%1%2.png").arg(m_index).arg(QDateTime::currentDateTime().toString("_dd.MM.yyyy_hh.mm"));

    bool ok = pixmap.save(path + name + suffix);

    if(!ok){
        QMessageBox::critical(nullptr, "Speichern fehlgeschlagen", "Screenshot konnte nicht gespeichert werden!");
    }

    m_index++;

    //Delete old and deprecated Screenshots
    if(auto max = m_settings.maxScreenshot();/* m_index > max && */max > 0){
        auto min = m_index - max;
        QDir screenshotFolder(path);
        const auto list(screenshotFolder.entryList(QDir::Files |QDir::QDir::NoDotAndDotDot));

        for( const auto &entry : list){
            if(entry.contains(name)){
                //Split string with name to remove potential '_' from the user
                const QString restString = entry.split(name).last();
                if(auto splitted = restString.split('_'); splitted.size() > 1){
                    QString number = splitted.at(1);
                    if(number.contains('.'))
                        //No DateTime added to file name -> number also contains '.png'
                        number = number.split('.').first();
                    if(number.toInt() < min || number.toInt() > m_index){
                        QFile::remove(screenshotFolder.absoluteFilePath(entry));
                    }
                }
            }
        }
    }
}
