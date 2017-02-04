#ifndef THEMES_H
#define THEMES_H

#include <QWidget>
#include "themeslist.h"
class Themes : public QWidget
{
    Q_OBJECT
public:
    Themes(QWidget *parent=NULL);
    ~Themes();
private:
    ThemesList* m_themeslist;
};

#endif // THEMES_H
