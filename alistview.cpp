#include "alistview.h"
#include "supportfunctions.h"
#include <QStringListModel>

AListView::AListView(QWidget *parent) :
    QListView(parent)
{
}

void AListView::setCurrentIndex(const QModelIndex &idx)
{
    curridx = idx ;
    if (hasFocus()) {
        QListView::setCurrentIndex(curridx) ;
        selectCurrent() ;
        dbg(QString("hasfocus, curridx.row=%1").arg(curridx.row())) ;
        emit selectionChanged(curridx);
    } else {
        dbg(QString("does not have focus, caching curridx.row=%1").arg(curridx.row())) ;
    }
}

// The current entry is marked as selected, unless the list has no selection (a list of entries to move through and choose, as in
// the search results and the transaction edit).  The selection model would otherwise mark it whatever the mode.
void AListView::selectCurrent()
{
    if (selectionMode()==NoSelection || !selectionModel()) return ;
    selectionModel()->select(curridx, QItemSelectionModel::Select);
}

void AListView::layoutNow()
{
    // The entries are laid out after the model has changed, not at once, so this does it when they are needed now
    executeDelayedItemsLayout() ;
}

void AListView::focusInEvent(QFocusEvent * e)
{    
    QListView::focusInEvent(e) ;

    if (curridx.isValid()) {
        QListView::setCurrentIndex(curridx) ;
        selectCurrent() ;
        dbg(QString("gained focus, curridx.row=%1").arg(curridx.row())) ;
        emit selectionChanged(curridx) ;
    }
}

void AListView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    // The view scrolls to other entries as it lays itself out, and to show a row, so only the current entry is kept here.  A
    // scroll to another entry does not change which entry is current.
    QListView::scrollTo(index, hint) ;
    if (selectionModel() && index==selectionModel()->currentIndex()) {
        curridx = index ;
        dbg(QString("Changed Index Position, curridx.row=%1").arg(curridx.row())) ;
        emit selectionChanged(index) ;
    }
}
