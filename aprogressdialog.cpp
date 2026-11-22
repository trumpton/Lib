#include "aprogressdialog.h"
#include "yieldtoevents.h"

#include <QPushButton>

void AProgressDialog::showProgress()
{
    show() ;
    // The cancel button is the dialog's only control that takes the focus
    QPushButton *cancel = findChild<QPushButton *>() ;
    if (cancel) cancel->setFocus() ;
}

void AProgressDialog::setValue(int value)
{
    QProgressDialog::setValue(value) ;
    yieldIfStepDue(yieldClock) ;
}
