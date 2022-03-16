#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void onAllDone();

signals:
    void startAutoScreenshots();
    void stopAutoScreenshots();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void updateNameExample() noexcept;

    void start() noexcept;
    void startAndHide() noexcept;

    void folderPathEdited() noexcept;
    void selectSaveFolder() noexcept;

    void onStartEndPointToggled(bool toggled) noexcept;

private:
    void disableUi(bool disable);

private:
    Ui::Widget *ui;
    Settings m_settings;
};
#endif // WIDGET_H
