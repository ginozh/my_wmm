#include <QtWidgets>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>
#include <QLineEdit>

#include "mainwindow.h"
LineEdit::LineEdit(QWidget *parent, DiagramScene* tscene)
    //: QLabel(parent)
    : QLineEdit(parent)
    , scene(tscene)
      //, m_idx(idx)
{
    setPlaceholderText("A[Enter text here]");
    setReadOnly(true);
    //connect(this, SIGNAL(selectTextSignal(int)), parentWidget(), SLOT(selectedText()));
    //connect(this, SIGNAL(selectTextSignal()), parentWidget(), SLOT(selectedText()));
}
void LineEdit::mousePressEvent(QMouseEvent *event)
{
    //emit selectTextSignal(m_idx);
    //QMessageBox::information(this, "info", QString(tr("textlabel")));
    emit selectedTextSignal(placeholderText());
    //QLabel::mouseDoubleClickEvent(event);
    //QLabel::mousePressEvent(event);
    //QLineEdit::mousePressEvent(event);
}
void LineEdit::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("LineEdit::keyPressEvent")));
    QCoreApplication::sendEvent(scene, keyEvent);
    //QLineEdit::keyPressEvent(keyEvent);
}
bool LineEdit::eventFilter( QObject *o, QEvent *e ) 
{
#if 1
	if ( e->type() == QEvent::KeyPress ) {
        return QObject::eventFilter(o, e);
		//return QGraphicsScene::eventFilter( o, e );
	} else {
		// 标准事件处理
		return QLineEdit::eventFilter( o, e );
	}
#endif
}
DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    DiagramTextItem *textItem;
    textItem = new DiagramTextItem();
    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    textItem->setFont(font);
    textItem->setPlainText("Qt Everywhere!");
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setDefaultTextColor(Qt::black);
    textItem->setZValue(1000.0);
    textItem->setPos(50, 300);

    addItem(textItem);

    curtextItem = textItem;
}
#if 0
bool DiagramScene::eventFilter( QObject *o, QEvent *e ) 
{
	if ( e->type() == QEvent::KeyPress ) {
		// 对于键被按下进行特殊处理
#if 0
		QKeyEvent *k = (QKeyEvent *)e;
        if(curtextItem)
        {
            DiagramTextItem *textItem = curtextItem;
            textItem->setFocus(Qt::OtherFocusReason);
#if 0
            textItem->setActive(true);
            textItem->setVisible(true);
            textItem->setFocus(Qt::OtherFocusReason);
            textItem->grabKeyboard();
            //textItem->setSelected(true);
            //emit textItem->selectedChange(textItem);
#endif
            bool b = sendEvent(curtextItem, k);
            //QMessageBox::information(NULL, "info", QString(tr("DiagramScene::eventFilter key: %1 ok: %2")).arg(k->text()).arg(b));
            qDebug( "iagramScene::eventFilter key press %d", k->key() );
        }
		//qDebug( "Ate key press %d", k->key() );
		//return true; // 吃掉了这个事件
#endif
		return QGraphicsScene::eventFilter( o, e );
	} else {
		// 标准事件处理
		return QGraphicsScene::eventFilter( o, e );
	}
}
#endif
#if 1
void DiagramScene::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("MainWindow::keyPressEvent")));
    if(curtextItem)
    {
        DiagramTextItem *textItem = curtextItem;
        textItem->setFocus(Qt::OtherFocusReason);
        textItem->setCursor(Qt::IBeamCursor);
        QTextCursor textCursor=textItem->textCursor();
        textCursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        textItem->setTextCursor(textCursor);
        bool b = sendEvent(curtextItem, keyEvent);
        //QMessageBox::information(NULL, "info", QString(tr("DiagramScene::eventFilter key: %1 ok: %2")).arg(k->text()).arg(b));
    }
    else
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}
#endif
void DiagramScene::activeVideoText(const QString& oritxt)
{
    DiagramTextItem *textItem = curtextItem;
    textItem->setFocus(Qt::OtherFocusReason);
    if(textItem->toPlainText().isEmpty())
    {
        textItem->setPlainText(oritxt);
    }
}

void DiagramTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
    o->state |= QStyle::State_HasFocus|QStyle::State_Selected;
    //o->state &= QStyle::State_UpArrow;
    QGraphicsTextItem::paint(painter, o, widget);
}
MainWindow::MainWindow()
{
    scene = new DiagramScene(this);
    QGraphicsView *graphicsView = new QGraphicsView(scene);

    QGraphicsVideoItem* videoItem = new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(500, 480));
    scene->addItem(videoItem);

    //installEventFilter( scene );
