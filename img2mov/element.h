#ifndef ELEMENT_H
#define ELEMENT_H

#include <QWidget>
#include <QVBoxLayout>
#include "image.h"
extern "C"{
#include "ffmpeg.h"
}
typedef struct to_buffer FfmpegBuffer;

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
//! [0]
class Element : public QWidget
{
    Q_OBJECT

public:
    Element(QWidget *parent = 0, const QString& image=tr(""));
    ~Element();
    const QString& getImageName(){return m_qsImageName;}
    void unselectedImage(){if(m_pimage){m_pimage->unselectedImage();}}
signals:
    void insertImage();
    void selectedImageSignal();
public slots:
    void insert();
    void selectedImage();
private:
    QVBoxLayout *m_elementLayout;
    Image *m_pimage;
    QString m_qsImageName; //图片路径
public: // uncomplete
    FfmpegBuffer m_fbOriFile; // 原始文件(图片、视频)
    QByteArray   m_baOriFile;
    FfmpegBuffer m_fbScaleFile; // 缩放(像素缩小)文件
    FfmpegBuffer m_fbInputScaleFile; // 缩放(像素缩小)文件
    FfmpegBuffer m_fbScaleAniVideo; // 缩放后的动画视频
    FfmpegBuffer m_fbInputAniVideo; // 缩放后的动画视频
    uint8_t* m_pBufferScaleFile;
    size_t m_iOutScaleFile; 
    uint8_t* m_pBufferVideo;
    size_t m_iOutVidow; 
};
//! [0]

#endif // ELEMENT_H
