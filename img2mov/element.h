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
    Element();
private:
    QVBoxLayout *m_elementLayout;
};
//! [0]

#endif // ELEMENT_H
