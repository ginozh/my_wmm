#ifndef THEMESLIST_H
#define THEMESLIST_H

#include <QWidget>
#include "flowlayout.h"
#include "image.h"
class ThemesList : public QWidget
{
    Q_OBJECT
public:
    ThemesList(QWidget *parent=NULL);
    ~ThemesList();
public slots:
    void selectedImage(QWidget*);
private:
    FlowLayout *m_flowLayout;
    QWidget *m_lastSelectedElement;
    int m_idxCurrentElement;
};

class ThemeElement : public QWidget
{
    Q_OBJECT
public:
    ThemeElement(const QString& imgName, const QString& displayName, QWidget *parent=NULL);
    ~ThemeElement();
    void unselectedImage(){if(m_pimage){m_pimage->unselectedImage();}}
    void doFocusImage(){if(m_pimage){m_pimage->doFocusImage();}}
public slots:
    void selectedImage();
signals:
    void selectedImageSignal(QWidget*);
private:
    Image* m_pimage;
};

#endif // THEMESLIST_H
