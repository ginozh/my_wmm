#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QTabWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QToolButton>
#include <QMap>
#include <QFontComboBox>
#include <QComboBox>
#include "comm.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QMenu;
class QIcon;
class QColor;
class QAction;
QT_END_NAMESPACE
class DiagramScene;

class ParameterEdit : public QWidget
{
public:
    virtual void emitChange() = 0;
};
//class ColorEdit : public ParameterEdit
class ColorEdit : public QWidget
{
    Q_OBJECT
public:
    ColorEdit(QRgb initialColor, int id);
    QRgb color() const {return m_color;}
    //virtual void emitChange() Q_DECL_OVERRIDE {emit colorChanged(m_color, m_id);}
    void setColor(QRgb color); // also emits colorChanged()
public slots:
    void editDone();
signals:
    void colorChanged(QRgb color, int id);
protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
    //QGraphicsScene *m_dialogParentScene;
    QLineEdit *m_lineEdit;
    QFrame *m_button;
    QRgb m_color;
    int m_id;
};
//! [0]
class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent, QWidget *elementsEdit, DiagramScene* scene);
public slots:
    void activeTabText(void* element);
#if 0
    void fontSizeChanged(const QString &);
    void currentFontChanged();
    void currentBoldChanged(bool);
#endif
    void handleFontChange();

private:
    void createTabHome();
    void createTabAnimations();
    void createTabText();
    void initialScrollArea(QScrollArea *scrollArea);
    void appendLine(QHBoxLayout *hbox);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);
    QIcon createColorIcon(QColor color);
    void assignTabWidget(const stTextAttr *textItem);
    void textColorChanged();
private:
    QListWidget *contentsWidget;
    QSize m_iconSize;
    QWidget *m_elementsEdit;
    DiagramScene* m_scene;
    void* m_element;
    QMap<void*, stTextAttr*> m_mapText;

    QWidget *m_tabHome;
    QWidget *m_tabAnimations;

    //text
    QWidget *m_tabText;
    //font
    QFontComboBox* m_fontCombo;
    QComboBox* m_fontSizeCombo;
    QToolButton *m_boldButton;
    QToolButton *m_italicButton;
    QToolButton *m_underlineButton;
    ColorEdit *m_colorEdit;
    QToolButton* fontColorToolButton;
    QAction *textAction;
    //Paragraph
    QToolButton *m_leftTextButton;
    QToolButton *m_centerTextButton;
    QToolButton *m_rightTextButton;
    //Adjust
    QComboBox* m_startTimeTextCombo;
    QComboBox* m_durationTextCombo;
};
//! [0]
#endif // TABWIDGET_H
