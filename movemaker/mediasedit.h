#ifndef MEDIASEDIT_H
#define MEDIASEDIT_H
#include <QWidget>
#include "flowlayout.h"
#include "element.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QVBoxLayout;
QT_END_NAMESPACE

class MediasEdit : public QWidget
{
    Q_OBJECT

public:
    MediasEdit(QWidget *parent=NULL);
    ~MediasEdit();
private:
    void initialFirstLayout();
    void createFlowLayout();
    Element* initialImage(const GlobalImageAttr& newGlobalImageAttr, int& idx);
public slots:
    void addMedias();
    //void selectedImage(QWidget*);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
    FlowLayout *m_flowLayout;
    QVBoxLayout *m_firstLayout;
    QLabel* m_firstLabel;
    bool m_isFirstClick;

    QString m_qsFileType;
};

#endif // MEDIASEDIT_H
