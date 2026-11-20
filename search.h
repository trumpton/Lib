#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QStringList>
#include "accessiblestringlistmodel.h"

namespace Ui {
class Search;
}

class AListView ;

// Search: the list dialog shared by every search and by the transaction edit.  Its first line is the list's name, such as
// "Global Find Contact Manager Results", and it is the line the list starts on.  That line is not a result: choosing it plays the
// error sound.  The results are the lines after it.
class Search : public QDialog
{
    Q_OBJECT

public:
    // firstLine is the name line (for example resultsName("Global Find")).  task is the dialog's task, for its title
    // ("Contact Manager - Search Results").
    explicit Search(QWidget *parent = 0, const QString &firstLine = QString("Search Results"), const QString &task = QString("Search Results"));
    ~Search();
    void clearResults() ;
    void addString(QString resulttext, QString resulthint) ;
    QString getSelection() ;      // the hint of the chosen result, or blank
    QString getSelectionText() ;  // the text of the chosen result, or blank
    QString getFirst() ;          // the hint of the first result, or blank

public slots:
    void accept() override ;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override ;

    // Enter, or a double click, on a line.  The default accepts the dialog (the name line is an error).
    virtual void lineChosen() ;

    // The list, for a dialog that changes it or moves in it
    AListView *resultList() ;

private:
    Ui::Search *ui;
    AccessibleStringListModel strings ;
    QString firstLine ;
};

#endif // SEARCH_H
