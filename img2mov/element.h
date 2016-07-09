#ifndef ELEMENT_H
#define ELEMENT_H

#include <QWidget>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
//! [0]
class Element : public QWidget
{
    Q_OBJECT

public:
    Element(QWidget *parent = 0, const QString& image=tr(""));
private:
    QVBoxLayout *m_elementLayout;
};
//! [0]

#endif // ELEMENT_H
