#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QQmlError>
#include <QQuickView>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    void quickViewStatusChanged(QQuickView::Status);
    void sceneGraphError(QQuickWindow::SceneGraphError error, const QString &message);

private:
    QQuickView *m_quickView;
    QObject *m_currentRootObject;
};

#endif
