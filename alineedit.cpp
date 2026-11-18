#include "alineedit.h"
#include <QKeyEvent>
#include <QCursor>
#include <QApplication>
#include <QPainter>
#include <QStyleOptionFrame>
#include "../Lib/alertsound.h"

ALineEdit::ALineEdit(QWidget *parent) :
    QLineEdit(parent),
    keepcursor(false),
    hintTaken(false)
{
}

void ALineEdit::showEvent(QShowEvent *event)
{
    if (!hintTaken) {
        hintTaken = true ;
        hintText = placeholderText() ;
        QLineEdit::setPlaceholderText(QString()) ;
    }
    QLineEdit::showEvent(event) ;
}

void ALineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event) ;
    if (text().isEmpty() && !hintText.isEmpty()) {
        QStyleOptionFrame option ;
        initStyleOption(&option) ;
        QRect r = style()->subElementRect(QStyle::SE_LineEditContents, &option, this) ;
        QPainter painter(this) ;
        painter.setPen(palette().color(QPalette::PlaceholderText)) ;
        painter.drawText(r.adjusted(3, 0, -3, 0), Qt::AlignLeft | Qt::AlignVCenter, hintText) ;
    }
}

void ALineEdit::preloadText(const QString &text)
{
    setText(text) ;
    deselect() ;
    end(false) ;

    // When the field has the focus already, the cursor is where it is set.  Otherwise it is kept at the end on the next focus-in
    keepcursor = !hasFocus() ;
}

void ALineEdit::focusAtEnd()
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


void ALineEdit::keyPressEvent(QKeyEvent *event)
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

void ALineEdit::focusInEvent ( QFocusEvent * event)
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

void ALineEdit::focusOutEvent ( QFocusEvent * event)
{
    // The cursor goes to the start on a later focus-in, unless it was set to the end again
    keepcursor = false ;
    QLineEdit::focusOutEvent(event) ;
}
