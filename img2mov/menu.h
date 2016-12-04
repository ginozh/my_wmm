#ifndef MENU_H
#define MENU_H
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QToolButton>
//! [0]
class Menu : public QToolButton
{
    Q_OBJECT

public:
//    Menu(QWidget *parent=0, QWidget *elementsEdit=0);
    //Menu(QWidget *parent, QWidget *elementsEdit, const QRect &rect);
    Menu(QWidget *parent, QWidget *elementsEdit, int x, int y);
protected:
    //void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
    QMenu* m_menu;
    QWidget *m_elementsEdit;
};
//! [0]
#endif // MENU_H
