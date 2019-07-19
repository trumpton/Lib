#ifndef SAFELISTVIEW_H
#define SAFELISTVIEW_H

#include <QListView>

class SafeListView : public QListView
{
    Q_OBJECT

private:
    QModelIndex curridx ;

public:
    explicit SafeListView(QWidget *parent = 0);
    void focusInEvent(QFocusEvent * e) ;
    void setCurrentIndex(const QModelIndex &idx) ;
    void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint = EnsureVisible) ;

signals:
    void selectionChanged(const QModelIndex &index) ;

public slots:
    void refreshCurrentIndex() ;

};

#endif // SAFELISTVIEW_H
