#include "warningyesno.h"
#include "ui_warningyesno.h"

#include <QPushButton>

WarningYesNo::WarningYesNo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningYesNo)
{
    ui->setupUi(this);
    QStyle *s = QApplication::style();
    int sz = s->pixelMetric(QStyle::PM_MessageBoxIconSize) ;
    QIcon i = s->standardIcon(QStyle::SP_MessageBoxQuestion) ;
    ui->icon->setPixmap(i.pixmap(sz,sz)) ;
    ui->textEditWarning->viewport()->setAutoFillBackground( false );
    ui->textEditWarning->setFocus() ;
    yes = QChar('y') ;
    no = QChar('n') ;
}

WarningYesNo::~WarningYesNo()
{
    delete ui;
}

// Handle Y/N commands
void WarningYesNo::keyPressEvent(QKeyEvent *event)
{
    QChar key = QChar(event->key()) ;

    if (key.toLower()==yes) accept() ;
    else if (key.toLower()==no) reject() ;
    else QDialog::keyPressEvent(event) ;
}

void WarningYesNo::setText(QString text)
{
    ui->textEditWarning->setText(text) ;
}

void WarningYesNo::setButtons(QString yestext, QChar yeskey, QString notext, QChar nokey)
{
    ui->buttonBox->button(QDialogButtonBox::Yes)->setText(yestext) ;
    ui->buttonBox->button(QDialogButtonBox::No)->setText(notext) ;
    yes = yeskey.toLower() ;
    no = nokey.toLower() ;
}

