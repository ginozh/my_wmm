#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QTabWidget>
#include <QListWidget>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE
//! [0]
class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent=0, QWidget *elementsEdit=0);
private:
    void createTabHome();
    void createTabAnimations();
    void createTabText();
    void initialScrollArea(QScrollArea *scrollArea);
    void appendLine(QHBoxLayout *hboxAnimations);
private:
    QListWidget *contentsWidget;
    QSize m_iconSize;
    QWidget *m_elementsEdit;
};
//! [0]
#endif // TABWIDGET_H
