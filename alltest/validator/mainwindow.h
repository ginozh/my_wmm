#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QValidator>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class SpinBox : public QDoubleSpinBox 
{
	Q_OBJECT
public:
	explicit SpinBox( QWidget *parent = 0)
		: QDoubleSpinBox (parent)
	{
	}
signals:
    void textChangedSignal(double);
protected:
    //void leaveEvent(QEvent *event)
    void focusOutEvent(QFocusEvent*)
    {
        emit textChangedSignal(value());
    }
#if 0
    void setRange(double minimum, double maximum)
    {
        m_minimum = minimum;
        m_maximum = maximum;
        QDoubleSpinBox::setRange(minimum, maximum);
    }
#if 0
	bool acceptableInput() const
    {
        double v = value() ;
        if(v>=m_minimum && v<=m_maximum)
            return true;
        else
            return false;

    }    
#endif
private:
    double m_minimum, m_maximum;
#endif
};
class lvalidator : public QValidator
{
	Q_OBJECT
public:
	explicit lvalidator(QObject *parent = 0)
		: QValidator(parent)
	{
	}
	virtual State validate ( QString & input, int & pos ) const
	{
		if (input.isEmpty())
			return Acceptable;

		bool b;
		int val = input.toInt(&b);

		if ((b == true) && (val > 0) && (val < 76))
		{
			return Acceptable;
		}
		return Intermediate;
	}
};
class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent=NULL);
    void appendLine(QHBoxLayout *hbox);
private slots:
    void boxValueChanged(double);
};

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    //void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
private:
    QLabel *lblDurationTrans;
};
//! [0]

#endif
