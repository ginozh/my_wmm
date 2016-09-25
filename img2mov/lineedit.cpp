#include "lineedit.h"
#include <QMessageBox>

LineEdit::LineEdit(QWidget *parent)
    //: QLabel(parent)
    : QLineEdit(parent)
      //, m_idx(idx)
{
    setPlaceholderText("A[Enter text here]");
    setReadOnly(true);
    //connect(this, SIGNAL(selectTextSignal(int)), parentWidget(), SLOT(selectedText()));
    //connect(this, SIGNAL(selectTextSignal()), parentWidget(), SLOT(selectedText()));
}
void LineEdit::mousePressEvent(QMouseEvent *event)
{
    //emit selectTextSignal(m_idx);
    //QMessageBox::information(this, "info", QString(tr("textlabel")));
    emit selectedTextSignal(placeholderText());
    //QLabel::mouseDoubleClickEvent(event);
    //QLabel::mousePressEvent(event);
    //QLineEdit::mousePressEvent(event);
}
