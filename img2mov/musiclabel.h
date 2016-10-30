#ifndef MUSICLABEL_H
#define MUSICLABEL_H
#include <QLabel>
//! [0]
class MusicLabel : public QLabel
{
    Q_OBJECT

public:
    MusicLabel(QSize size, QWidget *parent=0);
    void updateMusicInfo(int pos, const QString& qsText);
signals:
    void selectedMusicSignal();
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
    QString m_qsText;
    int m_iFillWidth;
};
//! [0]
#endif // MUSICLABEL_H
