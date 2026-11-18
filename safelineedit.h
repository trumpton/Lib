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

    // The hint text (the placeholder) is drawn here, not by Qt: Qt reports the placeholder to the screen reader as the field's
    // description, so it was read with every empty field.  The hint is taken from the placeholder when the field is first shown.
    QString hintText ;
    bool hintTaken ;

    void showEvent(QShowEvent *event) override ;
    void paintEvent(QPaintEvent *event) override ;

    void focusInEvent ( QFocusEvent *event) ;
    void focusOutEvent ( QFocusEvent *event) ;

protected:
    // A subclass that keeps its own keys calls this for the rest (its keys are not blocked by the selection)
    void keyPressEvent(QKeyEvent *event) ;

signals:

public slots:

};

#endif // SAFELINEEDIT_H
