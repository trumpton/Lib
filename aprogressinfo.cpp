#include "aprogressinfo.h"
#include "ui_aprogressinfo.h"
#include "aprogressbar.h"
#include "atextedit.h"
#include "apushbutton.h"
#include "alertsound.h"
#include "dialogfocus.h"
#include "supportfunctions.h"

#include <QCoreApplication>
#include <QtGlobal>

AProgressInfo::AProgressInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AProgressInfo)
{
    ui->setupUi(this) ;
    logView = ui->updateStatusReport ;
    barView = ui->progressBar ;
    buttonView = ui->pushButton ;
    complete = 100 ;
    running = false ;
    aborted = false ;
    finished = false ;
    barView->setRange(0, complete) ;
}

AProgressInfo::~AProgressInfo()
{
    delete ui ;
}

void AProgressInfo::logLine(const QString &line)
{
    if (!line.isEmpty()) logView->appendPlainText(line) ;
}

void AProgressInfo::setPartNames(const QString &logName, const QString &barName)
{
    logView->setAccessibleName(logName) ;
    barView->setAccessibleName(barName) ;
}

void AProgressInfo::startProgress(const QString &task, const QString &runningText)
{
    QString title = dialogTitle(task) ;
    setWindowTitle(title) ;
    // The accessible name is the title without its dash, as the appointment form names its dialog (Contact Manager Appointment)
    setAccessibleName(QString(title).replace(" - ", " ")) ;
    running = true ;
    aborted = false ;
    finished = false ;
    barView->setRange(0, complete) ;
    barView->setValue(0) ;
    setModal(true) ;
    buttonView->setAccessibleName(runningText) ;
    buttonView->setText(runningText) ;
    show() ;
    buttonView->setFocus() ;
}

void AProgressInfo::setCompleteValue(int value)
{
    complete = value ;
    barView->setRange(0, complete) ;
}

int AProgressInfo::completeValue() const
{
    return complete ;
}

void AProgressInfo::setProgress(int value)
{
    barView->setValue(qBound(0, value, complete)) ;
    QCoreApplication::processEvents() ;
}

void AProgressInfo::reportProgress(int value, const QString &line)
{
    logLine(line) ;
    setProgress(value) ;
}

void AProgressInfo::finishProgress(const QString &summary, const QString &buttonText)
{
    logLine(summary) ;
    barView->setValue(complete) ;
    QCoreApplication::processEvents() ;
    play(Query) ;
    running = false ;
    finished = true ;
    buttonView->setAccessibleName(buttonText) ;
    buttonView->setText(buttonText) ;
    buttonView->setFocus() ;
    execFocused(this) ;
}

bool AProgressInfo::isAborted() const
{
    return aborted ;
}

void AProgressInfo::buttonPressed()
{
    if (finished) accept() ;
    else if (running) aborted = true ;
}

void AProgressInfo::on_pushButton_clicked()
{
    buttonPressed() ;
}
