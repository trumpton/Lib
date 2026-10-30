#ifndef DATEINPUTDIALOG_H
#define DATEINPUTDIALOG_H

#include <QDate>
#include <QString>

class QWidget ;

// Asks for a date.  The prompt is shown above an accessible date field (AccessibleDateTimeEdit), which takes the day, month
// and year changed with the arrow keys or typed as numbers, with OK and Cancel.  Enter accepts and Escape cancels.
//
// Returns the date chosen.  ok is set to true when OK is pressed, and to false when cancelled, when the date returned is
// not valid.  The field starts on defaultdate (today, if defaultdate is not valid).
QDate dateInputDialog(QWidget *parent, QString title, QString prompt, QDate defaultdate, bool *ok) ;

#endif // DATEINPUTDIALOG_H