#if 0
    textItem = new DiagramTextItem();
    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    textItem->setFont(font);
    textItem->setPlainText("Qt Everywhere!");
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setDefaultTextColor(Qt::black);
    textItem->setZValue(1000.0);
    textItem->setPos(50, 300);

    scene->addItem(textItem);
#endif
    QWidget *tabAnimations = new QWidget;
    //tabAnimations->setMinimumHeight(200);
    //tabAnimations->setMaximumHeight(200);

    QHBoxLayout* hboxAnimations = new QHBoxLayout;
    hboxAnimations->addWidget(graphicsView);
    {
        QLabel *lbl = new QLabel(tr("Text duration:"));

        hboxAnimations->addWidget(lbl);
    }
    {
        QComboBox* m_durationTextCombo = new QComboBox();
        //m_durationTextCombo->setEditable(true);
        for (int i = 2; i < 30; i = i + 2)
            m_durationTextCombo->addItem(QString().setNum(i));

        hboxAnimations->addWidget(m_durationTextCombo);
    }
    {
        QToolButton *addPhotos = new QToolButton();
        addPhotos->setText("Add photos");
        hboxAnimations->addWidget(addPhotos);
        removeEventFilter(addPhotos);
    }
    {
        m_lineEdit =  new LineEdit(this,scene);
        hboxAnimations->addWidget(m_lineEdit);
        //m_lineEdit->setPlaceholderText("A[Enter text here]");
        //m_lineEdit->setReadOnly(true);
        //removeEventFilter(m_lineEdit);
        //m_lineEdit->installEventFilter( this );
    }

    connect(m_lineEdit, SIGNAL(selectedTextSignal(const QString&)), scene,
            SLOT(activeVideoText(const QString&)));

    tabAnimations->setLayout(hboxAnimations);
    setCentralWidget(tabAnimations);
    setWindowTitle(tr("Character Map"));
}
//! [6]

//! [7]
void MainWindow::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentItem = styleCombo->currentText();
    styleCombo->clear();
//! [7]

//! [8]
    QString style;
    foreach (style, fontDatabase.styles(font.family()))
        styleCombo->addItem(style);

    int styleIndex = styleCombo->findText(currentItem);

    if (styleIndex == -1)
        styleCombo->setCurrentIndex(0);
    else
        styleCombo->setCurrentIndex(styleIndex);
}
//! [8]

void MainWindow::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = sizeCombo->currentText();
    sizeCombo->blockSignals(true);
    sizeCombo->clear();

    int size;
    if(fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font))) {
        foreach(size, QFontDatabase::standardSizes()) {
            sizeCombo->addItem(QVariant(size).toString());
            sizeCombo->setEditable(true);
        }

    } else {
        foreach(size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font))) {
            sizeCombo->addItem(QVariant(size).toString());
            sizeCombo->setEditable(false);
        }
    }

    sizeCombo->blockSignals(false);

    int sizeIndex = sizeCombo->findText(currentSize);

    if(sizeIndex == -1)
        sizeCombo->setCurrentIndex(qMax(0, sizeCombo->count() / 3));
    else
        sizeCombo->setCurrentIndex(sizeIndex);
}

//! [9]
void MainWindow::insertCharacter(const QString &character)
{
    lineEdit->insert(character);
}
//! [9]

//! [10]
#ifndef QT_NO_CLIPBOARD
void MainWindow::updateClipboard()
{
//! [11]
    clipboard->setText(lineEdit->text(), QClipboard::Clipboard);
//! [11]
    clipboard->setText(lineEdit->text(), QClipboard::Selection);
}
#endif
//! [10]
#if 0
void MainWindow::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("MainWindow::keyPressEvent")));
    QMainWindow::keyPressEvent(keyEvent);
}
#endif
#if 0
bool MainWindow::eventFilter( QObject *o, QEvent *e ) 
{
	if ( e->type() == QEvent::KeyPress ) {
#if 0
        if (m_lineEdit==o)
        {
            //return true;
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
            //QMessageBox::information(NULL, "info", QString(tr("MainWindow::eventFilter")));
            QCoreApplication::sendEvent(scene, keyEvent);
            return true;
            //bool b = sendEvent(scene, keyEvent);
            //return QObject::eventFilter(scene, e);
        }
        else
        {
            return QObject::eventFilter(o, e);
        }
#endif 
		return QMainWindow::eventFilter( o, e );
		//return QGraphicsScene::eventFilter( o, e );
	} else {
		// 标准事件处理
		return QMainWindow::eventFilter( o, e );
	}
}
#endif
