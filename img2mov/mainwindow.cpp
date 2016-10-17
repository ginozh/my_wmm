#include "mainwindow.h"
#include "elementsedit.h"
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QStatusBar>
#include <QTextEdit>
#include <QTreeView>
#include <QTableWidget>

#include <QFileInfo>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_globalContext(new GlobalContext)
    //, m_treeView(new QTreeView)
    //, m_detailsText(new QTextEdit)
{
    setWindowTitle(tr("img to movie"));
    //installEventFilter( m_player->Scene() );

    //setFocusPolicy ( Qt::StrongFocus );
    m_centralWidget = new QWidget(this);
    m_globalContext->m_player = new VideoPlayer(this);
    m_globalContext->m_scene = m_globalContext->m_player->Scene();

    m_globalContext->m_elementsEdit = new ElementsEdit(this, m_globalContext);

#if 0
    //widgets/widgets/tablet
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(tr("&Open..."), m_elementsEdit, &ElementsEdit::load, QKeySequence::Open);
    fileMenu->addAction(tr("&Open..."), m_elementsEdit, SLOT(load()), QKeySequence::Open);
    QAction *exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QApplication::closeAllWindows);
    exitAction->setShortcuts(QKeySequence::Quit);

    //QMenu *findMenu = menuBar()->addMenu(tr("&Edit"));
    menuBar()->addMenu(tr("&About"))->addAction(tr("&About Qt"), qApp, &QApplication::aboutQt);
#endif
    m_globalContext->m_tabWidget = new TabWidget(m_centralWidget, m_globalContext);

    QSplitter *centralSplitter = new QSplitter(m_centralWidget);
    //setCentralWidget(centralSplitter);
    //m_treeView = new QTreeView;

    //centralSplitter->addWidget(m_treeView);
    centralSplitter->addWidget(m_globalContext->m_player);
    /*const QRect availableGeometry = QApplication::desktop()->availableGeometry(m_player);
    m_player->resize(availableGeometry.width() / 6, availableGeometry.height() / 4);
    m_player->show();*/
    //m_detailsText->setReadOnly(true);
#if 1
    m_scrollArea = new QScrollArea;
    m_scrollArea->setBackgroundRole(QPalette::Light);
    m_scrollArea->setMinimumWidth(500);
    m_scrollArea->setWidgetResizable (true);
    m_scrollArea->setWidget(m_globalContext->m_elementsEdit);
    centralSplitter->addWidget(m_scrollArea);
#else
    centralSplitter->addWidget(m_elementsEdit);
#endif
    setCentralWidget(m_centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_menu = new Menu(m_centralWidget, m_globalContext->m_elementsEdit, m_globalContext->m_tabWidget->geometry().x(), m_globalContext->m_tabWidget->geometry().y());
    //QMessageBox::information(this, "Error Opening Picture", QString(tr("x: %1 y: %2")).arg(m_tabWidget->frameGeometry().x()).arg(m_tabWidget->frameGeometry().y()));
    mainLayout->addWidget(m_globalContext->m_tabWidget);
    mainLayout->addWidget(centralSplitter);

    m_centralWidget->setLayout(mainLayout);


    connect(m_globalContext->m_elementsEdit, SIGNAL(readyVideo(const QString&,const QByteArray&,int)), m_globalContext->m_player, SLOT(readyVideo(const QString&, const QByteArray&, int)));
    connect(m_globalContext->m_player->MediaPlayer(), SIGNAL(durationChanged(qint64)), m_globalContext->m_elementsEdit, SLOT(durationChanged(qint64)));
    connect(m_globalContext->m_player->MediaPlayer(), SIGNAL(positionChanged(qint64)), m_globalContext->m_elementsEdit, SLOT(positionChanged(qint64))); //播放条变更后，移动图片区的垂直条
    connect(m_globalContext->m_elementsEdit, SIGNAL(changePlayPosition(int)), m_globalContext->m_player, SLOT(setPosition(int)));
    connect(m_globalContext->m_elementsEdit, SIGNAL(playVideo()), m_globalContext->m_player, SLOT(play()));

    //创建videotext
    connect(m_globalContext->m_elementsEdit, SIGNAL(createTextSignal(void*)), m_globalContext->m_scene, SLOT(createText(void*)));
    //显示/隐藏videotext
    connect(m_globalContext->m_elementsEdit, SIGNAL(displayTextSignal(void*, bool /*display*/)), m_globalContext->m_scene, SLOT(displayVideoText(void*, bool)));
    //激活videotext
    connect(m_globalContext->m_elementsEdit, SIGNAL(activeVideoTextSignal(void*, const QString&)), m_globalContext->m_scene, SLOT(activeVideoText(void*, const QString&)));
    //更新videotext，生成2个视频：一个包含文字视频、一个未包含文字视频。
    //编辑文字时显示无文字视频；播放时显示有文字视频？
    //connect(m_globalContext->m_scene, SIGNAL(updatedTextSignal(stTextAttr*, const QString&)), m_globalContext->m_elementsEdit, SLOT(updatedText(stTextAttr*, const QString&)));
    connect(m_globalContext->m_scene, SIGNAL(updatedTextSignal(const QString&)), m_globalContext->m_elementsEdit, SLOT(updatedText(const QString&)));

    // tab
    connect(m_globalContext->m_elementsEdit, SIGNAL(activeTabTextSignal(void*)), m_globalContext->m_tabWidget, SLOT(activeTabText(void*)));

}
#if 0
void MainWindow::load()
{
    //qtconcurrent/imagescaling
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            "*.jpg *.png");
    if (files.count() == 0)
        return;
    for (int i = 0; i < files.count(); ++i) {
    }

    /*if (!m_canvas->loadImage(fileName))
        QMessageBox::information(this, "Error Opening Picture",
                                 "Could not open picture");
                                 */
}
#endif
void MainWindow::resizeEvent(QResizeEvent *ev)
{
    m_menu->setGeometry(QRect(m_globalContext->m_tabWidget->geometry().x()+18, m_globalContext->m_tabWidget->geometry().y()+3, m_globalContext->m_tabWidget->iconSize().width()+30, m_globalContext->m_tabWidget->iconSize().height()));
    //QMessageBox::information(this, "menu", QString(tr("x: %1 y: %2")).arg(m_tabWidget->frameGeometry().x()).arg(m_tabWidget->frameGeometry().y()));
    //QMessageBox::information(this, "menu", QString(tr("w: %1 h: %2")).arg(m_tabWidget->iconSize().width()).arg(m_tabWidget->iconSize().height()));
    //qDebug()<< "resizeEvent w: " << m_tabWidget->iconSize().width() << " h: "<< m_tabWidget->iconSize().height();
    QMainWindow::resizeEvent(ev);
}
#if 0
void MainWindow::paintEvent(QPaintEvent *ev)
{
    QMainWindow::paintEvent(ev);
}
#endif
void MainWindow::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("MainWindow::keyPressEvent")));
    QMainWindow::keyPressEvent(keyEvent);
}
