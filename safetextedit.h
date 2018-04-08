#ifndef SAFETEXTEDIT_H
#define SAFETEXTEDIT_H

#include <QPlainTextEdit>

class SafeTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit SafeTextEdit(QWidget *parent = 0);
    void setAutoHomeEnabled(bool state) ;
    void setMouseEnabled(bool state) ;


private:
    bool autohomeenabled, mouseenabled ;
    void keyPressEvent(QKeyEvent *event) ;

signals:

public slots:

};

#endif // SAFETEXTEDIT_H
