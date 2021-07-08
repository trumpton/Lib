#include "lineeditnavigator.h"
#include <QDir>
#include <QDate>
#include <QApplication>
#include <QInputDialog>
#include "../Lib/alertsound.h"

LineEditNavigator::LineEditNavigator(QWidget *parent) :
    QLineEdit(parent)
{
    clear() ;
}

LineEditNavigator::~LineEditNavigator()
{
}

void LineEditNavigator::clear()
{
    entries.clear() ;
    arguments.clear() ;
    ix=-1 ;
}

void LineEditNavigator::selectItem(QString item)
{
    setCurrentIndex(findText(item)) ;
}

void LineEditNavigator::selectData(QString data)
{
    setCurrentIndex(findData(data)) ;
}

int LineEditNavigator::findData(QString data)
{
    for (int i=0, sz=arguments.size(); i<sz; i++) {
        if (data.compare(arguments.at(i))==0) return i ;
    }
    return -1 ;
}


int LineEditNavigator::findText(QString item)
{
    item=item.toLower() ;
    for (int i=0, sz=entries.size(); i<sz; i++) {
        if (item.compare(entries.at(i).toLower())==0) return i ;
    }
    return -1 ;
}


void LineEditNavigator::setCurrentIndex(int i)
{
    if (entries.size()==0 || i<0) ix=-1 ;
    else if (i>=entries.size()) ix=entries.size()-1 ;
    else ix=i ;
    if (ix>=0) {
        QString txt = entries.at(ix) ;
        txt.replace("_","") ;
        this->setText(txt) ;
    } else {
        this->setText("") ;
    }
    this->setCursorPosition(0) ;
}

void LineEditNavigator::addItem(QString entry, QString data, int i)
{
    if (i>=0 && i<entry.size()) {
        entries.insert(i, entry) ;
        arguments.insert(i, data) ;
    } else {
        QString lcentry = entry.toLower() ;
        int j=0, sz=entries.size() ;
        bool found=false ;
        while (j<sz && !found) {
            if (entries.at(j).toLower() > lcentry) found=true ;
            else j++ ;
        }
        if (found) {
            entries.insert(j, entry) ;
            arguments.insert(j, data) ;
        } else {
            entries.append(entry) ;
            arguments.append(data) ;
        }
    }
    if (ix!=0) {
        setCurrentIndex(ix) ;
    }
}

int LineEditNavigator::currentIndex()
{
    return ix ;
}

int LineEditNavigator::size()
{
    return entries.size() ;
}

const QString& LineEditNavigator::itemText(int i)
{
    if (i<0 || i>entries.size()) selectionText="" ;
    else selectionText = entries.at(i) ;
    return selectionText ;
}

const QString& LineEditNavigator::currentText()
{
    return itemText(ix) ;
}

const QString& LineEditNavigator::itemData(int i)
{
    if (i<0 || i>=arguments.size()) dataText="" ;
    else dataText = arguments.at(i) ;
    return dataText ;
}

const QString& LineEditNavigator::currentData()
{
    return itemData(ix) ;
}

void LineEditNavigator::keyPressEvent(QKeyEvent *event)
{
    QString txt="" ;
    bool updated=false ;
    int key = event->key() ;

    if (key==Qt::Key_Enter || key==Qt::Key_Return || key==Qt::Key_Escape) {
        QLineEdit::keyPressEvent(event);
        return ;
    }
    else if (isReadOnly()) {
        play(Disabled) ;
        return ;
    }
    else if (entries.size()<=0) {
        play(Disabled) ;
        return ;
    }

    else if (key==Qt::Key_Up) {
        ix-- ;
        if (ix<0) ix=0 ;
        txt=entries.at(ix) ;
        updated=true ;
    }

    else if (key==Qt::Key_Down) {
        ix++ ;
        if (ix>=entries.size()) ix=entries.size()-1 ;
        txt=entries.at(ix) ;
        updated=true ;
    }

    else if (key==Qt::Key_Minus || (key>=Qt::Key_A && key<=Qt::Key_Z) || (key>=Qt::Key_0 && key<=Qt::Key_9)) {
        QChar letter ;
        if (key==Qt::Key_Minus) letter = '-' ;
        else if (key>=Qt::Key_A && key<=Qt::Key_Z) letter = QChar('a' + key - Qt::Key_A) ;
        else letter = QChar('0' + key - Qt::Key_0) ;

        int currentindex=ix ;
        bool finished=false ;
        while (!finished) {
            currentindex++ ;
            if (currentindex>=entries.size()) currentindex=0 ;
            QChar ch = entries.at(currentindex).at(0).toLower() ;
            if (currentindex==ix) {
                play(Ok) ;
                finished=true ;
            } else if (ch==letter) {
                finished=true ;
                ix=currentindex ;
                txt=entries.at(ix) ;
                updated=true ;
            }
        }

    } else {
        play(Disabled) ;
    }

    if (updated) {
        setCurrentIndex(ix) ;
    }

}


void LineEditNavigator::focusInEvent ( QFocusEvent * event)
{
    QLineEdit::focusInEvent(event) ;
    this->setCursorPosition(0) ;
}

