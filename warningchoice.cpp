#include "warningchoice.h"
#include "dialogfocus.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QIcon>
#include "alabel.h"
#include "apushbutton.h"
#include "atextedit.h"
#include <QSizePolicy>
#include <QStyle>
#include <QTextDocument>
#include <cmath>
#include <QVBoxLayout>

WarningChoice::WarningChoice(QWidget *parent) :
    QDialog(parent),
    defaultindex(-1),
    chosen(-1)
{
    layout = new QVBoxLayout(this) ;

    // The question, with an icon to its left, as WarningOk and WarningYesNo show theirs
    QHBoxLayout *top = new QHBoxLayout() ;
    QStyle *s = QApplication::style() ;
    int sz = s->pixelMetric(QStyle::PM_MessageBoxIconSize) ;
    ALabel *icon = new ALabel(this) ;
    icon->setPixmap(s->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(sz,sz)) ;
    icon->setAlignment(Qt::AlignTop) ;
    top->addWidget(icon) ;

    textWarning = new ATextEdit(this) ;
    textWarning->setFrameShape(QFrame::NoFrame) ;
    textWarning->setFrameShadow(QFrame::Plain) ;
    textWarning->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff) ;
    textWarning->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff) ;
    textWarning->setTabChangesFocus(true) ;
    textWarning->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse) ;
    textWarning->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum) ;   // the choices take the space below
    textWarning->viewport()->setAutoFillBackground(false) ;
    textWarning->installEventFilter(this) ;
    top->addWidget(textWarning, 1) ;
    layout->addLayout(top) ;

}

WarningChoice::~WarningChoice()
{
}

void WarningChoice::setText(QString text)
{
    textWarning->setPlainText(text) ;
}

int WarningChoice::addChoice(QString label, QChar key, QString description)
{
    int index = buttons.size() ;
    QChar letter = key.toLower() ;

    // A square button showing the letter, and the label and description to its right, on two lines
    int square = fontMetrics().height() * 2 + 8 ;
    APushButton *button = new APushButton(QString(letter).toUpper(), this) ;
    button->setFixedSize(square, square) ;
    // The letter first, as the button shows it: "A: Change just this one"
    button->setAccessibleName(QString(letter.toUpper()) + ": " + label) ;
    connect(button, &APushButton::clicked, this, [this, index]() { chooseIndex(index) ; }) ;

    ALabel *details = new ALabel(label + "\n" + description, this) ;
    details->setAlignment(Qt::AlignLeft|Qt::AlignVCenter) ;

    QHBoxLayout *row = new QHBoxLayout() ;
    row->addWidget(button, 0, Qt::AlignTop) ;
    row->addWidget(details, 1) ;
    layout->addLayout(row) ;

    buttons.append(button) ;
    keys.append(letter) ;
    return index ;
}

void WarningChoice::setDefaultChoice(int index)
{
    defaultindex = (index>=0 && index<buttons.size()) ? index : -1 ;
}

int WarningChoice::choose()
{
    chosen = -1 ;

    // Wide enough for the descriptions on one line
    setMinimumWidth(fontMetrics().horizontalAdvance(QString(80, 'x'))) ;

    if (execFocused(this)==QDialog::Accepted) return chosen ;
    return -1 ;
}

void WarningChoice::chooseIndex(int index)
{
    chosen = index ;
    accept() ;
}

// The question box is as tall as its text, measured once shown, when its fonts and width are settled.  The text box
// has the focus, as in WarningOk and WarningYesNo.
void WarningChoice::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event) ;

    // The dialog takes its width first, so that the question is measured at the width it is shown at.  Measured before that,
    // the question was taller than its box at the final width, and the first choice covered its end.
    layout->activate() ;
    resize(qMax(sizeHint().width(), minimumWidth()), height()) ;
    layout->activate() ;

    QTextDocument *document = textWarning->document() ;
    document->setTextWidth(textWarning->viewport()->width()) ;
    textWarning->setFixedHeight(qCeil(document->size().height()) + 2*textWarning->frameWidth() + 8) ;

    // The dialog is as tall as its question and choices, and no taller
    layout->activate() ;
    resize(qMax(sizeHint().width(), minimumWidth()), sizeHint().height()) ;

    // The first choice (A) has the focus when the dialog opens
    if (!buttons.isEmpty()) buttons.first()->setFocus() ;
}

// Letters, Enter and Escape choose, whichever has the focus.  Tab and the arrow keys are left to the text box.
bool WarningChoice::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==textWarning && event->type()==QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event) ;
        int k = key->key() ;
        bool letter = !key->text().isEmpty() && key->text().at(0).isLetter() ;
        if (letter || k==Qt::Key_Enter || k==Qt::Key_Return || k==Qt::Key_Escape) {
            keyPressEvent(key) ;
            return true ;
        }
    }
    return QDialog::eventFilter(watched, event) ;
}

void WarningChoice::keyPressEvent(QKeyEvent *event)
{
    int k = event->key() ;
    if ((k==Qt::Key_Enter || k==Qt::Key_Return) && defaultindex>=0) {
        chooseIndex(defaultindex) ;
        return ;
    }
    QString text = event->text() ;
    if (!text.isEmpty()) {
        int index = keys.indexOf(text.at(0).toLower()) ;
        if (index>=0) {
            chooseIndex(index) ;
            return ;
        }
    }
    QDialog::keyPressEvent(event) ;
}
