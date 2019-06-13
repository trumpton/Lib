#include "safelistview.h"

SafeListView::SafeListView(QWidget *parent) :
    QListView(parent)
{
}

void SafeListView::focusInEvent(QFocusEvent * e)
{    
    QListView::focusInEvent(e) ;
    QModelIndex idx = currentIndex() ;

    if (!idx.isValid() && model()) {
        idx = model()->index(0, 0) ;
    }

    if (idx.isValid()) {
        //setCurrentIndex(idx) ;
        selectionModel()->select(idx, QItemSelectionModel::Select);
        int i = idx.row() ;
        emit gainedFocus("Selected Row " + QString::number(i) + ". ") ;
    }

}
