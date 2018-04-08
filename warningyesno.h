#ifndef WARNINGYESNO_H
#define WARNINGYESNO_H

#include <QDialog>
#include <QKeyEvent>
#include <QChar>

namespace Ui {
class WarningYesNo;
}

class WarningYesNo : public QDialog
{
    Q_OBJECT

public:
    explicit WarningYesNo(QWidget *parent = 0);
    ~WarningYesNo();
    void setText(QString text) ;
    void setButtons(QString yestext, QChar yeskey, QString notext, QChar nokey) ;

private:
    QChar yes, no ;
    Ui::WarningYesNo *ui;
    void keyPressEvent(QKeyEvent *event) ;
};

#endif // WARNINGYESNO_H
