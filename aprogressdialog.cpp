#include "aprogressdialog.h"
#include "dialogfocus.h"

#include <QPushButton>

// Each step lets the window handle events for this long (see AProgressInfo, which does the same)
static const int PROGRESS_YIELD_MS = 50 ;

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
    yieldToEvents(PROGRESS_YIELD_MS) ;
}
