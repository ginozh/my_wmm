#ifndef IMAGE_H
#define IMAGE_H
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
//! [0]
class Image : public QLabel
{
    Q_OBJECT

public:
    Image(const QString& path, QSize size, QWidget *parent=0);
    void unselectedImage();
signals:
    void selectedImageSignal();
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
    QPixmap *m_pixMap;
    bool m_focus;
};
//! [0]
#endif // IMAGE_H
