#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsTextItem>

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
class DiagramScene;
class DiagramTextItem;

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    //LineEdit(int idx, QWidget *parent = 0);
    //LineEdit(QWidget *parent = 0);
    LineEdit(QWidget *parent, DiagramScene* scene);
signals:
    //void selectTextSignal(int idx);
    void selectedTextSignal(const QString&);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
private:
    int m_idx;
    DiagramScene *scene;
};
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DiagramScene(QObject *parent = 0);
protected:
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
public slots:
    void activeVideoText(const QString&);
private:
    DiagramTextItem *curtextItem;
};
class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
};
//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
    void insertCharacter(const QString &character);
#ifndef QT_NO_CLIPBOARD
    void updateClipboard();
#endif
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
    DiagramTextItem * textItem;

    LineEdit* m_lineEdit;
    DiagramScene *scene;
};
//! [0]

#endif
