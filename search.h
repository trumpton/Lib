#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QStringList>
#include "accessiblestringlistmodel.h"

namespace Ui {
class Search;
}

// Search: the results list, shared by every search in ContactManager and EasyNotepad.  Its first line names the search
// ("Search Results for Email / SMS To Non-Clients").  That line is not a result: choosing it plays the error sound.
// The results are the lines after it.
class Search : public QDialog
{
    Q_OBJECT
    
public:
    explicit Search(QWidget *parent = 0, const QString &heading = QString());
    ~Search();
    void clearResults() ;
    void addString(QString resulttext, QString resulthint) ;
    QString getSelection() ;
    QString getFirst() ;

public slots:
    void accept() override ;

private:
    Ui::Search *ui;
    AccessibleStringListModel strings ;
    QString heading ;

};

#endif // SEARCH_H
