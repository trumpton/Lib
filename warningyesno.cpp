#include "warningyesno.h"
#include "ui_warningyesno.h"

#include <QPushButton>
#include <QStyle>

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
    // Names for the buttons, so that a screen test can find them (the message box is textEditWarning, from the form)
    ui->buttonBox->button(QDialogButtonBox::Yes)->setObjectName("warningYes") ;
    ui->buttonBox->button(QDialogButtonBox::No)->setObjectName("warningNo") ;
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
    int k = event->key() ;
    QString typed = event->text().toLower() ;
    if (k==Qt::Key_Enter || k==Qt::Key_Return) accept() ;
    else if (k==Qt::Key_Escape) reject() ;
    else if (!yes.isNull() && typed==QString(yes)) accept() ;
    else if (!no.isNull() && typed==QString(no)) reject() ;
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

void WarningYesNo::setSpokenText(QString text)
{
    ui->textEditWarning->setAccessibleName(text) ;
}

void WarningYesNo::setFocusOnYes()
{
    ui->buttonBox->button(QDialogButtonBox::Yes)->setFocus() ;
}

void WarningYesNo::setButtonDescriptions(QString yesdescription, QString nodescription)
{
    ui->buttonBox->button(QDialogButtonBox::Yes)->setAccessibleDescription(yesdescription) ;
    ui->buttonBox->button(QDialogButtonBox::No)->setAccessibleDescription(nodescription) ;
}

void WarningYesNo::setCheckBox(QString text, bool checked)
{
    ui->checkBox->setText(text) ;
    ui->checkBox->setChecked(checked) ;
    ui->checkBox->setVisible(true) ;
}

bool WarningYesNo::isChecked()
{
    return ui->checkBox->isChecked() ;
}

