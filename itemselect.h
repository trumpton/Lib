#ifndef ITEMSELECT_H
#define ITEMSELECT_H

#include <QDialog>

namespace Ui {
class ItemSelect;
}

class ItemSelect : public QDialog
{
    Q_OBJECT

public:
    explicit ItemSelect(QWidget *parent = 0);
    ~ItemSelect();

    void setTitle(QString title, QString prompt) ;
    void clear() ;
    int size() ;

    void addEntry(QString entry, QString data, int index=-1) ;

    void setIndex(int i) ;
    int index() ;

    void selectData(QString data) ;
    const QString& getSelection() ;
    const QString& getData() ;

    int exec() ;


private:
    Ui::ItemSelect *ui;
};

#endif // ITEMSELECT_H
