#ifndef IMAGE_H
#define IMAGE_H
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
#include "comm.h"
//! [0]
class Image : public QLabel
{
    Q_OBJECT

public:
    Image(const QString& path, QSize size, QWidget *parent=0);
    void unselectedImage();
    void doFocusImage();
	void rotate(bool bRight);
    QString rotateVFilter();
    GlobalImageAttr* globalImageAttr(){return m_globalImageAttr;}
signals:
    void selectedImageSignal();
//public slots:
//    void focusImage();
protected:
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
#if 0
private slots:
    void handleContextMenuRequested(const QPoint &pos);
private slots:
    void open();
#endif
private:
    GlobalImageAttr* m_globalImageAttr;
    QPixmap m_pixMap;
    bool m_focus;
	//int m_iRotateLeft;
    //int m_iRotateRight;
    QSize m_iScaleSize; 
};
//! [0]
#endif // IMAGE_H
