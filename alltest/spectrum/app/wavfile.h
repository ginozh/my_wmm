#ifndef WAVFILE_H
#define WAVFILE_H

#include <QObject>
#include <QFile>
#include <QAudioFormat>

class WavFile : public QFile
{
public:
    WavFile(QObject *parent = 0);

    using QFile::open;
    bool open(const QString &fileName);
    const QAudioFormat &fileFormat() const;
    qint64 headerLength() const;

private:
    bool readHeader();

private:
    QAudioFormat m_fileFormat;
    qint64 m_headerLength;
};

#endif // WAVFILE_H
