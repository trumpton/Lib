#include "itemselect.h"
#include "ui_itemselect.h"

ItemSelect::ItemSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemSelect)
{
    ui->setupUi(this);
}

ItemSelect::~ItemSelect()
{
    delete ui;
}

int ItemSelect::exec()
{
    ui->inputLineEdit->setFocus() ;
    return QDialog::exec() ;
}

void ItemSelect::setTitle(QString title, QString prompt)
{
    this->setWindowTitle(title) ;
    ui->inputLineEdit->setAccessibleName(prompt) ;
    ui->label->setText(prompt) ;
}

void ItemSelect::setIndex(int i)
{
    ui->inputLineEdit->setCurrentIndex(i) ;
}

void ItemSelect::selectData(QString data)
{
    ui->inputLineEdit->selectData(data) ;
}

void ItemSelect::clear()
{
    ui->inputLineEdit->clear() ;
}

void ItemSelect::addEntry(QString entry, QString data, int index)
{
    ui->inputLineEdit->addItem(entry, data, index) ;
}

int ItemSelect::index()
{
    return ui->inputLineEdit->currentIndex() ;
}

int ItemSelect::size()
{
    return ui->inputLineEdit->size() ;
}

const QString& ItemSelect::getSelection()
{
    return ui->inputLineEdit->currentText() ;
}

const QString& ItemSelect::getData()
{
    return ui->inputLineEdit->currentData() ;
}
