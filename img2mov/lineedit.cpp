#include "lineedit.h"
#include <QMessageBox>
#include <QCoreApplication>
#include "videoscene.h"

LineEdit::LineEdit(GraphicsScene* scene, QWidget *parent)
    //: QLabel(parent)
    : QLineEdit(parent)
      //, m_idx(idx)
    ,m_scene(scene)
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
void LineEdit::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("LineEdit::keyPressEvent")));
    //QCoreApplication::sendEvent((QObject *)m_scene->getGraphicsTextItem(), (QEvent*)keyEvent);
    QCoreApplication::sendEvent((QObject *)m_scene, (QEvent*)keyEvent);
    //QLineEdit::keyPressEvent(keyEvent);
}
