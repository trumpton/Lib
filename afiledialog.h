#ifndef AFILEDIALOG_H
#define AFILEDIALOG_H

#include <QFileDialog>

// AFileDialog: the file dialogs of ContactManager and Lib.  For now each call passes through to the system's QFileDialog, with
// the same arguments.  Every change to how file dialogs are shown is made here.
class AFileDialog
{
public:
    static QString getSaveFileName(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(),
                                   const QString &filter = QString(), QString *selectedFilter = nullptr,
                                   QFileDialog::Options options = QFileDialog::Options())
    {
        return QFileDialog::getSaveFileName(parent, caption, dir, filter, selectedFilter, options) ;
    }
    static QString getOpenFileName(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(),
                                   const QString &filter = QString(), QString *selectedFilter = nullptr,
                                   QFileDialog::Options options = QFileDialog::Options())
    {
        return QFileDialog::getOpenFileName(parent, caption, dir, filter, selectedFilter, options) ;
    }
} ;

#endif // AFILEDIALOG_H
