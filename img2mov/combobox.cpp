#include "combobox.h"
#include <QMessageBox>
#include <QDebug>

ComboBox::ComboBox(QWidget *parent)
    : QComboBox(parent)
{
}
void ComboBox::leaveEvent(QEvent *event)
{
#if 0
    if(!m_bActived)
        return;
    m_bActived = false;
#endif
    //QComboBox::leaveEvent(event);
    QString qsText = currentText();
    //QMessageBox::information(this, "Info", QString(tr("last text: %1 current text: %2")).arg(m_qsLastText).arg(qsText));
    //emit textChangedSignal(qsText);
#if 1
    qDebug() << "qsText: " << qsText << " m_qsLastText: " << m_qsLastText;
    if(m_qsLastText.compare(qsText)!=0)
    {
        m_qsLastText = qsText;
        emit textChangedSignal(qsText);
    }
    else
    {
    }
#endif
}
void ComboBox::setCurrentText(const QString &text)
{
    //m_bActived = true;
    QComboBox::setCurrentText(text);
    //QMessageBox::information(this, "Info", QString(tr("last text: %1 current text: %2")).arg(m_qsLastText).arg(text));
    qDebug() << "setCurrentText. qsText: " << text << " m_qsLastText: " << m_qsLastText;
    m_qsLastText = text;
#if 0
    //m_mapElementText[element] = text;
    if(m_mapElementText.contains(element))
    {
        m_mapElementText[element] = text;
    }
    else
    {
        textItem = new stTextAttr;
        m_mapText.insert(element, textItem);
    }
#endif
}
