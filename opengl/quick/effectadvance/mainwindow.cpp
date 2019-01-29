#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include "mainwindow.h"
#include "threadrenderer.h"
#include "textbox.h"
#include "dynamicentrymodel.h"
#include "documenthandler.h"

MainWindow::MainWindow()
{
    setWindowTitle("qtquick test");
    qDebug()<<"MainWindow::MainWindow";
    qmlRegisterType<ThreadRenderer>("SceneGraphRendering", 1, 0, "Renderer");
    qmlRegisterType<TextBox>("TextBoxPlugin", 1, 0, "TextBox");
    qmlRegisterType<DynamicEntryModel>("org.example", 1, 0, "DynamicEntryModel");
    //qmlRegisterType<SortFilterProxyModel>("org.qtproject.example", 1, 0, "SortFilterProxyModel");
    qmlRegisterType<DocumentHandler>("org.qtproject.example", 1, 0, "DocumentHandler");
    m_quickView=new QQuickView;
    m_quickView->setPersistentOpenGLContext(true);
    m_quickView->setPersistentSceneGraph(true);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    connect(m_quickView, &QQuickView::statusChanged,
            this, &MainWindow::quickViewStatusChanged);
    connect(m_quickView, &QQuickWindow::sceneGraphError,
            this, &MainWindow::sceneGraphError);
    m_quickView->setSource(QUrl(QStringLiteral("qrc:///main.qml")));

    m_currentRootObject = m_quickView->rootObject();

    QWidget *container = QWidget::createWindowContainer(m_quickView);
    container->setMinimumSize(m_quickView->size());
    container->setFocusPolicy(Qt::TabFocus);

    layout->addWidget(new QLineEdit(QStringLiteral("A QLineEdit")));
    layout->addWidget(container);
    m_lineEdit=new QLineEdit(QStringLiteral("{\"posx\":200, \"posy\":0, \"text\":\"hello,world\",\"color\":\"red\"}"));
    layout->addWidget(m_lineEdit);
    m_pbAdd = new QPushButton("add color");
    layout->addWidget(m_pbAdd);
    connect((QObject *)m_currentRootObject, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(m_pbAdd, &QAbstractButton::clicked, [=]() {
            //QMetaObject::invokeMethod(m_currentRootObject, "performLayerBasedGrab",Q_ARG(QVariant, fd.selectedFiles().first()));
            //quickwidgets/quickwidget/main.cpp
            QVariant retVal;
            QMetaObject::invokeMethod(m_currentRootObject, "addNewText"
                    ,Q_RETURN_ARG(QVariant, retVal)
                    ,Q_ARG(QVariant, m_lineEdit->text().trimmed())
                    //,Q_ARG(QVariant, QString("{\"posx\":200, \"posy\":0, \"text\":\"hello,world\",\"color\":\"red\"}"))
                    );
            qDebug()<<"addNewText retVal: "<<retVal.toInt();
            });
    m_lineEditI=new QLineEdit(QStringLiteral("0"));
    layout->addWidget(m_lineEditI);
    m_lineEditM=new QLineEdit(QStringLiteral("{\"posx\":200, \"posy\":0, \"text\":\"hello,world\",\"color\":\"red\"}"));
    layout->addWidget(m_lineEditM);
    m_pbM = new QPushButton("modify index color. red means disappear");
    layout->addWidget(m_pbM);
    connect(m_pbM, &QAbstractButton::clicked, [=]() {
            //QMetaObject::invokeMethod(m_currentRootObject, "performLayerBasedGrab",Q_ARG(QVariant, fd.selectedFiles().first()));
            //quickwidgets/quickwidget/main.cpp
            QMetaObject::invokeMethod(m_currentRootObject, "modify"
                    ,Q_ARG(QVariant, m_lineEditI->text().trimmed())
                    ,Q_ARG(QVariant, m_lineEditM->text().trimmed()));
            });
    setCentralWidget(centralWidget);

    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("Quit"), qApp, &QCoreApplication::quit);
}

void MainWindow::quickViewStatusChanged(QQuickView::Status status)
{
    if (status == QQuickView::Error) {
        QStringList errors;
        const auto viewErrors = m_quickView->errors();
        for (const QQmlError &error : viewErrors)
            errors.append(error.toString());
        statusBar()->showMessage(errors.join(QStringLiteral(", ")));
        qInfo()<<"MainWindow::quickViewStatusChanged error. errors: "<<errors.join(QStringLiteral(", "));
    }
}

void MainWindow::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    statusBar()->showMessage(message);
    qInfo()<<"MainWindow::sceneGraphError error. message: "<<message;
}

void MainWindow::textChanged(QString text)
{
    qDebug()<<"MainWindow::textChanged text: "<<text;
}
