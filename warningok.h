#ifndef WARNINGOK_H
#define WARNINGOK_H

#include <QDialog>

namespace Ui {
class WarningOk;
}

class WarningOk : public QDialog
{
    Q_OBJECT

public:
    explicit WarningOk(QWidget *parent = 0);
    ~WarningOk();
    void setText(QString text) ;

private:
    Ui::WarningOk *ui;
};

#endif // WARNINGOK_H
