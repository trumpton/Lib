#ifndef ASEARCHLINEEDIT_H
#define ASEARCHLINEEDIT_H

#include <QLineEdit>

// ASearchLineEdit: the text box of the Find and Global Find dialogs.  The text of the last search is shown selected, with the
// cursor at its end (searchInputDialog selects it), and typing replaces the selection, as in any text box.  Unlike ALineEdit
// it does not refuse a key while text is selected, and does not move the cursor to the start on focus.  ALineEdit, and the
// other dialogs that use it, are not changed.
class ASearchLineEdit : public QLineEdit
{
public:
    using QLineEdit::QLineEdit ;
} ;

#endif // ASEARCHLINEEDIT_H
