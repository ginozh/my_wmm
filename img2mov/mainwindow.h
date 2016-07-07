#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QTreeView)

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    QTreeView *m_treeView;
    QTextEdit *m_detailsText;
};

#endif // MAINWINDOW_H
