#ifndef LINEEDIT_H
#define LINEEDIT_H
//#include <QLabel>
#include <QLineEdit>
class GraphicsScene;
//class LineEdit : public QLabel
class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    //LineEdit(int idx, QWidget *parent = 0);
    LineEdit(GraphicsScene* scene, QWidget *parent = 0);
signals:
    //void selectTextSignal(int idx);
    void selectedTextSignal(const QString&);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
private:
    int m_idx;
    GraphicsScene* m_scene;
};

#endif // LINEEDIT_H
