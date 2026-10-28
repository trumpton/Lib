#include "warningchoice.h"

#include <QCommandLinkButton>
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
    textWarning->viewport()->setAutoFillBackground(false) ;
    layout->addWidget(textWarning) ;

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
    QCommandLinkButton *button = new QCommandLinkButton(label, description, this) ;
    button->setAccessibleName(label) ;
    button->setAccessibleDescription(QString("Press %1 to choose").arg(key.toLower())) ;
    connect(button, &QAbstractButton::clicked, this, [this, index]() { chooseIndex(index) ; }) ;

    // The choices go above the stretch at the foot of the layout
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(this->layout()) ;
    layout->insertWidget(layout->count()-1, button) ;

    buttons.append(button) ;
    keys.append(key.toLower()) ;
    return index ;
}

void WarningChoice::setDefaultChoice(int index)
{
    defaultindex = (index>=0 && index<buttons.size()) ? index : -1 ;
}

int WarningChoice::choose()
{
    chosen = -1 ;
    if (!buttons.isEmpty()) {
        QCommandLinkButton *focus = buttons.at(defaultindex>=0 ? defaultindex : 0) ;
        focus->setFocus() ;
    }
    if (exec()==QDialog::Accepted) return chosen ;
    return -1 ;
}

void WarningChoice::chooseIndex(int index)
{
    chosen = index ;
    accept() ;
}

// Handle the letter keys, and Enter for the default choice
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
