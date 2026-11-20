#ifndef ALISTVIEW_H
#define ALISTVIEW_H

#include <QListView>

class AListView : public QListView
{
    Q_OBJECT

private:
    QModelIndex curridx ;

public:
    explicit AListView(QWidget *parent = 0);
    void focusInEvent(QFocusEvent * e) ;
    void setCurrentIndex(const QModelIndex &idx) ;
    void selectCurrent() ;   // marks the current entry as selected, unless the list has no selection
    void layoutNow() ;     // Lays out the entries now, so that the scroll bar and the positions of the entries are current
    void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint = EnsureVisible) ;

signals:
    void selectionChanged(const QModelIndex &index) ;

public slots:

};

#endif // ALISTVIEW_H
