#include "Menu.h"
#include <QApplication>
Menu::Menu(QWidget *parent, QWidget *elementsEdit, int x, int y)
    : QLabel(parent)
    , m_menu(new QMenu(this))
    , m_elementsEdit(elementsEdit)
{
    //widgets/graphicsview/diagramscene
    setObjectName(QStringLiteral("label"));
    //setGeometry(QRect(x, y, 54, 22));
    //setGeometry(rect);
    setText(QApplication::translate("MainWindow", "Menu", 0));
    raise();

    //m_menu->setScaledContents(true);
    m_menu->addAction(tr("Add Image"), m_elementsEdit, SLOT(load())  );
    m_menu->addSeparator();
    m_menu->addAction(tr("E&xit"), parent, SLOT());
    //QWidget* widget = new QWidget( m_menu );
    //widget->setGeometry( 25, 80, 54, 22 );
    //m_menu->setGeometry(QRect(25, 80, 54, 22));
}
void Menu::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //m_menu->exec(QCursor::pos());
        m_menu->exec(parentWidget()->mapToGlobal(QPoint(25,60)));
    }
}
