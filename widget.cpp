#include "widget.h"
#include "ui_widget.h"
#include <QCloseEvent>
#include <QMessageBox>

#include <QFileSystemModel>
#include <QProcess>
#include <QFileDialog>
#include <QDesktopServices>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("QtAutoScreenshot");
    setAttribute(Qt::WA_DeleteOnClose,false);

    QFileSystemModel *model = new QFileSystemModel(this);
    model->setReadOnly(true);
    model->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    auto index = model->setRootPath(m_settings.targetFolder());
    ui->lvFiles->setModel(model);
    ui->lvFiles->setRootIndex(index);

    connect(ui->lvFiles, &QTreeView::doubleClicked, this, [=](const QModelIndex &index)->void {
        QDesktopServices::openUrl(QUrl(model->filePath(index)));
    });

    connect(model, &QFileSystemModel::rowsInserted, ui->lvFiles, [=, counter = 0]() mutable ->void{
        if(!(counter++ % 10 )){
            ui->lvFiles->resizeColumnToContents(0);
        }
    });

    //Setup ui via Settings;
    ui->lePath->setText(m_settings.targetFolder());
    connect(ui->lePath, &QLineEdit::editingFinished, this, &Widget::folderPathEdited);

    ui->sbInterval->setValue(m_settings.interval());
    connect(ui->sbInterval, QOverload<int>::of(&QSpinBox::valueChanged), &m_settings, &Settings::setInterval);

    ui->sbMax->setValue(m_settings.maxScreenshot());
    connect(ui->sbMax, QOverload<int>::of(&QSpinBox::valueChanged), &m_settings, &Settings::setMaxScreenshot);

    ui->leName->setText(m_settings.nameTemplate());
    connect(ui->leName, &QLineEdit::editingFinished, this, [=]()->void{m_settings.setNameTemplate(ui->leName->text());});

    ui->cbDateTime->setChecked(m_settings.useDateTime());
    connect(ui->cbDateTime, &QCheckBox::toggled, &m_settings, &Settings::setUseDateTime);

    ui->dteStart->setDateTime(m_settings.startTime());
    connect(ui->dteStart, &QDateTimeEdit::dateTimeChanged, &m_settings, &Settings::setStartTime);

    ui->dteEnd->setDateTime(m_settings.endTime());
    connect(ui->dteEnd, &QDateTimeEdit::dateTimeChanged, &m_settings, &Settings::setEndTime);

    ui->cbStartEnd->setChecked(m_settings.useStartEndTime());
    connect(ui->cbStartEnd, &QCheckBox::toggled, this, &Widget::onStartEndPointToggled);
    onStartEndPointToggled(m_settings.useStartEndTime());

    //Remaining connects
    connect(ui->leName, &QLineEdit::textChanged, this, &Widget::updateNameExample);
    connect(ui->cbDateTime, &QCheckBox::toggled, this, &Widget::updateNameExample);
    updateNameExample();


    connect(ui->btnStart, &QPushButton::clicked, this, &Widget::start);
    connect(ui->btnStartAndHide, &QPushButton::clicked, this, &Widget::startAndHide);
    connect(ui->btnStop, &QPushButton::clicked, this, &Widget::stopAutoScreenshots);

    connect(ui->btnSelectFolder, &QPushButton::clicked, this, &Widget::selectSaveFolder);

    ui->gb1->setTitle(tr("Location"));
    ui->btnSelectFolder->setText(tr("Select folder"));
    ui->gb2->setTitle(tr("Settings"));
    ui->lblMax->setText(tr("max number of screenshots"));
    ui->lblInterval->setText(tr("interval [ms]"));
    ui->lblNameTemplate->setText(tr("name template"));
    ui->cbDateTime->setText(tr("date/Time in the name"));
    ui->lblStart->setText(tr("start"));
    ui->lblEnd->setText(tr("end"));
    ui->cbStartEnd->setText(tr("use start and end time"));
    ui->gb3->setTitle(tr("Start"));
    ui->btnStart->setText("start");
    ui->btnStartAndHide->setText("start and hide");
    ui->btnStop->setText("stop");
    ui->lblStartInfo->setText(tr("Old screenshots with the same name template will be deleted automatically!"));
    ui->lblStartHideInfo->setText(tr("Start and end time needed!"));
    ui->gb4->setTitle(tr("Screenshots"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onAllDone()
{
    show();
    disableUi(false);
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(ui->btnStop->isEnabled()){
        QMessageBox::warning(this, "Warnung, Prozess läuft im Hintergrund", "Bitte Screenshot Prozess zuvor beenden!");
        event->ignore();
    }else{
        event->accept();
        qApp->quit();
    }
}

void Widget::updateNameExample() noexcept
{
    QString txt = ui->leName->text() + QStringLiteral("_1%1.png").arg(ui->cbDateTime->isChecked() ? QDateTime::currentDateTime().toString("_dd.MM.yyyy_hh.mm") : "");
    ui->lblNameExample->setText(txt);
}

void Widget::start() noexcept
{
    emit startAutoScreenshots();
    disableUi(true);
}

void Widget::startAndHide() noexcept
{
    emit startAutoScreenshots();
    hide();
    disableUi(true);
}

void Widget::folderPathEdited() noexcept
{
    m_settings.setTargetFolder(ui->lePath->text());
    QFileSystemModel *model = qobject_cast<QFileSystemModel*>(ui->lvFiles->model());
    if(model){
        auto index = model->setRootPath(ui->lePath->text());
        ui->lvFiles->setRootIndex(index);
    }
}

void Widget::selectSaveFolder() noexcept
{
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ui->lePath->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty())
        return;

    ui->lePath->setText(dir);
    folderPathEdited();
}

void Widget::onStartEndPointToggled(bool toggled) noexcept
{
    m_settings.setUseStartEndTime(toggled);
    ui->dteStart->setEnabled(toggled);
    ui->dteEnd->setEnabled(toggled);
    ui->btnStartAndHide->setEnabled(toggled);

    if(toggled){
        auto dt = QDateTime::currentDateTime();
        ui->dteStart->setDateTime(dt);
        dt.setTime({dt.time().hour(), dt.time().minute() +1,dt.time().second()});
        ui->dteEnd->setDateTime(dt);
    }
}

void Widget::disableUi(bool disable)
{
    ui->gb1->setEnabled(!disable);
    ui->gb2->setEnabled(!disable);
    ui->btnStart->setEnabled(!disable);
    ui->btnStartAndHide->setEnabled(!disable);
    ui->btnStop->setEnabled(disable);
}
