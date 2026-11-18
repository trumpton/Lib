#ifndef ATEXTEDIT_H
#define ATEXTEDIT_H

#include <QPlainTextEdit>

class ATextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ATextEdit(QWidget *parent = 0);
    void setAutoHomeEnabled(bool state) ;
    void setMouseEnabled(bool state) ;


private:
    bool autohomeenabled, mouseenabled ;
    void keyPressEvent(QKeyEvent *event) ;

signals:

public slots:

};

#endif // ATEXTEDIT_H
