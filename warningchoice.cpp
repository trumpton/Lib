#include "warningchoice.h"

#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

WarningChoice::WarningChoice(QWidget *parent) :
    QDialog(parent),
    defaultindex(-1),
    chosen(-1)
{
    QVBoxLayout *layout = new QVBoxLayout(this) ;

    textWarning = new QTextEdit(this) ;
    textWarning->setReadOnly(true) ;
    textWarning->setTabChangesFocus(true) ;
    textWarning->viewport()->setAutoFillBackground(false) ;
    textWarning->installEventFilter(this) ;
    layout->addWidget(textWarning) ;

    // The choices go above this stretch, at the foot of the layout
    layout->addStretch(0) ;
}

WarningChoice::~WarningChoice()
{
}

void WarningChoice::setText(QString text)
{
    textWarning->setPlainText(text) ;
}

int WarningChoice::addChoice(QString label, QChar key, QString description)
{
    int index = buttons.size() ;
    QChar letter = key.toLower() ;

    QPushButton *button = new QPushButton(QString("%1 (%2)").arg(label).arg(letter), this) ;
    button->setAccessibleDescription(description) ;
    connect(button, &QPushButton::clicked, this, [this, index]() { chooseIndex(index) ; }) ;

    QLabel *details = new QLabel(description, this) ;
    details->setWordWrap(true) ;
    details->setContentsMargins(20, 0, 0, 8) ;

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(this->layout()) ;
    layout->insertWidget(layout->count()-1, button) ;
    layout->insertWidget(layout->count()-1, details) ;

    buttons.append(button) ;
    keys.append(letter) ;
    return index ;
}

void WarningChoice::setDefaultChoice(int index)
{
    defaultindex = (index>=0 && index<buttons.size()) ? index : -1 ;
}

int WarningChoice::choose()
{
    chosen = -1 ;

    // Wide enough that the choices and their descriptions do not wrap, at the font in use
    setMinimumWidth(fontMetrics().horizontalAdvance(QString(90, 'x'))) ;

    if (exec()==QDialog::Accepted) return chosen ;
    return -1 ;
}

void WarningChoice::chooseIndex(int index)
{
    chosen = index ;
    accept() ;
}

// The text box has the focus when the dialog is shown, as in WarningYesNo and WarningOk
void WarningChoice::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event) ;
    textWarning->setFocus() ;
}

// Letters, Enter and Escape choose, whichever has the focus.  Tab and the arrow keys are left to the text box.
bool WarningChoice::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==textWarning && event->type()==QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event) ;
        int k = key->key() ;
        bool letter = !key->text().isEmpty() && key->text().at(0).isLetter() ;
        if (letter || k==Qt::Key_Enter || k==Qt::Key_Return || k==Qt::Key_Escape) {
            keyPressEvent(key) ;
            return true ;
        }
    }
    return QDialog::eventFilter(watched, event) ;
}

void WarningChoice::keyPressEvent(QKeyEvent *event)
{
    int k = event->key() ;
    if ((k==Qt::Key_Enter || k==Qt::Key_Return) && defaultindex>=0) {
        chooseIndex(defaultindex) ;
        return ;
    }
    QString text = event->text() ;
    if (!text.isEmpty()) {
        int index = keys.indexOf(text.at(0).toLower()) ;
        if (index>=0) {
            chooseIndex(index) ;
            return ;
        }
    }
    QDialog::keyPressEvent(event) ;
}
