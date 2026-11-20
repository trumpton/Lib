#include "search.h"
#include "ui_search.h"
#include "supportfunctions.h"
#include "alertsound.h"
#include "alistview.h"
#include <QVBoxLayout>
#include <QKeyEvent>

Search::Search(QWidget *parent, const QString &line, const QString &task) :
    QDialog(parent),
    ui(new Ui::Search),
    firstLine(line)
{
    ui->setupUi(this);
    setWindowTitle(dialogTitle(task)) ;
    // The list fills the dialog above the buttons, so it grows with the dialog
    QVBoxLayout *layout = new QVBoxLayout(this) ;
    layout->addWidget(ui->listSearchResults, 1) ;
    layout->addWidget(ui->buttonBox) ;
    ui->listSearchResults->setModel(&strings) ;
    // The list takes the focus when the dialog is shown: the buttons come after it
    setFocusProxy(ui->listSearchResults) ;
    // A double click chooses the line, and Enter does (see eventFilter)
    connect(ui->listSearchResults, &QAbstractItemView::activated, this, [this](const QModelIndex &) { lineChosen() ; }) ;
    ui->listSearchResults->installEventFilter(this) ;
    clearResults() ;
}

void Search::clearResults()
{
    ui->listSearchResults->reset() ;
    strings.clear() ;
    // The name line is the first line, and the current one: the user moves down from it to the results
    strings.appendData(firstLine, firstLine, QString()) ;
    ui->listSearchResults->setCurrentIndex(ui->listSearchResults->model()->index(0, 0)) ;
}

void Search::addString(QString resulttext, QString resulthint)
{
    strings.appendData(resulttext, resulttext, resulthint) ;
    dbg("Search::addString: listSearchResults->setFocus()") ;
    ui->listSearchResults->setFocus() ;
}

Search::~Search()
{
    delete ui;
}

// Enter on the list chooses the line, and the key is taken here: otherwise the dialog's default button (OK) would act on it too
bool Search::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==ui->listSearchResults && event->type()==QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event) ;
        if (key->key()==Qt::Key_Return || key->key()==Qt::Key_Enter) {
            lineChosen() ;
            return true ;
        }
    }
    return QDialog::eventFilter(watched, event) ;
}

AListView *Search::resultList()
{
    return ui->listSearchResults ;
}

void Search::lineChosen()
{
    accept() ;
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

QString Search::getSelectionText()
{
    int index = ui->listSearchResults->currentIndex().row() ;
    if (index>=1) return ui->listSearchResults->model()->index(index, 0).data(Qt::DisplayRole).toString() ;
    else return QString("") ;
}

QString Search::getFirst()
{
    if (strings.rowCount()>1) return strings.hintAt(1) ;
    else return QString("") ;
}
