#ifndef SAFELINEEDIT_H
#define SAFELINEEDIT_H

#include <QLineEdit>

class SafeLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SafeLineEdit(QWidget *parent = 0);

private:
    void keyPressEvent(QKeyEvent *event) ;
    void focusInEvent ( QFocusEvent *event) ;

signals:

public slots:

};

#endif // SAFELINEEDIT_H
