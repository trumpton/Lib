#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QTime>
#include <QVBoxLayout>
#include "dateinputdialog.h"
#include "accessibledatetimeedit.h"
#include "alertsound.h"

QDate dateInputDialog(QWidget *parent, QString title, QString prompt, QDate defaultdate, bool *ok)
{
    play(Query) ;

    if (!defaultdate.isValid()) defaultdate = QDate::currentDate() ;

    QDialog dialog(parent) ;
    dialog.setWindowTitle(title) ;

    QVBoxLayout *layout = new QVBoxLayout(&dialog) ;

    // The prompt is the field's label, so a screen reader reads it with the field
    QLabel *label = new QLabel(prompt, &dialog) ;
    label->setWordWrap(true) ;
    layout->addWidget(label) ;

    // Day, month name and year.  The time is noon, so the date is the same in every zone.
    AccessibleDateTimeEdit *edit = new AccessibleDateTimeEdit(&dialog) ;
    edit->setFormat("ddd ", "dd/MMM/yyyy") ;
    QDateTime when(defaultdate, QTime(12, 0)) ;
    edit->setDateTime(when) ;
    label->setBuddy(edit) ;
    edit->setAccessibleName(prompt) ;
    edit->setAccessibleDescription("Use the Up and Down keys to change the day, month or year, or type a number") ;
    layout->addWidget(edit) ;

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog) ;
    layout->addWidget(buttons) ;

    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept) ;
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject) ;
    QObject::connect(edit, &QLineEdit::returnPressed, &dialog, &QDialog::accept) ;

    edit->setFocus() ;
    bool accepted = (dialog.exec()==QDialog::Accepted) ;
    if (ok) *ok = accepted ;

    return accepted ? edit->getDateTime().date() : QDate() ;
}
