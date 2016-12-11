#ifndef ANIMATION_H
#define ANIMATION_H
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
#include <QTimer>
#include "videoplayer.h"
#include "comm.h"
//! [0]
class Animation : public QLabel
{
    Q_OBJECT

public:
    Animation(const QString& path, const QString& animation, 
            const QString& tipsname, QSize size,
            QWidget *parent=0);
    void unselectedAnimation();
signals:
    void selectedAnimationSignal(const QString& animation);
    void selectedImageSignal(QWidget*);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
#if 0
private slots:
    void handleContextMenuRequested(const QPoint &pos);
private slots:
    void open();
#endif
private slots:
    void createAnimation();
private:
    QString m_animation;
    QString m_tipsname;
    QPixmap *m_pixMap;
    bool m_focus;
    QTimer m_timerStartAnimation;
    QTimer m_timerPauseAnimation;
    GlobalContext* m_globalContext;

    qint64 m_iEnterTime;
};
//! [0]
#endif // ANIMATION_H
