#include "safetextedit.h"
#include <QKeyEvent>
#include <QTextCursor>
#include <QApplication>

#include "../Lib/alertsound.h"

SafeTextEdit::SafeTextEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setAutoHomeEnabled(true) ;
    setMouseEnabled(false) ;
}

void SafeTextEdit::setAutoHomeEnabled(bool state)
{
    autohomeenabled=state ;
}

void SafeTextEdit::setMouseEnabled(bool state)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, !state) ;
}

void SafeTextEdit::keyPressEvent(QKeyEvent *event)
{
    bool allowed=true ;
    bool selected=false ;
    int key = event->key() ;

    QTextCursor cursor = this->textCursor() ;

    if (event->type() == QKeyEvent::KeyPress) {

        allowed = (key==Qt::Key_Left || key==Qt::Key_Right || key==Qt::Key_Up ||
                        key==Qt::Key_Down || key==Qt::Key_Home || key==Qt::Key_End ||
                        key==Qt::Key_Control || key==Qt::Key_Shift || key==Qt::Key_Alt ||
                        key==Qt::Key_PageDown || key==Qt::Key_PageUp) ;

        allowed |= event->matches(QKeySequence::Copy) ;
        allowed |= (key==Qt::Key_Delete && (event->modifiers()&Qt::ShiftModifier)!=0) ;

        // move the cursor down and home or up and home
        enum QTextCursor::MoveMode shifted = QTextCursor::MoveAnchor ;
        if ((event->modifiers()&Qt::ShiftModifier)!=0) shifted=QTextCursor::KeepAnchor ;
        if (autohomeenabled && (key==Qt::Key_Up || key==Qt::Key_Down || key==Qt::Key_PageDown || key==Qt::Key_PageUp)) {
            cursor.movePosition(QTextCursor::StartOfLine, shifted) ;
            this->setTextCursor(cursor) ;
        }

        // If the cursor start and end are not the same, there is a selection
        selected = (cursor.selectionStart() != cursor.selectionEnd()) ;

    }

    if (selected && !allowed) {
        play(Disabled) ;
        cursor.setPosition(cursor.selectionEnd()) ;
        this->setTextCursor(cursor) ;
    }

    QPlainTextEdit::keyPressEvent(event);
}
