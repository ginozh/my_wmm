#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QTextOption>

QT_BEGIN_NAMESPACE
class QClipboard;
class QComboBox;
class QFontComboBox;
class QLineEdit;
class QScrollArea;
class QCheckBox;
class QGraphicsTextItem;
QT_END_NAMESPACE
class CharacterWidget;
class GraphicsScene;
class GraphicsTextItem;

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    //LineEdit(int idx, QWidget *parent = 0);
    //LineEdit(QWidget *parent = 0);
    LineEdit(QWidget *parent, GraphicsScene* scene);
signals:
    //void selectTextSignal(int idx);
    void selectedTextSignal(const QString&);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
private:
    int m_idx;
    GraphicsScene *scene;
};
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);
protected:
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
public slots:
    void activeVideoText(const QString&);
private:
    GraphicsTextItem *curtextItem;
};
class GraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    GraphicsTextItem();
    QRectF boundingRect() const;
    void hitFrame(QGraphicsSceneHoverEvent *event);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event)
    {
        setTextInteractionFlags(Qt::NoTextInteraction);
        //emit lostFocus(this);
        QGraphicsTextItem::focusOutEvent(event);
    }
    //! [2]

    //! [5]
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        if (textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags(Qt::TextEditorInteraction);
        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
//! [5]
};
//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    //void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;

private:
    //CharacterWidget *characterWidget;
#ifndef QT_NO_CLIPBOARD
    QClipboard *clipboard;
#endif
    QComboBox *styleCombo;
    QComboBox *sizeCombo;
    QFontComboBox *fontCombo;
    QLineEdit *lineEdit;
    QScrollArea *scrollArea;
    QCheckBox *fontMerging;
    GraphicsTextItem * textItem;

    LineEdit* m_lineEdit;
    GraphicsScene *scene;
};
//! [0]

#endif
