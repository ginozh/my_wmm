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
    LineEdit(QSize size, GraphicsScene* scene, QWidget *parent = 0);
    void addTextByTabCaption();
    void updateTextByVideo(const QString&);
    QString& videoText(){return m_qsVideoText;} //uncomple 有次对象提供貌似不太合理
private:
    void activeText();
signals:
    //void selectTextSignal(int idx);
    void selectedTextSignal(const QString&);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    int m_idx;
    GraphicsScene* m_scene;
    bool m_isFirstClick;

    const QString m_qsPreText;
    const QString m_qsInitialText;
    QString m_qsVideoText; //uncomplete 
};

#endif // LINEEDIT_H
