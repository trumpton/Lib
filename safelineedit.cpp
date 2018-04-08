#include "safelineedit.h"
#include <QKeyEvent>
#include <QCursor>
#include <QApplication>
#include "../Lib/alertsound.h"

SafeLineEdit::SafeLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
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
    this->setCursorPosition(0) ;
}
