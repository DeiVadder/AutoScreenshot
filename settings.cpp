#include "settings.h"

#include <QStandardPaths>

const auto FolderKey = "ScreenshotLocation";
const auto IntervalKey = "IntervalBetweenScreenshots";
const auto MaxScreenshotKey = "MaxCountOfScreenshots";
const auto NameTemplateKey = "NameTemplate";
const auto DateTimeKey = "AppendTimeStamp";
const auto UseStartEndKey = "ScreenshotsTimeWindow";
const auto StartTimeKey = "TimeWindowStart";
const auto EndTimeKey = "TimeWindowEnd";

Settings::Settings(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings(QString(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)) + "/AutoScreenshotConfig.ini", QSettings::IniFormat);

}

Settings::~Settings()
{
    delete m_settings;
}

void Settings::setTargetFolder(const QString &folder) noexcept
{
    m_settings->setValue(FolderKey, folder);
}

QString Settings::targetFolder() const noexcept
{
    QString folder = m_settings->value(FolderKey, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+ "/Screenshots").toString();
    if(!folder.endsWith(QChar('/')))
        folder.append(QChar('/'));
    return folder;
}

void Settings::setInterval(int interval) noexcept
{
    m_settings->setValue(IntervalKey, interval);
}

int Settings::interval() const noexcept
{
    return m_settings->value(IntervalKey, 1000).toInt();
}

void Settings::setMaxScreenshot(int count) noexcept
{
    m_settings->setValue(MaxScreenshotKey, count);
}

int Settings::maxScreenshot() const noexcept
{
    return m_settings->value(MaxScreenshotKey, -1).toInt();
}

void Settings::setNameTemplate(const QString &name) noexcept
{
    m_settings->setValue(NameTemplateKey, name);
}

QString Settings::nameTemplate() const noexcept
{
    return m_settings->value(NameTemplateKey, QString("AutoScreenshot")).toString();
}

void Settings::setUseDateTime(bool useDateTime) noexcept
{
    m_settings->setValue(DateTimeKey, useDateTime);
}

bool Settings::useDateTime() const noexcept
{
    return m_settings->value(DateTimeKey, true).toBool();
}

void Settings::setUseStartEndTime(bool useInterval) noexcept
{
    m_settings->setValue(UseStartEndKey, useInterval);
}

bool Settings::useStartEndTime() const noexcept
{
    return m_settings->value(UseStartEndKey, false).toBool();
}

void Settings::setStartTime(QDateTime time) noexcept
{
    m_settings->setValue(StartTimeKey, time);
}

QDateTime Settings::startTime() const noexcept
{
    return m_settings->value(StartTimeKey, QDateTime::currentDateTime()).toDateTime();
}

void Settings::setEndTime(QDateTime time) noexcept
{
    m_settings->setValue(EndTimeKey, time);
}

QDateTime Settings::endTime() const noexcept
{
    return m_settings->value(EndTimeKey, QDateTime::currentDateTime()).toDateTime();
}
