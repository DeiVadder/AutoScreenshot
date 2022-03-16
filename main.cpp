#include "widget.h"
#include "screenshot.h"

#include <QApplication>
#include <QStandardPaths>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << path;
    Screenshot screenshot;
//    sshot.shootScreen();
    Widget w;
    w.show();

    QObject::connect(&w, &Widget::startAutoScreenshots, &screenshot, &Screenshot::start, Qt::QueuedConnection);
    QObject::connect(&w, &Widget::stopAutoScreenshots, &screenshot, &Screenshot::stop);

    QObject::connect(&screenshot, &Screenshot::allDone, &w, &Widget::onAllDone);

    return a.exec();
}
