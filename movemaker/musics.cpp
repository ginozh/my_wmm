#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QStyle>
#include <QAbstractButton>
#include <QHeaderView>
#include <QToolButton>
#include <QDate>
#include <QDateTime>
#include "musics.h"
#include "comm.h"

Musics::Musics(QWidget *parent)
    : QWidget(parent)
    , m_lblSelectedMusic(new QLabel)
{
    GlobalContext* globalContext = GlobalContext::instance();
    double dFactorX = globalContext->m_dFactorX;
    setFixedWidth(1200*dFactorX);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        layout->addLayout(hbox);
        {
            QLabel* lbl = new QLabel(tr("SELECTED: "));
            hbox->addWidget(lbl);

            // m_lblSelectedMusic->setText(tr("SUMMER DAYS"));
            hbox->addWidget(m_lblSelectedMusic);

            QWidget* nullWidget = new QWidget;
            nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
            hbox->addWidget(nullWidget);
        }
    }
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        hbox->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        layout->addLayout(hbox);
        {
#define OPEN_FILE
#ifdef OPEN_FILE
            QAbstractButton *openButton = new QPushButton(tr("Open..."));
            hbox->addWidget(openButton);

            connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
#endif
        }
        {
            playButton = new QPushButton;
            hbox->addWidget(playButton);

            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
        }
        {
            stopButton = new QPushButton;
            hbox->addWidget(stopButton);

            stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
            connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
        }
        {
            positionSlider = new Slider(Qt::Horizontal);
            hbox->addWidget(positionSlider);

            positionSlider->setRange(0, 0);
            connect(positionSlider, SIGNAL(sliderMoved(int)),
                    this, SLOT(setPosition(int)));
        }
    }
    {
        QLabel* lbl = new QLabel(tr("RESET MUSIC"));
        layout->addWidget(lbl);
    }
    {
        musicTable = new QTableWidget(0, 4);
        layout->addWidget(musicTable);
        musicTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        QStringList labels;
        labels << tr("") << tr("NAME") << tr("TIME") << tr("STYLE");
        musicTable->setHorizontalHeaderLabels(labels);
        musicTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        musicTable->verticalHeader()->hide();
        musicTable->setShowGrid(false);

        // connect(musicTable, &QTableWidget::cellActivated, this, &Window::openFileOfItem);
        int row = musicTable->rowCount();
        musicTable->insertRow(row);
        int col=0;
#define NEW_TABLE_ITEM(var_name, var_label) \
        QTableWidgetItem *var_name = new QTableWidgetItem(tr(var_label)); \
        var_name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); \
        var_name->setFlags(var_name->flags() ^ Qt::ItemIsEditable);  \
        musicTable->setItem(row, col++, var_name);

        NEW_TABLE_ITEM(col1, "null")
        NEW_TABLE_ITEM(col2, "music_name")
        NEW_TABLE_ITEM(col3, "01:25")
        NEW_TABLE_ITEM(col4, "POP")
#undef NEW_TABLE_ITEM
#if 0
        musicTable->setItem(row, 0, "");
        musicTable->setItem(row, 1, "music_name");
        musicTable->setItem(row, 2, "01:25");
        musicTable->setItem(row, 3, "POP");
#endif
    }
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        // hbox->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        layout->addLayout(hbox);
        {
            QLabel* lbl = new QLabel(tr("ADD MY MUSIC"));
            hbox->addWidget(lbl);


            QToolButton* m_boldButton = new QToolButton(this);
            m_boldButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
            m_boldButton->setIcon(QIcon("images/music2.png"));
            hbox->addWidget(m_boldButton);
            connect(m_boldButton, SIGNAL(clicked(bool)),
                    this, SLOT(addMyMusic()));
            // hbox->setAlignment(m_boldButton, Qt::AlignRight|Qt::AlignTop);

            QWidget* nullWidget = new QWidget;
            nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
            hbox->addWidget(nullWidget);
        }
    }
    {
        myMusicTable = new QTableWidget(0, 4);
        layout->addWidget(myMusicTable);
        myMusicTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        QStringList labels;
        labels << tr("") << tr("NAME") << tr("TIME") << tr("STYLE");
        myMusicTable->setHorizontalHeaderLabels(labels);
        myMusicTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        myMusicTable->verticalHeader()->hide();
        myMusicTable->setShowGrid(false);
#if 0
        // connect(myMusicTable, &QTableWidget::cellActivated, this, &Window::openFileOfItem);
        int row = myMusicTable->rowCount();
        myMusicTable->insertRow(row);
        int col=0;
#define NEW_TABLE_ITEM(var_name, var_label) \
        QTableWidgetItem *var_name = new QTableWidgetItem(tr(var_label)); \
        var_name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); \
        var_name->setFlags(var_name->flags() ^ Qt::ItemIsEditable);  \
        myMusicTable->setItem(row, col++, var_name);

        NEW_TABLE_ITEM(col1, "null")
        NEW_TABLE_ITEM(col2, "music_name")
        NEW_TABLE_ITEM(col3, "01:25")
        NEW_TABLE_ITEM(col4, "POP")
