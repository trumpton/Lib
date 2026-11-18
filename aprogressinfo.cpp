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
    updateStatusReport = ui->updateStatusReport ;
    progressBar = ui->progressBar ;
    pushButton = ui->pushButton ;
    complete = 100 ;
    running = false ;
    aborted = false ;
    finished = false ;
    progressBar->setRange(0, complete) ;
}

AProgressInfo::~AProgressInfo()
{
    delete ui ;
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
    progressBar->setRange(0, complete) ;
    progressBar->setValue(0) ;
    setModal(true) ;
    pushButton->setAccessibleName(runningText) ;
    pushButton->setText(runningText) ;
    show() ;
    pushButton->setFocus() ;
}

void AProgressInfo::setCompleteValue(int value)
{
    complete = value ;
    progressBar->setRange(0, complete) ;
}

int AProgressInfo::completeValue() const
{
    return complete ;
}

void AProgressInfo::setProgress(int value)
{
    progressBar->setValue(qBound(0, value, complete)) ;
    QCoreApplication::processEvents() ;
}

void AProgressInfo::reportProgress(int value, const QString &line)
{
    if (!line.isEmpty()) updateStatusReport->appendPlainText(line) ;
    setProgress(value) ;
}

void AProgressInfo::finishProgress(const QString &summary, const QString &buttonText)
{
    if (!summary.isEmpty()) updateStatusReport->appendPlainText(summary) ;
    progressBar->setValue(complete) ;
    QCoreApplication::processEvents() ;
    play(Query) ;
    running = false ;
    finished = true ;
    pushButton->setAccessibleName(buttonText) ;
    pushButton->setText(buttonText) ;
    pushButton->setFocus() ;
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
