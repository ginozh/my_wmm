#ifndef FORMAT_H
#define FORMAT_H

#include <QWidget>
#include <QFrame>
#include <QDialog>

class Format : public QWidget
{
    Q_OBJECT

public:
    Format(QWidget *parent=NULL);
    ~Format();
private slots:
    void handleSetting();
private:
    //竖线
    QFrame *m_vecticalLine;
    QDialog* m_dlgSetting;
};

#endif // FORMAT_H
