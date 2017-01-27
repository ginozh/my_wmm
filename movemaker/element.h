#ifndef ELEMENT_H
#define ELEMENT_H

#include <QWidget>
#include <QVBoxLayout>
#include "image.h"
#include "comm.h"
extern "C"{
#include "ffmpeg.h"
}
typedef struct to_buffer FfmpegBuffer;
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Element : public QWidget
{
    Q_OBJECT

public:
    Element(QWidget *parent, const GlobalImageAttr& newGlobalImageAttr);
    ~Element();
private:
    QVBoxLayout *m_elementLayout;
    Image *m_pimage;
    QString m_qsImageName; //图片路径
public: // uncomplete
    FfmpegBuffer m_fbOriFile;        // 原始文件(图片、视频)
    QByteArray   m_baOriFile;
    FfmpegBuffer m_fbScaleFile;      //保存缩放(像素缩小)后文件内容
    FfmpegBuffer m_fbInputScaleFile; //指向缩放(像素缩小)后文件内容,作为下一级的输入(例如生成视频)
    uint8_t* m_pBufferScaleFile;
    size_t m_iOutScaleFile; 

    FfmpegBuffer m_fbScaledVideo;  // 保存缩小后的视频内容
    FfmpegBuffer m_fbInputScaledVideo;  // 指向缩小后的视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pBufferScaledVideo;
    size_t m_iOutScaledVideo; 

    FfmpegBuffer m_fbPanzoomVideo;       // 保存视频内容
    FfmpegBuffer m_fbInputPanzoomVideo;  // 指向视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pBufferPanzoomVideo;
    size_t m_iOutPanzoomVideo; 

    FfmpegBuffer m_fbTransitionVideo;  // 保存视频内容
    FfmpegBuffer m_fbInputTransitionVideo;  // 指向视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pBufferTransitionVideo;
    size_t m_iOutTransitionVideo; 

    // final video file
    FfmpegBuffer m_fbFileScaleFile;      //保存缩放(像素缩小)后文件内容
    FfmpegBuffer m_fbFileInputScaleFile; //指向缩放(像素缩小)后文件内容,作为下一级的输入(例如生成视频)
    uint8_t* m_pFileBufferScaleFile;
    size_t m_iFileOutScaleFile; 

    FfmpegBuffer m_fbFileScaledVideo;  // 保存缩小后的视频内容
    FfmpegBuffer m_fbFileInputScaledVideo;  // 指向缩小后的视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pFileBufferScaledVideo;
    size_t m_iFileOutScaledVideo; 

    FfmpegBuffer m_fbFilePanzoomVideo;       // 保存视频内容
    FfmpegBuffer m_fbFileInputPanzoomVideo;  // 指向视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pFileBufferPanzoomVideo;
    size_t m_iFileOutPanzoomVideo; 

    FfmpegBuffer m_fbFileTransitionVideo;  // 保存视频内容
    FfmpegBuffer m_fbFileInputTransitionVideo;  // 指向视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pFileBufferTransitionVideo;
    size_t m_iFileOutTransitionVideo; 
    //end final video file
    QLabel* m_musicLabel;
};
//! [0]

#endif // ELEMENT_H
