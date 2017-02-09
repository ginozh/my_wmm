#ifndef SAVE_H
#define SAVE_H
#include <QWidget>
#include <QToolButton>
#include <QStackedLayout>
#include "format.h"
#include "device.h"

class Save : public QWidget
{
    Q_OBJECT

public:
    Save(QWidget *parent=NULL);
    ~Save();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private slots:
    void handleTabChange();
private:
    void createStackWidget();
private:
    QStackedLayout* m_stackedLayout;

    QToolButton* m_btnFormat;
    QToolButton* m_btnDevice;
    QToolButton* m_btnYoutube;
    QToolButton* m_btnFacebook;
    QToolButton* m_btnVimeo;

    Format* m_format;
    Device* m_device;
};

#endif // SAVE_H
