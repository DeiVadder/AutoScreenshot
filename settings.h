#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QObject>
#include <QDateTime>

class Settings : public QObject
{
public:
    explicit Settings(QObject *parent = nullptr);
    ~Settings();

    void setTargetFolder(const QString &folder) noexcept;
    [[nodiscard]] QString targetFolder() const noexcept;

    void setInterval(int interval) noexcept;
    [[nodiscard]] int interval() const noexcept;

    void setMaxScreenshot(int count) noexcept;
    [[nodiscard]] int maxScreenshot() const noexcept;

    void setNameTemplate(const QString &name) noexcept;
    [[nodiscard]] QString nameTemplate() const noexcept;

    void setUseDateTime(bool useDateTime) noexcept;
    [[nodiscard]] bool useDateTime() const noexcept;

    void setUseStartEndTime(bool useInterval) noexcept;
    [[nodiscard]] bool useStartEndTime() const noexcept;

    void setStartTime(QDateTime time) noexcept;
    QDateTime startTime() const noexcept;

    void setEndTime(QDateTime time) noexcept;
    QDateTime endTime() const noexcept;

private:
    QSettings *m_settings;
};

#endif // SETTINGS_H
