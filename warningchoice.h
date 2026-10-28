#ifndef WARNINGCHOICE_H
#define WARNINGCHOICE_H

#include <QDialog>
#include <QKeyEvent>
#include <QChar>
#include <QList>

class QTextEdit ;
class QPushButton ;

// A dialog that asks the user to choose one of several actions, as WarningYesNo and WarningOk show their text.
// The text is a read-only text box, which has the focus when the dialog opens, so a braille display reads it.  Tab
// moves the focus to the choices.  Each choice is a button, showing its letter key, with its description under it
// (and read with it).  A letter key chooses at once, Enter chooses the default and Escape cancels.
class WarningChoice : public QDialog
{
    Q_OBJECT

public:
    explicit WarningChoice(QWidget *parent = 0);
    ~WarningChoice();

    // The text shown above the choices
    void setText(QString text) ;

    // Adds a choice, and returns its index.  The key is the letter that chooses it (case does not matter).
    int addChoice(QString label, QChar key, QString description = QString()) ;

    // The choice that Enter chooses, as its index
    void setDefaultChoice(int index) ;

    // Shows the dialog, and returns the index of the choice made, or -1 if cancelled
    int choose() ;

private:
    QTextEdit *textWarning ;
    QList<QPushButton *> buttons ;
    QList<QChar> keys ;
    int defaultindex ;
    int chosen ;

    void chooseIndex(int index) ;
    void showEvent(QShowEvent *event) ;
    bool eventFilter(QObject *watched, QEvent *event) ;
    void keyPressEvent(QKeyEvent *event) ;
};

#endif // WARNINGCHOICE_H
