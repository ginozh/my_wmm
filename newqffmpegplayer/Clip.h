#ifndef CLIP_H
#define CLIP_H

#include "FFmpegReader.h"
//#include "Frame.h"
#include <QImage>
class Clip
{
    public:
        Clip();
        Clip(FFmpegReader*);
        QImage GetFrame(long int requested_frame){
            QImage image;
            if(m_reader)
            {
               return m_reader->GetFrame(requested_frame);
            }
            return image;
        }
        ~Clip();
    private:
        FFmpegReader* m_reader=NULL;
};

#endif
