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
    // A small step is not given time: the window yields when the value has moved by more than 1% since the last yield
    if (progressStepIsLarge(lastYieldValue, value, maximum() - minimum())) {
        lastYieldValue = value ;
        yieldToEvents(PROGRESS_YIELD_MS) ;
    }
}
