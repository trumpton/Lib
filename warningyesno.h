#ifndef WARNINGYESNO_H
#define WARNINGYESNO_H

#include <QDialog>
#include <QKeyEvent>
#include <QChar>

namespace Ui {
class WarningYesNo;
}

class WarningYesNo : public QDialog
{
    Q_OBJECT

public:
    explicit WarningYesNo(QWidget *parent = 0);
    ~WarningYesNo();
    void setText(QString text) ;
    void setButtons(QString yestext, QChar yeskey, QString notext, QChar nokey) ;

    // The text the screen reader says for the message (the message on the screen may be longer)
    void setSpokenText(QString text) ;
    // The focus starts on the Yes button, so that Space presses it; Enter still accepts and Escape still cancels.  By default
    // the focus is on the message text.
    void setFocusOnYes() ;

    // What the screen reader says for each button.  By default, the button's own text.  An empty string restores that.
    void setButtonSpokenText(QString yestext, QString notext) ;
    // A hint for each button, which the screen reader reads after it (for example what the button does).  Empty: none.
    void setButtonHint(QString yeshint, QString nohint) ;
    // When on, the screen reader says the dialog's text for each button, instead of the button's text (off by default).
    void setSpeakDialogText(bool on) ;

    // Adds a checkbox to the left of the buttons, with its text and its starting state.  Without this call
    // there is no checkbox.
    void setCheckBox(QString text, bool checked) ;
    bool isChecked() ;

private:
    QChar yes, no ;
    QString yespoken, nopoken ;
    bool speakDialogText = false ;
    void applyButtonNames() ;
    Ui::WarningYesNo *ui;
    void keyPressEvent(QKeyEvent *event) ;
};

#endif // WARNINGYESNO_H
