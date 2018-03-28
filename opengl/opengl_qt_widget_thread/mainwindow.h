#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QThread>

class GLWidget;
class MainWindow;
class GLHiddenWidget;
class QOpenGLContext;
class QDockWidget;
class TestOpenCLThread:public QThread
{
	Q_OBJECT
public:
	TestOpenCLThread(MainWindow *parent):m_parent(parent){}
	void run() Q_DECL_OVERRIDE;

private:
	MainWindow *m_parent;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
	~MainWindow();
	void process();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
	TestOpenCLThread *m_processThread;
    GLWidget* playerWidget;
	GLHiddenWidget *m_hiddenGl;
    QOpenGLContext* m_sharecontext=NULL;
    QDockWidget *stackedLayout;
};

#endif
