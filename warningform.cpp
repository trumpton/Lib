#include "warningform.h"
#include "ui_warningform.h"

WarningForm::WarningForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningForm)
{
    ui->setupUi(this);
    QStyle *s = QApplication::style();
    int sz = s->pixelMetric(QStyle::PM_MessageBoxIconSize) ;
    QIcon i = s->standardIcon(QStyle::SP_MessageBoxWarning) ;
    ui->icon->setPixmap(i.pixmap(sz,sz)) ;
}

WarningForm::~WarningForm()
{
    delete ui;
}
