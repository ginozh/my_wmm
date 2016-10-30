#ifndef COMBOBOX_H
#define COMBOBOX_H
#include <QComboBox>
//! [0]
class ComboBox : public QComboBox
{
    Q_OBJECT

public:
    ComboBox(QWidget *parent=0);
signals:
    void textChangedSignal(QString);
public slots:
    void setCurrentText(const QString &text);
protected:
    //void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
private:
    QString m_qsLastText;
    //QMap<void*, QString /*m_qsLastText*/> m_mapElementText;
    //bool m_bActived;
};
//! [0]
#endif // COMBOBOX_H
