#include "safelistview.h"
#include "supportfunctions.h"
#include <QStringListModel>

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
        emit selectionChanged(curridx);
    } else {
        dbg(QString("does not have focus, caching curridx.row=%1").arg(curridx.row())) ;
    }
}

void SafeListView::focusInEvent(QFocusEvent * e)
{    
    QListView::focusInEvent(e) ;

    if (curridx.isValid()) {
        QListView::setCurrentIndex(curridx) ;
        selectionModel()->select(curridx, QItemSelectionModel::Select);
        dbg(QString("gained focus, curridx.row=%1").arg(curridx.row())) ;
        emit selectionChanged(curridx) ;
    }
}

void SafeListView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    curridx = index ;
    dbg(QString("Changed Index Position, curridx.row=%1").arg(curridx.row())) ;
    QListView::scrollTo(index, hint) ;
    emit selectionChanged(index) ;
}
