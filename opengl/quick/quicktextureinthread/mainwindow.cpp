#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include "mainwindow.h"
#include "threadrenderer.h"

MainWindow::MainWindow()
{
    setWindowTitle("qtquick test");
    qDebug()<<"MainWindow::MainWindow";
    qmlRegisterType<ThreadRenderer>("SceneGraphRendering", 1, 0, "Renderer");
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
    m_quickView->setSource(QUrl(QStringLiteral("qrc:///scenegraph/textureinthread/main.qml")));

    QWidget *container = QWidget::createWindowContainer(m_quickView);
    container->setMinimumSize(m_quickView->size());
    container->setFocusPolicy(Qt::TabFocus);

    layout->addWidget(new QLineEdit(QStringLiteral("A QLineEdit")));
    layout->addWidget(container);
    layout->addWidget(new QLineEdit(QStringLiteral("A QLineEdit")));
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
    }
}

void MainWindow::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    statusBar()->showMessage(message);
}
