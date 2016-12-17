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
#include <QDoubleSpinBox>
#include "comm.h"
#include "combobox.h"
#include "flowlayout.h"

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
class TabBar : public QTabBar
{
    Q_OBJECT

public:
    TabBar(QWidget *parent=NULL);
protected:
    virtual QSize tabSizeHint(int) const Q_DECL_OVERRIDE;
};
class SpinBox : public QDoubleSpinBox 
{
	Q_OBJECT
public:
	explicit SpinBox( QWidget *parent = 0)
		: QDoubleSpinBox (parent)
	{
	}
signals:
    void valueChangedSignal(double);
protected:
    void leaveEvent(QEvent *event)
    //void focusOutEvent(QFocusEvent* event)
    {
        emit valueChangedSignal(value());
        //QDoubleSpinBox::focusOutEvent(event);
        QDoubleSpinBox::leaveEvent(event);
    }
};
class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent);
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
    void assignTabValue();
signals:
    void elementAttrChanged(int, bool);
//public slots:
    //void currentChanged(int index);
private:
    void createTabHome();
    void createTabAnimations();
    void createTabVisualEffects();
    void createTabVideo();
    void createTabMusic();
    void createTabText();
    void initialScrollArea(QScrollArea *scrollArea);
    void appendLine(QHBoxLayout *hbox);
    void appendExpandingWidget(QHBoxLayout *hbox);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);
    QIcon createColorIcon(QColor color);
    //void assignTabWidget(const stTextAttr *textItem);
    void assignTextInfo();
    void assignVideoInfo();
    void assignAnimationInfo();
    void assignMusciInfo();
private:
    QListWidget *contentsWidget;
    QSize m_iconSize;
    int m_iCellHeight; // 4 cells
    //QWidget *m_elementsEdit;
    //GraphicsScene* m_scene;
    GlobalContext* m_globalContext;
    void* m_element;
    //QMap<void*, stTextAttr*> m_mapText;

    QWidget *m_tabHome;
    QWidget *m_tabAnimations;
    //home
    QToolButton *m_tbRotateLeft;
    QToolButton *m_tbRotateRight;
    //video
    //QMap<void*, GlobalVideoAttr*> m_mapVieo;
    QWidget *m_tabVideo;
    QComboBox* m_cbSpeedVideo;
    ComboBox* m_cbDurationVideo;
    QDoubleValidator* m_dvDurationVideo;
    //music
    QWidget *m_tabMusic;
    SpinBox* m_dsbStartTimeMusic;
    SpinBox* m_dsbStartPointMusic;
    SpinBox* m_dsbEndPointMusic;
    //ComboBox* m_cbStartTimeMusic;
    //QDoubleValidator* m_dvStartTimeMusic;
    //ComboBox* m_cbStartPointMusic;
    //QDoubleValidator* m_dvStartPointMusic;
    //ComboBox* m_cbEndPointMusic;
    //QDoubleValidator* m_dvEndPointMusic;

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

    //animation
    FlowLayout* m_flowLayoutTransition;
    FlowLayout* m_flowLayoutPanZoom;
};
//! [0]
#endif // TABWIDGET_H
