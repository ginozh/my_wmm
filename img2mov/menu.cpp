#include "Menu.h"
#include <QApplication>
Menu::Menu(QWidget *parent, QWidget *elementsEdit, int x, int y)
    : QLabel(parent)
    , m_menu(new QMenu(this))
    , m_elementsEdit(elementsEdit)
{
    //下拉: widgets/graphicsview/diagramscene
    setObjectName(QStringLiteral("label"));
    //setGeometry(QRect(x, y, 83, 32));
    //setGeometry(rect);
    setText(QApplication::translate("MainWindow", "Menu", 0));
    raise();

    //m_menu->setScaledContents(true);
    m_menu->addAction(tr("New Project"), m_elementsEdit, SLOT(addImages())  );
    //xml parser: QXmlStreamReader xml/htmlinfo
    m_menu->addAction(tr("Open Project"), m_elementsEdit, SLOT(addImages())  );
    //xml write: QXmlStreamWriter xml/xmlstreamlint
    /*
       QXmlStreamWriter stream(&output);
       stream.setAutoFormatting(true);
       stream.writeStartDocument();
       ...
       stream.writeStartElement("bookmark");
       stream.writeAttribute("href", "http://qt-project.org/");
       stream.writeTextElement("title", "Qt Project");
       stream.writeEndElement(); // bookmark
       ...
       stream.writeEndDocument();
     */
    m_menu->addAction(tr("Save Project"), m_elementsEdit, SLOT(addImages())  );
    m_menu->addSeparator();
    m_menu->addAction(tr("Publish Moive"), m_elementsEdit, SLOT(addImages())  );
    m_menu->addAction(tr("Save Moive"), m_elementsEdit, SLOT(saveVideo())  );
    //m_menu->addAction(tr("Add Image"), m_elementsEdit, SLOT(addImages())  );
    m_menu->addSeparator();
    //m_menu->addAction(tr("E&xit"), parent, SLOT());
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
