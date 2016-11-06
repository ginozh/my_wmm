#ifndef ELEMENT_H
#define ELEMENT_H

#include <QWidget>
#include <QVBoxLayout>
#include "image.h"
#include "lineedit.h"
#include "musiclabel.h"
#include "comm.h"
extern "C"{
#include "ffmpeg.h"
}
class GraphicsScene;
typedef struct to_buffer FfmpegBuffer;

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE
//! [0]
class Element : public QWidget
{
    Q_OBJECT

public:
    Element(QWidget *parent, const QString& image, GraphicsScene* scene);
    ~Element();
    const QString& getImageName(){return m_qsImageName;}
    void unselectedImage(){if(m_pimage){m_pimage->unselectedImage();}}
    void doFocusImage(){if(m_pimage){m_pimage->doFocusImage();}}
    GlobalVideoAttr* globalVideoAttr(){return m_globalVideoAttr;}
    GlobalAnimationAttr* globalAnimationAttr(){return m_globalAnimationAttr;}
    GlobalTextAttr* globalTextAttr(){return m_globalTextAttr;}
    LineEdit* lineEdit(){return m_lineEdit;}
    MusicLabel* musicLabel(){return m_musicLabel;}
    void setValid(bool bValid){m_bValid = bValid;}
    bool valid(){return m_bValid;}
signals:
    void insertImage();
    void removeImage();
    void selectedImageSignal(QWidget*);
    void selectedTextSignal(const QString&);
public slots:
    void insert();
    void remove();
    void selectedImage();
    void selectedText(const QString&);
private:
    QVBoxLayout *m_elementLayout;
    Image *m_pimage;
    QString m_qsImageName; //图片路径
    bool m_bValid; //被flowlayout移除时设置false, 生成ass时才不会被引用
public: // uncomplete
    FfmpegBuffer m_fbOriFile;        // 原始文件(图片、视频)
    QByteArray   m_baOriFile;
    FfmpegBuffer m_fbScaleFile;      //保存缩放(像素缩小)后文件内容
    FfmpegBuffer m_fbInputScaleFile; //指向缩放(像素缩小)后文件内容,作为下一级的输入(例如生成视频)
    uint8_t* m_pBufferScaleFile;
    size_t m_iOutScaleFile; 

    FfmpegBuffer m_fbScaleAniVideo;  // 保存缩小后的视频内容
    FfmpegBuffer m_fbInputAniVideo;  // 指向缩小后的视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pBufferVideo;
    size_t m_iOutVideo; 

    FfmpegBuffer m_fbPanzoomVideo;       // 保存视频内容
    FfmpegBuffer m_fbInputPanzoomVideo;  // 指向视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pBufferPanzoomVideo;
    size_t m_iOutPanzoomVideo; 

    FfmpegBuffer m_fbTransitionVideo;  // 保存视频内容
    FfmpegBuffer m_fbInputTransitionVideo;  // 指向视频内容，作为下一级的输入(例如生成特效视频)
    uint8_t* m_pBufferTransitionVideo;
    size_t m_iOutTransitionVideo; 

    QPushButton* m_pushBtn;
    MusicLabel* m_musicLabel;
    LineEdit* m_lineEdit;

    //video attr
    GlobalVideoAttr* m_globalVideoAttr;

    //animation attr
    GlobalAnimationAttr* m_globalAnimationAttr;
    
    //text attr
    GlobalTextAttr* m_globalTextAttr;
    
    //visual effects attr
};
//! [0]

#endif // ELEMENT_H
