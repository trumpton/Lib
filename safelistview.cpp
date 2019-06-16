#include "safelistview.h"
#include "supportfunctions.h"

SafeListView::SafeListView(QWidget *parent) :
    QListView(parent)
{
}

void SafeListView::setCurrentIndex(const QModelIndex &idx)
{
    curridx = idx ;
    if (hasFocus()) {
        QListView::setCurrentIndex(curridx) ;
        selectionModel()->select(curridx, QItemSelectionModel::Select);
        dbg(QString("hasfocus, curridx.row=%1").arg(curridx.row())) ;
    } else {
        dbg("does not have focus") ;
    }
}

void SafeListView::focusInEvent(QFocusEvent * e)
{    
    QListView::focusInEvent(e) ;

    if (curridx.isValid()) {
        QListView::setCurrentIndex(curridx) ;
        selectionModel()->select(curridx, QItemSelectionModel::Select);
        emit gainedFocus(curridx.row()) ;
        dbg(QString("gained focus, curridx.row=%1").arg(curridx.row())) ;
    } else {
        emit gainedFocus(-1) ;
        dbg(QString("gained focus -1")) ;
    }
}

void SafeListView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    curridx = index ;
    dbg("Changed index position") ;
    QListView::scrollTo(index, hint) ;
}
