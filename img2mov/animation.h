#ifndef ANIMATION_H
#define ANIMATION_H
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
//! [0]
class Animation : public QLabel
{
    Q_OBJECT

public:
    Animation(const QString& path, const QString& animation, QSize size,QWidget *parent=0);
    void unselectedAnimation();
signals:
    void selectedAnimationSignal(const QString& animation);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
#if 0
private slots:
    void handleContextMenuRequested(const QPoint &pos);
private slots:
    void open();
#endif
private:
    QString m_animation;
    QPixmap *m_pixMap;
    bool m_focus;
};
//! [0]
#endif // ANIMATION_H
