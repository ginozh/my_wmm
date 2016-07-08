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
    Image(const QString& path=tr(""));
protected:
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE;
#if 0
protected:
    void    mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void handleContextMenuRequested(const QPoint &pos);
#endif
private:
    QPixmap *m_pixMap;
};
//! [0]
#endif // IMAGE_H
