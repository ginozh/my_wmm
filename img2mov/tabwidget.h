#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QTabWidget>
#include <QListWidget>
//! [0]
class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent=0, QWidget *elementsEdit=0);
private:
    //void createIcons();
    void createTabHome();
    void createTabHome1();
    void createTabAnimations();
private:
    QListWidget *contentsWidget;
    QSize m_iconSize;
    QWidget *m_elementsEdit;
};
//! [0]
#endif // TABWIDGET_H
