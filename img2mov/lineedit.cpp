#include "lineedit.h"
#include <QMessageBox>
#include <QCoreApplication>
#include "videoscene.h"
#include <QPainter>

LineEdit::LineEdit(QSize size, GraphicsScene* scene, QWidget *parent)
    //: QLabel(parent)
    : QLineEdit(parent)
      //, m_idx(idx)
    , m_scene(scene)
    , m_qsPreText("A ")
    , m_qsInitialText("[Enter text here]")
{
    setFixedSize(size);
    setReadOnly(true);
    setPlaceholderText(m_qsPreText);
    //setPlaceholderText(m_qsPreText+m_qsInitialText);
    //setStyleSheet("background-color: yellow;");
    setStyleSheet("background-color: #ffff00;");
    //connect(this, SIGNAL(selectTextSignal(int)), parentWidget(), SLOT(selectedText()));
    //connect(this, SIGNAL(selectTextSignal()), parentWidget(), SLOT(selectedText()));
}
void LineEdit::mousePressEvent(QMouseEvent* /*event*/)
{
    //emit selectTextSignal(m_idx);
    //QMessageBox::information(this, "info", QString(tr("textlabel")));
    //emit selectedTextSignal(placeholderText());
    //emit selectedTextSignal("");
    activeText();
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
void LineEdit::paintEvent(QPaintEvent *ev)
{
    //QPainter painter(this);
    //painter.fillRect(rect(), Qt::yellow);
    QLineEdit::paintEvent(ev);
}

void LineEdit::activeText()
{
    if(placeholderText().compare(m_qsPreText)==0 ||
            placeholderText().compare(m_qsPreText+m_qsInitialText)==0)
    {
        m_qsVideoText = m_qsInitialText; 
        setPlaceholderText(m_qsPreText+m_qsInitialText);
        emit selectedTextSignal(m_qsInitialText);
    }
    else
    {
        emit selectedTextSignal("");
    }
}
void LineEdit::addTextByTabCaption()
{
    setFocus();
    activeText();
}
void LineEdit::updateTextByVideo(const QString& qsText)
{
    m_qsVideoText = qsText;
    setPlaceholderText(m_qsPreText+qsText);
}
