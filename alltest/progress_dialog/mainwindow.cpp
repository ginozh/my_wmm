#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include "mainwindow.h"
#include <QFontMetrics>
#include <QFont>
#include <QToolButton>
#include <QMenu>
#include <QTest> 
MainWindow::MainWindow()
{
    setWindowTitle(tr("progress"));

    QWidget *tabAnimations = new QWidget;
    QHBoxLayout *hboxAnimations = new QHBoxLayout;
    {
        lblDurationTrans = new QLabel(QString(tr("test")));
        hboxAnimations->addWidget(lblDurationTrans);
    }
    {
        QToolButton* fontColorToolButton = new QToolButton;
        hboxAnimations->addWidget(fontColorToolButton);
        connect(fontColorToolButton, SIGNAL(clicked()), this, SLOT(testProgress()));
    }
    tabAnimations->setLayout(hboxAnimations);
    setCentralWidget(tabAnimations);
}
void MainWindow::testProgress()
{
    ProgressDialog *progressDialog = new ProgressDialog("test" , this);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    //progressDialog->setWindowFlags(Qt::Widget); 
    //connect(progressDialog, &QProgressDialog::canceled, this, &HttpWindow::cancelDownload);
    //connect(this, &ElementsEdit::saveMoiveProgress, progressDialog, &ProgressDialog::saveMoiveProgress);
    connect(this, &MainWindow::saveMoiveFinish, progressDialog, &ProgressDialog::hide);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->show();

    for (int i = 0; i < 10; ++i)
    {
        QTest::qSleep(500);
        emit saveMoiveProgress(i+1, 10+1);
        qApp->processEvents();
    }
    emit saveMoiveProgress(10+1, 10+1);
    emit saveMoiveFinish();
    progressDialog->deleteLater();

}
//! [6]

ProgressDialog::ProgressDialog(const QString &qsFileName, QWidget *parent)
  : QProgressDialog(parent)
{
    setWindowTitle(tr("Movie Maker"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setLabelText(tr("Saving Movie %1.").arg(qsFileName));
    setMinimum(0);
    setValue(0);
    setMinimumDuration(0);
}

void ProgressDialog::saveMoiveProgress(qint64 bytesRead, qint64 totalBytes)
{
    setMaximum(totalBytes);
    setValue(bytesRead);
}
