/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionleftright;
    QAction *actionupdown;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QWidget *widget_controller;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_open;
    QPushButton *pushButton_play;
    QPushButton *pushButton_pause;
    QPushButton *pushButton_stop;
    QPushButton *pushButton_two;
    QPushButton *pushButton_VRMode;
    QSlider *horizontalSlider;
    QHBoxLayout *horizontalLayout;
    QLabel *label_currenttime;
    QLabel *label_2;
    QLabel *label_totaltime;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(832, 540);
        actionleftright = new QAction(MainWindow);
        actionleftright->setObjectName(QStringLiteral("actionleftright"));
        actionupdown = new QAction(MainWindow);
        actionupdown->setObjectName(QStringLiteral("actionupdown"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 378, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        widget_controller = new QWidget(centralwidget);
        widget_controller->setObjectName(QStringLiteral("widget_controller"));
        widget_controller->setMinimumSize(QSize(0, 32));
        widget_controller->setMaximumSize(QSize(16777215, 32));
        widget_controller->setStyleSheet(QLatin1String("QWidget#widget_controller\n"
"{\n"
"	background-color:  rgb(255, 255, 255);\n"
"                     \n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(widget_controller);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        pushButton_open = new QPushButton(widget_controller);
        pushButton_open->setObjectName(QStringLiteral("pushButton_open"));

        horizontalLayout_2->addWidget(pushButton_open);

        pushButton_play = new QPushButton(widget_controller);
        pushButton_play->setObjectName(QStringLiteral("pushButton_play"));

        horizontalLayout_2->addWidget(pushButton_play);

        pushButton_pause = new QPushButton(widget_controller);
        pushButton_pause->setObjectName(QStringLiteral("pushButton_pause"));

        horizontalLayout_2->addWidget(pushButton_pause);

        pushButton_stop = new QPushButton(widget_controller);
        pushButton_stop->setObjectName(QStringLiteral("pushButton_stop"));

        horizontalLayout_2->addWidget(pushButton_stop);

        pushButton_two = new QPushButton(widget_controller);
        pushButton_two->setObjectName(QStringLiteral("pushButton_two"));

        horizontalLayout_2->addWidget(pushButton_two);

        pushButton_VRMode = new QPushButton(widget_controller);
        pushButton_VRMode->setObjectName(QStringLiteral("pushButton_VRMode"));

        horizontalLayout_2->addWidget(pushButton_VRMode);

        horizontalSlider = new QSlider(widget_controller);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(horizontalSlider);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_currenttime = new QLabel(widget_controller);
        label_currenttime->setObjectName(QStringLiteral("label_currenttime"));

        horizontalLayout->addWidget(label_currenttime);

        label_2 = new QLabel(widget_controller);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        label_totaltime = new QLabel(widget_controller);
        label_totaltime->setObjectName(QStringLiteral("label_totaltime"));

        horizontalLayout->addWidget(label_totaltime);


        horizontalLayout_2->addLayout(horizontalLayout);

        pushButton_play->raise();
        pushButton_pause->raise();
        pushButton_stop->raise();
        horizontalSlider->raise();
        pushButton_open->raise();
        pushButton_two->raise();
        pushButton_VRMode->raise();

        verticalLayout->addWidget(widget_controller);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 832, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionleftright->setText(QApplication::translate("MainWindow", "leftright", Q_NULLPTR));
        actionupdown->setText(QApplication::translate("MainWindow", "updown", Q_NULLPTR));
        pushButton_open->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        pushButton_play->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        pushButton_pause->setText(QApplication::translate("MainWindow", "Pause", Q_NULLPTR));
        pushButton_stop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        pushButton_two->setText(QApplication::translate("MainWindow", "VR", Q_NULLPTR));
        pushButton_VRMode->setText(QApplication::translate("MainWindow", "VRMode", Q_NULLPTR));
        label_currenttime->setText(QApplication::translate("MainWindow", "00:00:00", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "/", Q_NULLPTR));
        label_totaltime->setText(QApplication::translate("MainWindow", "00:00:00", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
