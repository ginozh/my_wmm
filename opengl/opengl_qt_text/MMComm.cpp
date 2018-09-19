#include <QFile>
#include <QTime>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QVariant> 
#include <QImage>
#include <QDebug>
#include "MMComm.h"
#include "FFmpegUtilities.h"

MMGlobalContext* MMGlobalContext::m_pInstance = NULL;
MMGlobalContext::MMGlobalContext()
{
#if 0
    for(int idx=0; idx<MAX_TEXTURES_CNT; idx++)
    {
        arrTexturesVar[idx].clear();
        arrTexturesVar[idx].append(QString("mSample%1").arg(idx));
    }
#endif

    allEffects.insert("Motion050", STEffectsInfo(MMET_MOTION,"Motion","Panupleft"));
    allEffects.insert("Motion051", STEffectsInfo(MMET_MOTION,"Motion","Panup"));
    allEffects.insert("Motion052", STEffectsInfo(MMET_MOTION,"Motion","Panupright"));
    allEffects.insert("Motion053", STEffectsInfo(MMET_MOTION,"Motion","Pandownleft"));
    allEffects.insert("Motion054", STEffectsInfo(MMET_MOTION,"Motion","Pandown"));
    allEffects.insert("Motion055", STEffectsInfo(MMET_MOTION,"Motion","Pandownright"));
    //allEffects.insert("Motion056", STEffectsInfo(MMET_MOTION,"Motion","Panrighttop"));
    allEffects.insert("Motion057", STEffectsInfo(MMET_MOTION,"Motion","Panright"));
    //allEffects.insert("Motion058", STEffectsInfo(MMET_MOTION,"Motion","Panrightbottom"));
    //allEffects.insert("Motion059", STEffectsInfo(MMET_MOTION,"Motion","Panlefttop"));
    allEffects.insert("Motion061", STEffectsInfo(MMET_MOTION,"Motion","Panleft"));
    //allEffects.insert("Motion062", STEffectsInfo(MMET_MOTION,"Motion","Panleftbottom"));
    allEffects.insert("Motion063", STEffectsInfo(MMET_MOTION,"Motion","Zoomincenter"));
    //allEffects.insert("Motion064", STEffectsInfo(MMET_MOTION,"Motion","Zoomintopleft"));
    //allEffects.insert("Motion065", STEffectsInfo(MMET_MOTION,"Motion","Zoomintopright"));
    //allEffects.insert("Motion066", STEffectsInfo(MMET_MOTION,"Motion","Zoominright"));
    //allEffects.insert("Motion067", STEffectsInfo(MMET_MOTION,"Motion","Zoominbottomright"));
    //allEffects.insert("Motion068", STEffectsInfo(MMET_MOTION,"Motion","Zoominbottom"));
    //allEffects.insert("Motion069", STEffectsInfo(MMET_MOTION,"Motion","Zoominbottomleft"));
    //allEffects.insert("Motion070", STEffectsInfo(MMET_MOTION,"Motion","Zoominleft"));
    allEffects.insert("Motion071", STEffectsInfo(MMET_MOTION,"Motion","Zoomoutcenter"));
#if 0
    allEffects.insert("Motion072", STEffectsInfo(MMET_MOTION,"Motion","Zoomouttopleft"));
    allEffects.insert("Motion073", STEffectsInfo(MMET_MOTION,"Motion","Zoomouttop"));
    allEffects.insert("Motion074", STEffectsInfo(MMET_MOTION,"Motion","Zoomoutright"));
    allEffects.insert("Motion075", STEffectsInfo(MMET_MOTION,"Motion","Zoomoutbottomright"));
    allEffects.insert("Motion076", STEffectsInfo(MMET_MOTION,"Motion","Zoomoutbottom"));
    allEffects.insert("Motion077", STEffectsInfo(MMET_MOTION,"Motion","Zoomoutbottomleft"));
    allEffects.insert("Motion078", STEffectsInfo(MMET_MOTION,"Motion","Zoomoutleft"));
#endif
    //trasition
    allEffects.insert("Trans013", STEffectsInfo(MMET_TRANSITION,"CrossZoom"));
    allEffects.insert("Trans019", STEffectsInfo(MMET_TRANSITION,"Fadegrayscale"));
    allEffects.insert("Trans020", STEffectsInfo(MMET_TRANSITION,"ButterFlyWaveScrawler"));
    allEffects.insert("Trans021", STEffectsInfo(MMET_TRANSITION,"ColourDistance"));
    allEffects.insert("Trans022", STEffectsInfo(MMET_TRANSITION,"CrazyParammetricFun"));
    allEffects.insert("Trans023", STEffectsInfo(MMET_TRANSITION,"Cube"));
    allEffects.insert("Trans024", STEffectsInfo(MMET_TRANSITION,"Dispersionblur"));
    allEffects.insert("Trans026", STEffectsInfo(MMET_TRANSITION,"Evaporate1"));
    allEffects.insert("Trans027", STEffectsInfo(MMET_TRANSITION,"Evaporate2"));
    //allEffects.insert("Trans028", STEffectsInfo(MMET_TRANSITION,"Flash"));
    allEffects.insert("Trans029", STEffectsInfo(MMET_TRANSITION,"Heart"));
    allEffects.insert("Trans031", STEffectsInfo(MMET_TRANSITION,"Morph"));
    allEffects.insert("Trans034", STEffectsInfo(MMET_TRANSITION,"PageCurl"));
    allEffects.insert("Trans035", STEffectsInfo(MMET_TRANSITION,"Pinwheel"));
    allEffects.insert("Trans036", STEffectsInfo(MMET_TRANSITION,"PolkaDot"));
    allEffects.insert("Trans037", STEffectsInfo(MMET_TRANSITION,"Ripper"));
    allEffects.insert("Trans038", STEffectsInfo(MMET_TRANSITION,"Star"));
    allEffects.insert("Trans039", STEffectsInfo(MMET_TRANSITION,"Swap"));
    allEffects.insert("Trans040", STEffectsInfo(MMET_TRANSITION,"Swirl"));
    allEffects.insert("Trans042", STEffectsInfo(MMET_TRANSITION,"Undulating"));
    allEffects.insert("Trans044", STEffectsInfo(MMET_TRANSITION,"Flyeye"));
    allEffects.insert("Trans046", STEffectsInfo(MMET_TRANSITION,"DoorWay"));
    allEffects.insert("Trans056", STEffectsInfo(MMET_TRANSITION,"Pixelate"));
    allEffects.insert("Trans058", STEffectsInfo(MMET_TRANSITION,"Roll_anti_clockwise"));
    allEffects.insert("Trans059", STEffectsInfo(MMET_TRANSITION,"Roll_clockwise"));
    allEffects.insert("Trans065", STEffectsInfo(MMET_TRANSITION,"Row_split_2"));
    allEffects.insert("Trans071", STEffectsInfo(MMET_TRANSITION,"Sweep_anti_clockwise"));
    allEffects.insert("Trans072", STEffectsInfo(MMET_TRANSITION,"Sweep_clockwise"));
    allEffects.insert("Trans073", STEffectsInfo(MMET_TRANSITION,"Twrl"));
    allEffects.insert("Trans074", STEffectsInfo(MMET_TRANSITION,"Wave"));
    allEffects.insert("Trans081", STEffectsInfo(MMET_TRANSITION,"Round_zoom_in"));
    allEffects.insert("Trans082", STEffectsInfo(MMET_TRANSITION,"Round_zoom_out"));
    allEffects.insert("Trans083", STEffectsInfo(MMET_TRANSITION,"Zoom"));
#if 0
    //trasition_ffmpeg
	allEffects.insert("Trans500", STEffectsInfo(MMET_TRANSITION_FFMPEG,"bowtieh"));
    allEffects.insert("Trans501", STEffectsInfo(MMET_TRANSITION_FFMPEG,"bowtiev"));
    allEffects.insert("Trans502", STEffectsInfo(MMET_TRANSITION_FFMPEG,"diagboxout"));
    allEffects.insert("Trans503", STEffectsInfo(MMET_TRANSITION_FFMPEG,"diagcrossout"));
    allEffects.insert("Trans504", STEffectsInfo(MMET_TRANSITION_FFMPEG,"diagdownright"));
    allEffects.insert("Trans505", STEffectsInfo(MMET_TRANSITION_FFMPEG,"filledvdown"));
    allEffects.insert("Trans506", STEffectsInfo(MMET_TRANSITION_FFMPEG,"filledvleft"));
    allEffects.insert("Trans507", STEffectsInfo(MMET_TRANSITION_FFMPEG,"filledvright"));
    allEffects.insert("Trans508", STEffectsInfo(MMET_TRANSITION_FFMPEG,"filledvup"));
    allEffects.insert("Trans509", STEffectsInfo(MMET_TRANSITION_FFMPEG,"barsh"));
    allEffects.insert("Trans510", STEffectsInfo(MMET_TRANSITION_FFMPEG,"barsvertical"));
    allEffects.insert("Trans511", STEffectsInfo(MMET_TRANSITION_FFMPEG,"crossfade"));
    allEffects.insert("Trans512", STEffectsInfo(MMET_TRANSITION_FFMPEG,"checkerb"));
    allEffects.insert("Trans513", STEffectsInfo(MMET_TRANSITION_FFMPEG,"circle"));
    allEffects.insert("Trans514", STEffectsInfo(MMET_TRANSITION_FFMPEG,"circles"));
    allEffects.insert("Trans515", STEffectsInfo(MMET_TRANSITION_FFMPEG,"diamond"));
    //allEffects.insert("Trans516", STEffectsInfo(MMET_TRANSITION_FFMPEG,"heart"));
    allEffects.insert("Trans517", STEffectsInfo(MMET_TRANSITION_FFMPEG,"rectangle"));
    //allEffects.insert("Trans518", STEffectsInfo(MMET_TRANSITION_FFMPEG,"wheel"));
    allEffects.insert("Trans519", STEffectsInfo(MMET_TRANSITION_FFMPEG,"insetbottoml"));
    allEffects.insert("Trans520", STEffectsInfo(MMET_TRANSITION_FFMPEG,"insetbottomr"));
    allEffects.insert("Trans521", STEffectsInfo(MMET_TRANSITION_FFMPEG,"insettopleft"));
    allEffects.insert("Trans522", STEffectsInfo(MMET_TRANSITION_FFMPEG,"insettopr"));
    allEffects.insert("Trans523", STEffectsInfo(MMET_TRANSITION_FFMPEG,"iris"));
    allEffects.insert("Trans524", STEffectsInfo(MMET_TRANSITION_FFMPEG,"revealdown"));
    allEffects.insert("Trans525", STEffectsInfo(MMET_TRANSITION_FFMPEG,"revealright"));
    allEffects.insert("Trans526", STEffectsInfo(MMET_TRANSITION_FFMPEG,"revealleft"));
    allEffects.insert("Trans527", STEffectsInfo(MMET_TRANSITION_FFMPEG,"roll"));
    allEffects.insert("Trans528", STEffectsInfo(MMET_TRANSITION_FFMPEG,"slide"));
    allEffects.insert("Trans529", STEffectsInfo(MMET_TRANSITION_FFMPEG,"slideupt"));
    allEffects.insert("Trans530", STEffectsInfo(MMET_TRANSITION_FFMPEG,"slideleftt"));
    allEffects.insert("Trans531", STEffectsInfo(MMET_TRANSITION_FFMPEG,"slidedownt"));
    allEffects.insert("Trans532", STEffectsInfo(MMET_TRANSITION_FFMPEG,"splith"));
    allEffects.insert("Trans533", STEffectsInfo(MMET_TRANSITION_FFMPEG,"splitv"));
    allEffects.insert("Trans534", STEffectsInfo(MMET_TRANSITION_FFMPEG,"fanin"));
#endif
    //Filter
    allEffects.insert("Filter004", STEffectsInfo(MMET_FILTER,"Aibao"));
    allEffects.insert("Filter018", STEffectsInfo(MMET_FILTER,"BW_Noise"));
    allEffects.insert("Filter025", STEffectsInfo(MMET_FILTER,"FuzzyImg"));
    allEffects.insert("Filter035", STEffectsInfo(MMET_FILTER,"Oldvideo"));
    allEffects.insert("Filter037", STEffectsInfo(MMET_FILTER,"Rainbow1"));
    allEffects.insert("Filter038", STEffectsInfo(MMET_FILTER,"Rainbow2"));
    allEffects.insert("Filter046", STEffectsInfo(MMET_FILTER,"VCRdistortion"));
    allEffects.insert("Filter051", STEffectsInfo(MMET_FILTER,"WS_OLD_PHOTO"));
    allEffects.insert("Filter053", STEffectsInfo(MMET_FILTER,"ChromaticAberration"));
    allEffects.insert("Filter056", STEffectsInfo(MMET_FILTER,"Fisheye"));
    allEffects.insert("Filter059", STEffectsInfo(MMET_FILTER,"Kaleidoscope"));
    allEffects.insert("Filter064", STEffectsInfo(MMET_FILTER,"Ribbon"));
    allEffects.insert("Filter068", STEffectsInfo(MMET_FILTER,"Water"));
    allEffects.insert("Filter073", STEffectsInfo(MMET_FILTER,"Flushed"));
    allEffects.insert("Filter080", STEffectsInfo(MMET_FILTER,"Vintage"));
    allEffects.insert("Filter082", STEffectsInfo(MMET_FILTER,"1977"));
    allEffects.insert("Filter083", STEffectsInfo(MMET_FILTER,"Aegean"));
    allEffects.insert("Filter084", STEffectsInfo(MMET_FILTER,"Amaro"));
    allEffects.insert("Filter085", STEffectsInfo(MMET_FILTER,"Brannan"));
    allEffects.insert("Filter086", STEffectsInfo(MMET_FILTER,"Earlybird"));
    allEffects.insert("Filter088", STEffectsInfo(MMET_FILTER,"Hefe"));
    allEffects.insert("Filter090", STEffectsInfo(MMET_FILTER,"Hudson"));
    allEffects.insert("Filter091", STEffectsInfo(MMET_FILTER,"InkWell"));
    allEffects.insert("Filter092", STEffectsInfo(MMET_FILTER,"Lomo"));
    allEffects.insert("Filter093", STEffectsInfo(MMET_FILTER,"LordKelvin"));
    allEffects.insert("Filter094", STEffectsInfo(MMET_FILTER,"Nashville"));
    allEffects.insert("Filter097", STEffectsInfo(MMET_FILTER,"Sierra"));
    allEffects.insert("Filter099", STEffectsInfo(MMET_FILTER,"Sunrise"));
    allEffects.insert("Filter100", STEffectsInfo(MMET_FILTER,"SunSet"));
    allEffects.insert("Filter101", STEffectsInfo(MMET_FILTER,"Sutro"));
    allEffects.insert("Filter102", STEffectsInfo(MMET_FILTER,"Toaster"));
    allEffects.insert("Filter103", STEffectsInfo(MMET_FILTER,"Valencia"));
    allEffects.insert("Filter104", STEffectsInfo(MMET_FILTER,"Walden"));
    allEffects.insert("Filter105", STEffectsInfo(MMET_FILTER,"XproII"));
    allEffects.insert("Filter109", STEffectsInfo(MMET_FILTER,"Vivid"));
    allEffects.insert("Filter110", STEffectsInfo(MMET_FILTER,"Canvas"));
    allEffects.insert("Filter112", STEffectsInfo(MMET_FILTER,"Classic"));
    allEffects.insert("Filter113", STEffectsInfo(MMET_FILTER,"ColorSketch"));
    allEffects.insert("Filter115", STEffectsInfo(MMET_FILTER,"Shadow"));
    allEffects.insert("Filter116", STEffectsInfo(MMET_FILTER,"Sharp"));
    //allEffects.insert("Filter117", STEffectsInfo(MMET_FILTER,"Sketch"));
    allEffects.insert("Filter118", STEffectsInfo(MMET_FILTER,"Textile"));
    allEffects.insert("Filter119", STEffectsInfo(MMET_FILTER,"Wash"));
    allEffects.insert("Filter121", STEffectsInfo(MMET_FILTER,"BrightLights"));
    allEffects.insert("Filter122", STEffectsInfo(MMET_FILTER,"Metropolis"));
    allEffects.insert("Filter123", STEffectsInfo(MMET_FILTER,"September"));
}
MMGlobalContext* MMGlobalContext::instance()  
{  
    if(m_pInstance == NULL)  //判断是否第一次调用  
        m_pInstance = new MMGlobalContext();  
    return m_pInstance;  
}  
#if 1
static int lockmgr(void **mutex, enum AVLockOp op)
{
    SDL_mutex **mtx=(SDL_mutex **)mutex;
    switch(op) {
        case AV_LOCK_CREATE:
            *mtx = SDL_CreateMutex();
            if(!*mtx) {
                qInfo()<<"lockmgr error. SDL_CreateMutex(): "<<SDL_GetError();
                return 1;
            }
            return 0;
        case AV_LOCK_OBTAIN:
            return !!SDL_LockMutex(*mtx);
        case AV_LOCK_RELEASE:
            return !!SDL_UnlockMutex(*mtx);
        case AV_LOCK_DESTROY:
            SDL_DestroyMutex(*mtx);
            return 0;
    }
    return 1;
}

