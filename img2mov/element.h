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
    const QString& getImageName(){return m_qsImageName;}
signals:
    void insertImage();
public slots:
    void insert();
private:
    QVBoxLayout *m_elementLayout;
    QString m_qsImageName;
};
//! [0]

#endif // ELEMENT_H
