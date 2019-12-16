#ifndef TIMELINE_H
#define TIMELINE_H

#include "Clip.h"
//#include "Frame.h"
#include <QVector>
#include <QThread>
#include "SDL.h"

class Timeline
{
    public:
        Timeline(){
            //createThread privateThread
#if 0
            if (SDL_Init(SDL_INIT_AUDIO)) {
                fprintf(stderr,"Could not initialize SDL - %s. \n", SDL_GetError());
                exit(1);
            }
#endif
        }
        void AddClip(Clip* clip){
            m_clips.push_back(clip);
            av_log(NULL, AV_LOG_INFO, "AddClip size: %d\n", m_clips.size());
        }
#if 0
        Frame* GetFrame(long int requested_frame){
            Frame* frame=NULL;
            for (int i = 0; i < m_clips.size(); ++i) {
                if((frame=m_clips.at(i)->GetFrame(requested_frame))!=NULL){
                    return frame;
                }
            }
            return frame;
        }
#endif
        QImage GetFrame(long int requested_frame){
            QImage frame;
#if 0
            //1, 计算哪个clip
            for (int i = 0; i < m_clips.size(); ++i) {
            }
#endif
            if(iCurrentClip>=m_clips.size())
            {
                av_log(NULL, AV_LOG_INFO, "iCurrentClip: %d m_clips.size(): %d\n"
                        , iCurrentClip, m_clips.size());
                return frame;
            }
            //先顺序播放 uncomplete
            frame=m_clips.at(iCurrentClip)->GetFrame(requested_frame);
            if(frame.isNull()){
                av_log(NULL, AV_LOG_INFO, "\n\n======\n\n");
                if(++iCurrentClip>=m_clips.size())
                {
                    av_log(NULL, AV_LOG_INFO, "iCurrentClip2: %d m_clips.size(): %d\n"
                            , iCurrentClip, m_clips.size());
                    return frame;
                }
                return m_clips.at(iCurrentClip)->GetFrame(requested_frame);
            }
            return frame;
        }
        void Open(){
        }
        ~Timeline();
    private:
        QVector<Clip*> m_clips;
        int iCurrentClip=0;
};
#endif
