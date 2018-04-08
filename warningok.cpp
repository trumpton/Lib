#include "warningok.h"
#include "ui_warningok.h"

WarningOk::WarningOk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningOk)
{
    ui->setupUi(this);
    QStyle *s = QApplication::style();
    int sz = s->pixelMetric(QStyle::PM_MessageBoxIconSize) ;
    QIcon i = s->standardIcon(QStyle::SP_MessageBoxWarning) ;
    ui->icon->setPixmap(i.pixmap(sz,sz)) ;
    ui->textEditWarning->viewport()->setAutoFillBackground( false );
    ui->textEditWarning->setFocus() ;
}

WarningOk::~WarningOk()
{
    delete ui;
}

void WarningOk::setText(QString text)
{
    ui->textEditWarning->setText(text) ;
}
