#ifndef WARNINGFORM_H
#define WARNINGFORM_H

#include <QDialog>

namespace Ui {
class WarningForm;
}

class WarningForm : public QDialog
{
    Q_OBJECT

public:
    explicit WarningForm(QWidget *parent = 0);
    ~WarningForm();

private:
    Ui::WarningForm *ui;
};

#endif // WARNINGFORM_H
