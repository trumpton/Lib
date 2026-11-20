#include "search.h"
#include "ui_search.h"
#include "supportfunctions.h"
#include "alertsound.h"

Search::Search(QWidget *parent, const QString &name) :
    QDialog(parent),
    ui(new Ui::Search),
    heading(name)
{
    ui->setupUi(this);
    setWindowTitle(dialogTitle("Search Results")) ;
    ui->listSearchResults->setModel(&strings) ;
    // Enter or a double click chooses the line, as OK does
    connect(ui->listSearchResults, &QAbstractItemView::activated, this, &Search::accept) ;
    clearResults() ;
}

void Search::clearResults()
{
    ui->listSearchResults->reset() ;
    strings.clear() ;
    // The first line names the search.  It is the list's only line until a result is added, and it is not a result.
    QString title = heading.isEmpty() ? QString("Search Results") : QString("Search Results for ") + heading ;
    strings.appendData(title, title, QString()) ;
}

void Search::addString(QString resulttext, QString resulthint)
{
    strings.appendData(resulttext, resulttext, resulthint) ;
    // The first result is the line chosen, not the name line
    if (strings.rowCount()==2) ui->listSearchResults->setCurrentIndex(ui->listSearchResults->model()->index(1, 0)) ;
    dbg("Search::addString: listSearchResults->setFocus()") ;
    ui->listSearchResults->setFocus() ;
}

Search::~Search()
{
    delete ui;
}

// Choosing the name line is an error; the dialog stays open
void Search::accept()
{
    if (ui->listSearchResults->currentIndex().row()<1) {
        play(Error) ;
        return ;
    }
    QDialog::accept() ;
}

QString Search::getSelection()
{
    int index = ui->listSearchResults->currentIndex().row() ;
    if (index>=1) return strings.hintAt(index) ;
    else return QString("") ;
}

QString Search::getFirst()
{
    if (strings.rowCount()>1) return strings.hintAt(1) ;
    else return QString("") ;
}
