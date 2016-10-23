#include "waveform.h"
//#include "mainwidget.h"
#include "spectrum.h"
//#include "engine.h"
#include "utils.h"
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QMessageBox>
// storm 
#define PAINT_EVENT_TRACE
#ifdef PAINT_EVENT_TRACE
#   define WAVEFORM_PAINT_DEBUG qDebug()
#else
#   define WAVEFORM_PAINT_DEBUG nullDebug()
#endif

Waveform::Waveform(QWidget *parent)
    :   QWidget(parent)
    ,   m_bufferPosition(0)
    ,   m_bufferLength(0)
    ,   m_audioPosition(0)
    ,   m_active(false)
    ,   m_tileLength(0)
    ,   m_tileArrayStart(0)
    ,   m_windowPosition(0)
    ,   m_windowLength(0)

    ,   m_audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice())
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumHeight(50);
    setMaximumHeight(50);

    QString fileName(tr("C:/qtproject/alltest/spectrum/debug/longlovechina1.wav"));
    //QString fileName(tr("C:/QtProjects/QtExamples/multimedia/spectrum/debug/longlovechina1.wav"));
    if(!loadFile(fileName))
    {
        QMessageBox::information(NULL, "error","loadfile");
    }
    m_format = m_file->fileFormat();
    QAudioFormat format = m_format;

    initialize(format, WaveformTileLength, WaveformWindowDuration);

}
bool Waveform::drawWave(int processedUSecs)
{
    const int    LevelWindowUs          = 100 * 1000;
    m_levelBufferLength = audioLength(m_format, LevelWindowUs);
    m_spectrumBufferLength = SpectrumLengthSamples *
        (m_format.sampleSize() / 8) * m_format.channelCount();

    const qint64 playPosition = audioLength(m_format, processedUSecs/*800442*/ /*m_audioOutput->processedUSecs()*/); //播放微妙数
    //const qint64 playPosition = audioLength(m_format, 360000/*800442*/ /*m_audioOutput->processedUSecs()*/); //播放微妙数
    //const qint64 playPosition = audioLength(m_format, 800442 /*m_audioOutput->processedUSecs()*/); //播放微妙数
    //setPlayPosition(qMin(bufferLength(), playPosition));
    audioPositionChanged(qMin(bufferLength(), playPosition));
    const qint64 levelPosition = playPosition - m_levelBufferLength;
    const qint64 spectrumPosition = playPosition - m_spectrumBufferLength;
    const qint64 readPos = qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readEnd = qMin(m_analysisFile->size(), qMax(levelPosition + m_levelBufferLength, spectrumPosition + m_spectrumBufferLength));
    const qint64 readLen = readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    qDebug() << "Waveform::audioNotify [0]"
        << "processedUSecs" << processedUSecs
        << "bufferLength" << bufferLength()
        << "playPosition" << playPosition
        << "levelPosition" << levelPosition
        << "m_levelBufferLength" << m_levelBufferLength
        << "m_spectrumBufferLength" << m_spectrumBufferLength
        << "spectrumPosition" << spectrumPosition
        << "analysisFileSize" << m_analysisFile->size()
        << "readPos" << readPos
        << "readLen" << readLen;
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);

    return true;
}
bool Waveform::audioNotify()
{
#if 1
    /*
    drawWave(240000);
    drawWave(320000);
    drawWave(400000);
    drawWave(520000);
    drawWave(600000);
    drawWave(720000);
    drawWave(800000);
    drawWave(800442);*/
    for(int processedUSecs=0; processedUSecs<=800000; processedUSecs+=80000)
        drawWave(processedUSecs);
    return true;
#else
    {
    const int    LevelWindowUs          = 100 * 1000;
    m_levelBufferLength = audioLength(m_format, LevelWindowUs);
    m_spectrumBufferLength = SpectrumLengthSamples *
        (m_format.sampleSize() / 8) * m_format.channelCount();

    const qint64 playPosition = audioLength(m_format, 240000/*800442*/ /*m_audioOutput->processedUSecs()*/); //播放微妙数
    //const qint64 playPosition = audioLength(m_format, 360000/*800442*/ /*m_audioOutput->processedUSecs()*/); //播放微妙数
    //const qint64 playPosition = audioLength(m_format, 800442 /*m_audioOutput->processedUSecs()*/); //播放微妙数
    //setPlayPosition(qMin(bufferLength(), playPosition));
    setWindowPosition(42336);
    const qint64 levelPosition = playPosition - m_levelBufferLength;
    const qint64 spectrumPosition = playPosition - m_spectrumBufferLength;
    const qint64 readPos = 24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readEnd = 141198; //qMin(m_analysisFile->size(), qMax(levelPosition + m_levelBufferLength, spectrumPosition + m_spectrumBufferLength));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    qDebug() << "Waveform::audioNotify [0]"
        //<< "processedUSecs" << m_audioOutput->processedUSecs()
        << "bufferLength" << bufferLength()
        << "playPosition" << playPosition
        << "levelPosition" << levelPosition
        << "m_levelBufferLength" << m_levelBufferLength
        << "m_spectrumBufferLength" << m_spectrumBufferLength
        << "spectrumPosition" << spectrumPosition
        << "analysisFileSize" << m_analysisFile->size()
        << "readPos" << readPos
        << "readLen" << readLen;
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }
#endif


    {
    setWindowPosition(49376);
    const qint64 readPos = 31744;//24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }


    {
    setWindowPosition(52962);
    const qint64 readPos = 52928;//24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }


    {
    setWindowPosition(52962);
    const qint64 readPos = 74080;//24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }


    {
    setWindowPosition(52962);
    const qint64 readPos = 88192;//24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }


    {
    setWindowPosition(52962);
    const qint64 readPos = 109376;//24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }


    {
    setWindowPosition(52962);
    const qint64 readPos = 123488;//24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }


    {
    setWindowPosition(52962);
    const qint64 readPos = 123552;//24704; //qMax(qint64(0), qMin(levelPosition, spectrumPosition));
    const qint64 readLen = 105824;//141198; //readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
        m_buffer.resize(readLen);
        m_bufferPosition = readPos;
        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
        qDebug() << "Waveform::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
    } else {
        qDebug() << "Waveform::audioNotify [2]" << "file seek error";
    }
    bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
    }

    return true;
}
bool Waveform::loadFile(const QString &fileName)
{
    bool result = false;
    m_file = new WavFile(this);
    if (m_file->open(fileName)) {
        if (isPCMS16LE(m_file->fileFormat())) {
            //result = initialize();
            result=true;
        } else {
            QMessageBox::information(this, "error", QString(tr("Audio format not supported")));
            emit errorMessage(tr("Audio format not supported"),
                              formatToString(m_file->fileFormat()));
        }
    } else {
        QMessageBox::information(this, "error", QString(tr("Could not open file")));
        emit errorMessage(tr("Could not open file"), fileName);
    }
    if (result) {
        m_analysisFile = new WavFile(this);
        m_analysisFile->open(fileName);
    }
    return result;
}
qint64 Waveform::bufferLength() const
{
    //return m_file ? m_file->size() : m_bufferLength;
    return m_file->size();
}

Waveform::~Waveform()
{
    deletePixmaps();
}

void Waveform::paintEvent(QPaintEvent * /*event*/)
{//return QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (m_active) 
    {
        WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent"
                             << "windowPosition" << m_windowPosition
                             << "windowLength" << m_windowLength;
        qint64 pos = m_windowPosition;
        const qint64 windowEnd = m_windowPosition + m_windowLength;
        int destLeft = 0;
        int destRight = 0;
        while (pos < windowEnd) {
            const TilePoint point = tilePoint(pos);
            WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "pos" << pos
                                 << "tileIndex" << point.index
                                 << "positionOffset" << point.positionOffset
                                 << "pixelOffset" << point.pixelOffset;

            if (point.index != NullIndex) {
                const Tile &tile = m_tiles[point.index];
                if (tile.painted) {
                    const qint64 sectionLength = qMin((m_tileLength - point.positionOffset),
                                                     (windowEnd - pos));
                    Q_ASSERT(sectionLength > 0);
                    const int sourceRight = tilePixelOffset(point.positionOffset + sectionLength);
                    destRight = windowPixelOffset(pos - m_windowPosition + sectionLength);

                    QRect destRect = rect();
                    destRect.setLeft(destLeft);
                    destRect.setRight(destRight);

                    QRect sourceRect(QPoint(), m_pixmapSize);
                    sourceRect.setLeft(point.pixelOffset);
                    sourceRect.setRight(sourceRight);

                    WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "tileIndex" << point.index
                                         << "source" << point.pixelOffset << sourceRight
                                         << "dest" << destLeft << destRight;

                    painter.drawPixmap(destRect, *tile.pixmap, sourceRect);

                    destLeft = destRight;

                    if (point.index < m_tiles.count()) {
                        pos = tilePosition(point.index + 1);
                        WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "pos ->" << pos;
                    } else {
                        // Reached end of tile array
                        WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "reached end of tile array";
                        break;
                    }
                } else {
                    // Passed last tile which is painted
                    WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "tile" << point.index << "not painted";
#if 0
                    if (point.index < m_tiles.count()) {
                        pos = tilePosition(point.index + 1);
                        WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "pos ->" << pos;
                    } else {
                        // Reached end of tile array
                        WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "reached end of tile array";
                        break;
                    }
#else
                    break;
#endif
                }
            } else {
                // pos is past end of tile array
                WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "pos" << pos << "past end of tile array";
                break;
            }
        }
        WAVEFORM_PAINT_DEBUG << "Waveform::paintEvent" << "final pos" << pos << "final x" << destRight;
        //m_active=false;
    }
}

void Waveform::resizeEvent(QResizeEvent *event)
{
    if (event->size() != event->oldSize())
    {
        createPixmaps(event->size());
        m_active=true;
        audioNotify();
    }
}

void Waveform::initialize(const QAudioFormat &format, qint64 audioBufferSize, qint64 windowDurationUs)
{
    WAVEFORM_DEBUG << "Waveform::initialize"
                   << "audioBufferSize" << audioBufferSize
                   << "windowDurationUs" << windowDurationUs;

    reset();

    m_format = format;

    // Calculate tile size
    m_tileLength = audioBufferSize;

    // Calculate window size
    m_windowLength = audioLength(m_format, windowDurationUs);

    // Calculate number of tiles required
    int nTiles;
    if (m_tileLength > m_windowLength) {
        nTiles = 2;
    } else {
        nTiles = m_windowLength / m_tileLength + 1;
        if (m_windowLength % m_tileLength)
            ++nTiles;
    }

    WAVEFORM_DEBUG << "Waveform::initialize"
                   << "tileLength" << m_tileLength
                   << "windowLength" << m_windowLength
                   << "nTiles" << nTiles;

    m_pixmaps.fill(0, nTiles);
    m_tiles.resize(nTiles);

    createPixmaps(rect().size());

    m_active = true;
}

void Waveform::reset()
{
    WAVEFORM_DEBUG << "Waveform::reset";

    m_bufferPosition = 0;
    m_buffer = QByteArray();
    m_audioPosition = 0;
    m_format = QAudioFormat();
    m_active = false;
    deletePixmaps();
    m_tiles.clear();
    m_tileLength = 0;
    m_tileArrayStart = 0;
    m_windowPosition = 0;
    m_windowLength = 0;
}

void Waveform::bufferChanged(qint64 position, qint64 length, const QByteArray &buffer)
{
    WAVEFORM_DEBUG << "Waveform::bufferChanged"
                   << "audioPosition" << m_audioPosition
                   << "bufferPosition" << position
                   << "bufferLength" << length;
    m_bufferPosition = position;
    m_bufferLength = length;
    m_buffer = buffer;
    paintTiles();
}

void Waveform::audioPositionChanged(qint64 position)
{
    WAVEFORM_DEBUG << "Waveform::audioPositionChanged"
                   << "audioPosition" << position
                   << "bufferPosition" << m_bufferPosition
                   << "bufferLength" << m_bufferLength;

    if (position >= m_bufferPosition) {
        if (position + m_windowLength > m_bufferPosition + m_bufferLength)
            position = qMax(qint64(0), m_bufferPosition + m_bufferLength - m_windowLength);
        m_audioPosition = position;
        setWindowPosition(position);
    }
}

void Waveform::deletePixmaps()
{
    QPixmap *pixmap;
    foreach (pixmap, m_pixmaps)
        delete pixmap;
    m_pixmaps.clear();
}

void Waveform::createPixmaps(const QSize &widgetSize)
{
    m_pixmapSize = widgetSize;
    m_pixmapSize.setWidth(qreal(widgetSize.width()) * m_tileLength / m_windowLength);

    WAVEFORM_DEBUG << "Waveform::createPixmaps"
                   << "widgetSize" << widgetSize
                   << "pixmapSize" << m_pixmapSize;

    Q_ASSERT(m_tiles.count() == m_pixmaps.count());

    // (Re)create pixmaps
    for (int i=0; i<m_pixmaps.size(); ++i) {
        delete m_pixmaps[i];
        m_pixmaps[i] = 0;
        m_pixmaps[i] = new QPixmap(m_pixmapSize);
    }

    // Update tile pixmap pointers, and mark for repainting
    for (int i=0; i<m_tiles.count(); ++i) {
        m_tiles[i].pixmap = m_pixmaps[i];
        m_tiles[i].painted = false;
    }
}

void Waveform::setWindowPosition(qint64 position)
{
    WAVEFORM_DEBUG << "Waveform::setWindowPosition"
                   << "old" << m_windowPosition << "new" << position
                   << "tileArrayStart" << m_tileArrayStart;

    const qint64 oldPosition = m_windowPosition;
    m_windowPosition = position;

    if ((m_windowPosition >= oldPosition) &&
        (m_windowPosition - m_tileArrayStart < (m_tiles.count() * m_tileLength))) {
        // Work out how many tiles need to be shuffled
        const qint64 offset = m_windowPosition - m_tileArrayStart;
        const int nTiles = offset / m_tileLength;
        shuffleTiles(nTiles);
    } else {
        resetTiles(m_windowPosition);
    }

    if (!paintTiles() && m_windowPosition != oldPosition)
        update();
}

qint64 Waveform::tilePosition(int index) const
{
    return m_tileArrayStart + index * m_tileLength;
}

Waveform::TilePoint Waveform::tilePoint(qint64 position) const
{
    TilePoint result;
    if (position >= m_tileArrayStart) {
        const qint64 tileArrayEnd = m_tileArrayStart + m_tiles.count() * m_tileLength;
        if (position < tileArrayEnd) {
            const qint64 offsetIntoTileArray = position - m_tileArrayStart;
            result.index = offsetIntoTileArray / m_tileLength;
            Q_ASSERT(result.index >= 0 && result.index <= m_tiles.count());
            result.positionOffset = offsetIntoTileArray % m_tileLength;
            result.pixelOffset = tilePixelOffset(result.positionOffset);
            Q_ASSERT(result.pixelOffset >= 0 && result.pixelOffset <= m_pixmapSize.width());
        }
    }

    return result;
}

int Waveform::tilePixelOffset(qint64 positionOffset) const
{
    Q_ASSERT(positionOffset >= 0 && positionOffset <= m_tileLength);
    const int result = (qreal(positionOffset) / m_tileLength) * m_pixmapSize.width();
    return result;
}

int Waveform::windowPixelOffset(qint64 positionOffset) const
{
    Q_ASSERT(positionOffset >= 0 && positionOffset <= m_windowLength);
    const int result = (qreal(positionOffset) / m_windowLength) * rect().width();
    return result;
}

bool Waveform::paintTiles()
{
    WAVEFORM_DEBUG << "Waveform::paintTiles";
    bool updateRequired = false;

    for (int i=0; i<m_tiles.count(); ++i) {
        const Tile &tile = m_tiles[i];
        if (!tile.painted) {
            const qint64 tileStart = m_tileArrayStart + i * m_tileLength;
            const qint64 tileEnd = tileStart + m_tileLength;
            if (m_bufferPosition <= tileStart && m_bufferPosition + m_bufferLength >= tileEnd) {
                paintTile(i);
                updateRequired = true;
            }
        }
    }

    if (updateRequired)
        update();

    return updateRequired;
}

void Waveform::paintTile(int index)
{
    const qint64 tileStart = m_tileArrayStart + index * m_tileLength;

    WAVEFORM_DEBUG << "Waveform::paintTile"
                   << "index" << index
                   << "bufferPosition" << m_bufferPosition
                   << "bufferLength" << m_bufferLength
                   << "start" << tileStart
                   << "end" << tileStart + m_tileLength;

    Q_ASSERT(m_bufferPosition <= tileStart);
    Q_ASSERT(m_bufferPosition + m_bufferLength >= tileStart + m_tileLength);

    Tile &tile = m_tiles[index];
    Q_ASSERT(!tile.painted);

    const qint16* base = reinterpret_cast<const qint16*>(m_buffer.constData());
    const qint16* buffer = base + ((tileStart - m_bufferPosition) / 2);
    const int numSamples = m_tileLength / (2 * m_format.channelCount());

    QPainter painter(tile.pixmap);

    painter.fillRect(tile.pixmap->rect(), Qt::black);

    QPen pen(Qt::white);
    painter.setPen(pen);

    // Calculate initial PCM value
    qint16 previousPcmValue = 0;
    if (buffer > base)
        previousPcmValue = *(buffer - m_format.channelCount());

    // Calculate initial point
    const qreal previousRealValue = pcmToReal(previousPcmValue);
    const int originY = ((previousRealValue + 1.0) / 2) * m_pixmapSize.height();
    const QPoint origin(0, originY);

    QLine line(origin, origin);
    //QMessageBox::information(this, "info", QString(tr("paint title start")));
    for (int i=0; i<numSamples; ++i) {
        const qint16* ptr = buffer + i * m_format.channelCount();

        const int offset = reinterpret_cast<const char*>(ptr) - m_buffer.constData();
        Q_ASSERT(offset >= 0);
        Q_ASSERT(offset < m_bufferLength);
        Q_UNUSED(offset);

        const qint16 pcmValue = *ptr;
        const qreal realValue = pcmToReal(pcmValue);

        const int x = tilePixelOffset(i * 2 * m_format.channelCount());
        const int y = ((realValue + 1.0) / 2) * m_pixmapSize.height();

        line.setP2(QPoint(x, y));
        painter.drawLine(line);
        line.setP1(line.p2());
    }
    //QMessageBox::information(this, "info", QString(tr("paint title end")));

    tile.painted = true;
}

void Waveform::shuffleTiles(int n)
{
    WAVEFORM_DEBUG << "Waveform::shuffleTiles" << "n" << n;

    while (n--) {
        Tile tile = m_tiles.first();
        tile.painted = false;
        m_tiles.erase(m_tiles.begin());
        m_tiles += tile;
        m_tileArrayStart += m_tileLength;
    }

    WAVEFORM_DEBUG << "Waveform::shuffleTiles" << "tileArrayStart" << m_tileArrayStart;
}

void Waveform::resetTiles(qint64 newStartPos)
{
    WAVEFORM_DEBUG << "Waveform::resetTiles" << "newStartPos" << newStartPos;
    QVector<Tile>::iterator i = m_tiles.begin();
    for ( ; i != m_tiles.end(); ++i)
        i->painted = false;

    m_tileArrayStart = newStartPos;
}

