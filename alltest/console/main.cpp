#include <QCoreApplication>
#include <QtCore>

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent = 0) : QObject(parent) {}

public slots:
    void run()
    {
        // Do processing here
        QString b = A();
		qDebug() << "1 b: " << b;
        change();
		qDebug() << "2 b: " << b;
        emit finished();
    }
private:
    QString a;
    QString A(){
        a = "aaaaaaaaa";
        return a;
    }
    void change(){
        a = "bbbbbb";
    }
signals:
    void finished();
};
#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	// Task parented to the application so that it
    // will be deleted by the application.
    Task *task = new Task(&a);

    // This will cause the application to exit when
    // the task signals finished.    
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));


    return a.exec();
}
