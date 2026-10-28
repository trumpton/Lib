#ifndef WARNINGCHOICE_H
#define WARNINGCHOICE_H

#include <QDialog>
#include <QKeyEvent>
#include <QChar>
#include <QList>

class QTextEdit ;
class QCommandLinkButton ;

// A dialog that asks the user to choose one of several actions.  Each choice has a button, its own letter key, and an
// optional description, which a screen reader reads with it.  The letter key chooses at once, Enter chooses the default,
// and Escape cancels.  The choices stack one below the other and their text wraps, so the dialog works at large fonts.
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
    QList<QCommandLinkButton *> buttons ;
    QList<QChar> keys ;
    int defaultindex ;
    int chosen ;

    void chooseIndex(int index) ;
    void keyPressEvent(QKeyEvent *event) ;
};

#endif // WARNINGCHOICE_H
