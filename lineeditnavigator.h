#ifndef LINEEDITNAVIGATOR_H
#define LINEEDITNAVIGATOR_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QString>
#include <QStringList>

class LineEditNavigator : public QLineEdit
{
    Q_OBJECT
private:
    QStringList entries, arguments ;
    QString selectionText, dataText ;
    int ix ;
    void keyPressEvent(QKeyEvent *event) ;
    void focusInEvent ( QFocusEvent * event) ;
    void update() ;

public:
    explicit LineEditNavigator(QWidget *parent = 0);
    ~LineEditNavigator() ;

    void clear() ;
    int size() ;
    void addItem(QString entry, QString data, int index=-1) ;
    void setCurrentIndex(int i) ;
    int currentIndex() ;
    void selectItem(QString item) ;
    void selectData(QString data) ;
    int findText(QString text) ;
    int findData(QString data) ;
    const QString& currentText() ;
    const QString& currentData() ;
    const QString& itemText(int index) ;
    const QString& itemData(int index) ;

signals:

public slots:

};

#endif // LINEEDITNAVIGATOR_H
