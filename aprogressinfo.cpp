#include "aprogressinfo.h"
#include "ui_aprogressinfo.h"
#include "aprogressbar.h"
#include "atextedit.h"
#include "apushbutton.h"
#include "alertsound.h"
#include "dialogfocus.h"
#include "supportfunctions.h"

#include <QAccessible>
#include <QAccessibleValueInterface>
#include <QApplication>
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

    // With the accessibility trace on, each change of the bar is traced with the value its accessibility interface reports:
    // a change the screen reader is not sent appears here with no accessible value, or with the old one
    connect(barView, &QProgressBar::valueChanged, this, [this](int value) {
        if (!accessibleTraceIsOn()) return ;
        QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(barView) ;
        QString accessible = (iface && iface->valueInterface()) ? iface->valueInterface()->currentValue().toString() : QString("none") ;
        accessibleTrace(QString("progress bar value %1 of %2, accessible value %3, visible %4, focus %5")
                        .arg(value).arg(barView->maximum()).arg(accessible).arg(barView->isVisible())
                        .arg(QApplication::focusWidget() ? QApplication::focusWidget()->objectName() : QString("none"))) ;
    }) ;
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
    // Every progress window starts with the same sound, as the Google sync always has
    play(Ok) ;
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
    // Shown active, with the button focused, so that the screen reader follows the window while the bar moves
    buttonView->setFocus() ;
    showFocused(this) ;
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
