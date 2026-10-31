#include "safelineedit.h"
#include <QKeyEvent>
#include <QCursor>
#include <QApplication>
#include "../Lib/alertsound.h"

SafeLineEdit::SafeLineEdit(QWidget *parent) :
    QLineEdit(parent),
    keepcursor(false)
{
}

void SafeLineEdit::preloadText(const QString &text)
{
    setText(text) ;
    deselect() ;
    end(false) ;

    // When the field has the focus already, the cursor is where it is set.  Otherwise it is kept at the end on the next focus-in
    keepcursor = !hasFocus() ;
}

void SafeLineEdit::focusAtEnd()
{
    keepcursor = true ;
    setFocus() ;

    // Focus-in is not sent when the field has the focus already, so the cursor is moved here
    if (hasFocus()) {
        deselect() ;
        end(false) ;
        keepcursor = false ;
    }
}


void SafeLineEdit::keyPressEvent(QKeyEvent *event)
{
    bool allowed=true ;
    bool selected=false ;
    int key = event->key() ;

    if (event->type() == QKeyEvent::KeyPress) {

        allowed = (key==Qt::Key_Left || key==Qt::Key_Right || key==Qt::Key_Up ||
                        key==Qt::Key_Down || key==Qt::Key_Home || key==Qt::Key_End ||
                        key==Qt::Key_Delete || key==Qt::Key_Backspace ||
                        key==Qt::Key_Control || key==Qt::Key_Shift || key==Qt::Key_Alt) ;

        allowed |= event->matches(QKeySequence::Copy) ;
        allowed |= (key==Qt::Key_Delete && event->matches(QKeySequence::Cut)) ;

        selected = this->hasSelectedText() ;
    }

    if (selected && !allowed) {
        int pos = this->cursorPosition() ;
        this->setCursorPosition(pos) ;
        play(Disabled) ;
    }

    QLineEdit::keyPressEvent(event);
}

void SafeLineEdit::focusInEvent ( QFocusEvent * event)
{
    QLineEdit::focusInEvent(event) ;
    if (keepcursor) {
        deselect() ;
        end(false) ;
        keepcursor = false ;
    } else {
        this->setCursorPosition(0) ;
    }
}

void SafeLineEdit::focusOutEvent ( QFocusEvent * event)
{
    // The cursor goes to the start on a later focus-in, unless it was set to the end again
    keepcursor = false ;
    QLineEdit::focusOutEvent(event) ;
}
