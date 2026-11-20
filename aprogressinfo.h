#ifndef APROGRESSINFO_H
#define APROGRESSINFO_H

#include <QDialog>
#include <QString>

class ATextEdit ;
class AProgressBar ;
class APushButton ;
namespace Ui {
class AProgressInfo ;
}

// AProgressInfo: a progress window for a long operation, with a log of what has been done, a bar and one button.  The
// widgets are private: a subclass uses logLine, setProgress, setPartNames, startProgress and finishProgress.  While the operation runs the button is Abort: pressing it sets isAborted(), which
// the operation checks as it goes.  finishProgress() ends the report: it writes the summary, sets the bar to 100%, plays the
// completion sound, and changes the button to its finished text (OK by default).  Then the button closes the window.
//
// Construct it with no Qt parent: new AProgressInfo(nullptr).  startProgress() shows the window with show(), as it stays open
// while the user works elsewhere (documents/tickets/parented-windows.md); finishProgress() waits for the user with execFocused().
class AProgressInfo : public QDialog
{
    Q_OBJECT

public:
    explicit AProgressInfo(QWidget *parent = nullptr) ;
    ~AProgressInfo() ;

    // Shows the window for a task, with the title dialogTitle(task).  The bar is at 0, and the button says runningText.
    void startProgress(const QString &task, const QString &runningText = QString("Abort")) ;

    // The bar's value for 100%.  It is 100 unless set here.
    void setCompleteValue(int value) ;
    int completeValue() const ;

    // Sets the bar, and lets the window process events so that it keeps responding.  The value is kept between 0 and the 100% value.
    virtual void setProgress(int value) ;

    // Writes a line to the log (when it is not empty), and sets the bar.
    void reportProgress(int value, const QString &line) ;

    // Ends the report: the summary is written to the log (when it is not empty), the bar goes to 100%, the completion sound
    // plays, and the button says buttonText.  The user closes the window with the button.
    void finishProgress(const QString &summary = QString(), const QString &buttonText = QString("OK")) ;

    // True once the button has been pressed while the operation was running.
    virtual bool isAborted() const ;

    // The accessible names of the log and of the bar, for a window whose parts have their own (for example "Update Progress").
    // Set them before startProgress(), as the Google sync and the tidy do.
    void setPartNames(const QString &logName, const QString &barName) ;

protected:
    // Writes a line to the log.  The log is read by the screen reader as it is written, so only what the user needs is written.
    void logLine(const QString &line) ;

    // The button press: Abort while the operation runs, and closes the window once it has finished.  A subclass that keeps its
    // own state overrides this.
    virtual void buttonPressed() ;

private slots:
    void on_pushButton_clicked() ;

private:
    // The widgets belong to the dialog: a subclass uses the functions above, not the widgets
    ATextEdit *logView ;
    AProgressBar *barView ;
    APushButton *buttonView ;
    Ui::AProgressInfo *ui ;
    int complete ;
    bool running ;
    bool aborted ;
    bool finished ;
} ;

#endif // APROGRESSINFO_H
