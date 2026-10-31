#ifndef SAFELINEEDIT_H
#define SAFELINEEDIT_H

#include <QLineEdit>

class SafeLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SafeLineEdit(QWidget *parent = 0);

    // Sets the text, with the cursor at its end and nothing selected.  The cursor stays at the end when the field next gets the
    // focus, instead of moving to the start.
    void preloadText(const QString &text) ;

    // Gives the field the focus, with the cursor at its end
    void focusAtEnd() ;

private:
    bool keepcursor ;       // The next focus-in keeps the cursor where it is, not at the start.  Set by preloadText and focusAtEnd

    void keyPressEvent(QKeyEvent *event) ;
    void focusInEvent ( QFocusEvent *event) ;
    void focusOutEvent ( QFocusEvent *event) ;

signals:

public slots:

};

#endif // SAFELINEEDIT_H
