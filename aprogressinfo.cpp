#include "aprogressinfo.h"
#include "ui_aprogressinfo.h"
#include "aprogressbar.h"
#include "atextedit.h"
#include "apushbutton.h"
#include "alertsound.h"
#include "dialogfocus.h"
#include "yieldtoevents.h"
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
    progressoffset = 0 ;
    running = false ;
    aborted = false ;
    finished = false ;
    barView->setRange(0, complete) ;

    // The log does not take the focus when the window is shown.  Qt gives the focus to the first focusable control when the
    // window is activated, which would be the log.  NVDA would then read the log's last line before the button's name, so the
    // prompt would start with the end of a log line.  The log is made focusable when the operation finishes (finishProgress).
    // See documents/gotchas.md 6.15.
    logView->setFocusPolicy(Qt::NoFocus) ;
}

AProgressInfo::~AProgressInfo()
{
    delete ui ;
}

void AProgressInfo::logLine(const QString &line)
{
    if (line.isEmpty()) return ;
    logView->appendPlainText(line) ;
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
    // Shown without activation, then the button focused: the order the progress tone was last heard in (aa8811a).
    // Focusing before the window is shown is candidate 8 in the progress-tone ticket.
    show() ;
    // The button has the focus, and is the default button, for the whole operation.  The log stays out of the focus, so the
    // screen reader does not read its text as it is written: the listener hears the progress bar and the button's name.
    buttonView->setDefault(true) ;
    buttonView->setFocus() ;
    // The window and the focus settle before any text is written, so the screen reader has the button when the text arrives
    yieldToEvents(PROGRESS_SETTLE_MS) ;
    traceFocus("progress shown: " + title) ;
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

void AProgressInfo::setProgressOffset(int offset)
{
    progressoffset = offset ;
}

void AProgressInfo::setProgress(int value)
{
    int bar = qBound(0, value + progressoffset, complete) ;
    barView->setValue(bar) ;
    traceFocus(QString("progress bar %1").arg(bar)) ;
    yieldToEvents(PROGRESS_YIELD_MS) ;
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
    // The log can be reached with Tab once the operation has finished, so its text can be read (it was kept out of the focus during it)
    logView->setFocusPolicy(Qt::StrongFocus) ;
    traceFocus("progress finished, waiting for OK") ;
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
