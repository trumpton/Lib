#ifndef SAFELISTVIEW_H
#define SAFELISTVIEW_H

#include <QListView>

class SafeListView : public QListView
{
    Q_OBJECT
public:
    explicit SafeListView(QWidget *parent = 0);
    void focusInEvent(QFocusEvent * e) ;

signals:
    void gainedFocus(QString str) ;

public slots:

};

#endif // SAFELISTVIEW_H
