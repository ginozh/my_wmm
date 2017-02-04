#ifndef THEMESLIST_H
#define THEMESLIST_H

#include <QWidget>
#include "flowlayout.h"
class ThemesList : public QWidget
{
    Q_OBJECT
public:
    ThemesList(QWidget *parent=NULL);
    ~ThemesList();
private:
    FlowLayout *m_flowLayout;
};

#endif // THEMESLIST_H