static QMutex mutexInitFFmpeg;
static bool bInit=false;
void initFFmpeg()
{
    QMutexLocker locker(&mutexInitFFmpeg);
    if(!bInit)
    {
        avcodec_register_all();
#if 0
#if CONFIG_AVDEVICE
        avdevice_register_all();
#endif
#endif
        avfilter_register_all();
        av_register_all();
        avformat_network_init();
        if (av_lockmgr_register(lockmgr)) {
            qInfo()<<"initFFmpeg error. Could not initialize lock manager!";
            return ;
        }
        bInit=true;
    }
}
#endif
STFboInfo::STFboInfo()
{
}
STFboInfo::STFboInfo(int idx, GLuint fbo, GLuint texture, int /*w*/, int /*h*/){
    idxFrameBuf=idx;
#if 1
    framebuffer=fbo;
    fbotexture=texture;
#endif
}
STFboInfo::STFboInfo(int idx, int w, int h){
    idxFrameBuf=idx;
#if 0
    framebuffer=fbo;
    fbotexture=texture;
#endif
    if(qfbo)
    {
        qInfo()<<"STFboInfo::STFboInfo error. qfbo isnot null. qfbo: "<<qfbo;
        delete qfbo;
    }
    qfbo = new QGLFramebufferObject(w, h);
    //bFboUsed=false;
    //bTextureUsed=false;
}
STFboInfo::~STFboInfo(){
    //qDebug()<<"STFboInfo~STFboInfo qfbo: "<<qfbo;
    if(qfbo) 
    {
        qDebug()<<"STFboInfo~STFboInfo qfbo: "<<qfbo;
        delete qfbo;
        qfbo=NULL;
    }
}

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
QDialog *previewWindow = NULL;
QLabel* previewLabel = NULL;
void Display(QImage& previewImage)
{
    if(!previewWindow)
    {
        // Create window
        previewWindow = new QDialog();
        previewWindow->setStyleSheet("background-color: #000000;");
        QHBoxLayout layout;

        // Create label with current frame's image
        previewLabel=new QLabel(previewWindow);
        previewLabel->setPixmap(QPixmap::fromImage(previewImage));
        //previewLabel.setMask(QPixmap::fromImage(*previewImage).mask());
        layout.addWidget(previewLabel);
        //previewImage->save("getimage.log");

        // Show the window
        previewWindow->setLayout(&layout);
        previewWindow->show();
    }
    else
    {
        previewLabel->setPixmap(QPixmap::fromImage(previewImage));
        QThread::msleep(1000);
    }

}
