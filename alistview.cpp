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
        selectionModel()->select(curridx, QItemSelectionModel::Select);
        dbg(QString("hasfocus, curridx.row=%1").arg(curridx.row())) ;
        emit selectionChanged(curridx);
    } else {
        dbg(QString("does not have focus, caching curridx.row=%1").arg(curridx.row())) ;
    }
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
        selectionModel()->select(curridx, QItemSelectionModel::Select);
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
