#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QTabWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QToolButton>
#include <QButtonGroup>
#include <QMap>
#include <QFontComboBox>
#include <QComboBox>
#include "comm.h"
#include "combobox.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QMenu;
class QIcon;
class QColor;
class QAction;
QT_END_NAMESPACE
class GraphicsScene;

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
    TabWidget(QWidget *parent, GlobalContext* globalContext);
public slots:
    void activeTabText(void* element);
    void activeTabVideo(void* element, GlobalVideoAttr* globalVideoAttr);
    void activeTabMusic(GlobalMusicAttr* musicAttr);
#if 0
    void fontSizeChanged(const QString &);
    void currentFontChanged();
    void currentBoldChanged(bool);
#endif
    void textColorChanged();
    void handleFontChange();
    void handleVideoAttrChange();
signals:
    void elementAttrChanged(bool);
private slots:
    void currentChanged(int index);
private:
    void createTabHome();
    void createTabAnimations();
    void createTabVisualEffects();
    void createTabVideo();
    void createTabMusic();
    void createTabText();
    void initialScrollArea(QScrollArea *scrollArea);
    void appendLine(QHBoxLayout *hbox);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);
    QIcon createColorIcon(QColor color);
    void assignTabWidget(const stTextAttr *textItem);
    void assignTabWidget(const GlobalTextAttr *globalTextAttr);
    void assignVideoInfo();
private:
    QListWidget *contentsWidget;
    QSize m_iconSize;
    //QWidget *m_elementsEdit;
    //GraphicsScene* m_scene;
    GlobalContext* m_globalContext;
    void* m_element;
    QMap<void*, stTextAttr*> m_mapText;

    QWidget *m_tabHome;
    QWidget *m_tabAnimations;
    //video
    QMap<void*, GlobalVideoAttr*> m_mapVieo;
    QWidget *m_tabVideo;
    QComboBox* m_cbSpeedVideo;
    ComboBox* m_cbDurationVieo;
    //music
    QWidget *m_tabMusic;
    QComboBox* m_cbStartTimeMusic;
    QComboBox* m_cbStartPointMusic;
    QComboBox* m_cbEndPointMusic;

    //text
    QWidget *m_tabText;
    //font
    QFontComboBox* m_fontCombo;
    QComboBox* m_fontSizeCombo;
    QToolButton *m_boldButton;
    QToolButton *m_italicButton;
    QToolButton *m_underlineButton;
    //ColorEdit *m_colorEdit;
    QToolButton* fontColorToolButton;
    QAction *textAction;
    //Paragraph
    QButtonGroup *m_paragraphTextButtonG;
    QToolButton *m_leftTextButton;
    QToolButton *m_centerTextButton;
    QToolButton *m_rightTextButton;
    //Adjust
    QComboBox* m_startTimeTextCombo;
    QComboBox* m_durationTextCombo;
};
//! [0]
#endif // TABWIDGET_H
