#ifndef ELEMENT_H
#define ELEMENT_H

#include <QWidget>
#include <QVBoxLayout>
#include "image.h"

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
    void unselectedImage(){if(m_pimage){m_pimage->unselectedImage();}}
signals:
    void insertImage();
    void selectedImageSignal();
public slots:
    void insert();
    void selectedImage();
private:
    QVBoxLayout *m_elementLayout;
    QString m_qsImageName;
    Image *m_pimage;
};
//! [0]

#endif // ELEMENT_H