#undef NEW_TABLE_ITEM
#endif
    }
    setLayout(layout);

    m_musicMediaPlayer.setNotifyInterval(40); //1秒25帧,即40毫秒通知一次

    connect(&m_musicMediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&m_musicMediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&m_musicMediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
}

Musics::~Musics()
{
}
void Musics::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Music"),QStandardPaths::writableLocation(QStandardPaths::MusicLocation));

    if (!fileName.isEmpty()) {
        m_musicMediaPlayer.setMedia(QUrl::fromLocalFile(fileName));
        playButton->setEnabled(true);
    }
}
void Musics::mediaStateChanged(QMediaPlayer::State state)
{
    qDebug()<<"VideoPlayer::mediaStateChanged state: "<<state;
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    case QMediaPlayer::StoppedState:
        //m_musicMediaPlayer.pause(); // for display image  //uncomplete
    case QMediaPlayer::PausedState:
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
    // controlMusicState();
}
void Musics::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
    switch(m_musicMediaPlayer.state()) {
    case QMediaPlayer::StoppedState:
        m_musicMediaPlayer.pause(); // for display image  //uncomplete
        break;
    default:
        break;
    }
}
void Musics::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
    qDebug()<<"VideoPlayer::durationChanged duration: "<<duration<<" isVideoAvailable: "<<m_musicMediaPlayer.isVideoAvailable();
}
void Musics::setPosition(int position)
{
    qDebug()<<"VideoPlayer::setPosition. position: "<<position;
    m_musicMediaPlayer.setPosition(position); //notice 不会立即生效
    // controlMusicState(position);
}
void Musics::play()
{
    switch(m_musicMediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        m_musicMediaPlayer.pause();
        break;
    default:
        m_musicMediaPlayer.play();
        break;
    }
}
void Musics::stop()
{
    switch(m_musicMediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        m_musicMediaPlayer.stop();
        break;
    default:
        // m_musicMediaPlayer.play();
        break;
    }
}
void Musics::addMyMusic()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Music"),QStandardPaths::writableLocation(QStandardPaths::MusicLocation));

    if (fileName.isEmpty()) {
        return ;
    }
    m_musicMediaPlayer.setMedia(QUrl::fromLocalFile(fileName));
    connect(&m_musicMediaPlayer, SIGNAL(durationChanged(qint64)), this,
            SLOT(musicDurationChanged(qint64)));

    m_qsMusicFullFilename = fileName;
    m_isFirstMusic = true;

}
void Musics::musicDurationChanged(qint64 duration)
{
    if(m_isFirstMusic)
    {
        QFileInfo info(m_qsMusicFullFilename);
        playButton->setEnabled(true);
        m_lblSelectedMusic->setText(info.baseName());
        m_isFirstMusic = false;
        QString qsDuration = QDateTime(QDate::currentDate()).addMSecs(duration).toString("mm:ss");

        int row = myMusicTable->rowCount();
        myMusicTable->insertRow(row);
        int col=0;
#define NEW_TABLE_ITEM(var_name, var_label) \
        QTableWidgetItem *var_name = new QTableWidgetItem(var_label); \
        var_name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); \
        var_name->setFlags(var_name->flags() ^ Qt::ItemIsEditable);  \
        myMusicTable->setItem(row, col++, var_name);

        NEW_TABLE_ITEM(col1, "null")
        NEW_TABLE_ITEM(col2, info.baseName())
        NEW_TABLE_ITEM(col3, qsDuration)
        NEW_TABLE_ITEM(col4, "X")
#undef NEW_TABLE_ITEM
    }
}
