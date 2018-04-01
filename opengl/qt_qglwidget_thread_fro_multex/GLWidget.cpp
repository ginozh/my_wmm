#include <QTime>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QDebug>
#include "GLWidget.h"

GLWidget::GLWidget(QGLFormat format, const QGLWidget *shareWidget, QWidget *parent)
    : QGLWidget(format, parent, shareWidget),
    m_format(format),
    m_shareWidget(shareWidget)
{
	setAutoBufferSwap(false);

	doneCurrent();
    idxFrameBuf=0;
    maxFrameBuf=50;
    //usedFboCnt=10;

    for(int idx=0; idx<MAX_TEXTURES_CNT; idx++)
    {
        arrTexturesVar[idx].clear();
        arrTexturesVar[idx].append(QString("mSample%1").arg(idx));
    }
#if 0
    SDL_memset(&wanted_spec, 0, sizeof(wanted_spec));
#endif
    //motion
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
GLWidget::~GLWidget()
{
}
#if 0
void GLWidget::glInit()
{
    qDebug()<<"GLWidget::glInit";
	//stop QGLWidget standard behavior
}

void GLWidget::glDraw()
{
    qDebug()<<"GLWidget::glDraw";
	//notify thread of draw request
	//m_event.notify_one();
}
#endif
void GLWidget::initializeGL()
{
    qDebug()<<"GLWidget::initializeGL";
    initializeGLFunctions();
    if(m_init == 0)
        initialize();
}
void GLWidget::initialOpengl(int width, int height,bool bForce)
{
    qDebug()<<"GLWidget::initialOpengl width: "<<width<<" height: "<<height<<"bInitialGL:"<<bInitialGL;
    if(bInitialGL)
    {
        return;
    }
    else
    {
        bInitialGL=true;
    }
#if 1
    if(glw==width && glh==height)
    {
        return;
    }
#endif
    for (int ii=0; ii<vFboInfo.size(); ++ii)
    {
        qDebug()<<"GLWidget::releaseResource gldelete fb: "<<vFboInfo[ii].framebuffer;
        glDeleteFramebuffers(1, &vFboInfo[ii].framebuffer);
        qDebug()<<"GLWidget::releaseResource gldelete fbtexture: "<<vFboInfo[ii].fbotexture;
        glDeleteTextures(1, &vFboInfo[ii].fbotexture);
        //delete vFboInfo[ii].qfbo;
    }
    vFboInfo.clear();
    QMap<QString, STFragmentInfo >::iterator iA ;
    for(iA =  m_mapEffectProgram.begin(); iA!=m_mapEffectProgram.end(); ++iA)
    {
        STFragmentInfo& fragInfo=iA.value();
        glDeleteProgram(fragInfo.program);
        qDebug()<<"GLWidget::releaseResource gldelete program: "<<fragInfo.program;
        glDeleteShader(fragInfo.vshader);
        qDebug()<<"GLWidget::releaseResource gldelete vshader: "<<fragInfo.vshader;
        glDeleteShader(fragInfo.fshader);
        qDebug()<<"GLWidget::releaseResource gldelete fshader: "<<fragInfo.fshader;
    }
    m_mapEffectProgram.clear();

    if(ParseConfCreateProgram(":/Effects/Shader/","Basic")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","Overlay")!=0
            //|| ParseConfCreateProgram(":/Effects/Shader/","TiltShift")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","LightRoom")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","Background")!=0 
            || ParseConfCreateProgram(":/Effects/Shader/","FrameMap")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","SaveImage")!=0
      )
    {
        qInfo()<<"GLWidget::initialOpengl error. createProgram wrong. effect: Basic, Overlay...";
    }
    glw=width;
    glh=height;

    createBindFramebufferTexture(glw, glh);
}
void GLWidget::resizeGL(int width, int height)
{
    qDebug()<<"GLWidget::resizeGL width: "<<width<<" height: "<<height;
    //glViewport(0, 0, (GLint)width, (GLint)height);
    makeCurrent();
    initialOpengl(width, height);
    doneCurrent();
}

bool GLWidget::initialize()
{
    qDebug()<<"GLWidget::initialize";
	if(m_init == 0)
	{
		m_init=1;
	
		initializeGLContext();
	}
	return true;
}
void GLWidget::initializeGLContext()
{
    qDebug()<<"GLWidget::initializeGLContext";
	//create context from inside thread
	QGLContext *glContext=new QGLContext(m_format, this);

	//share context with another QGLWidget
	if(m_shareWidget != NULL)
    {
        qDebug()<<"GLWidget::initializeGLContext m_shareWidget->context: "<<m_shareWidget->context();
		glContext->create(m_shareWidget->context());
    }
	setContext(glContext);

	//make sure new context is current
	makeCurrent();

	//load extensions with glew
	///glewInit();

	glClearColor(0, 0, 0, 1.0f);
	glClearDepth(1.0f);

	doneCurrent();
}

void GLWidget::paintGL()
{
    qDebug()<<"GLWidget::paintGL m_idxFbo: "<<m_idxFbo;
    QTime startTime = QTime::currentTime();
    //makeCurrent();
    if(m_init == 0)
        initialize();
    if(m_idxFbo<=0)
    {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    //m_idxFbo=-1;
    int idxFbo=-1;
#if 0
    //image to fbo
    {
        QImage image1, image2;
        {
            QImage& image=image1;
            QString fileName="c:\\shareproject\\jpg\\img006.jpg";
            image.load(fileName);
            if (image.isNull()) {
                qDebug()<<"error";
            }
            image = image.convertToFormat(QImage::Format_RGBA8888);
        }
        GLuint texture=0;
        texture=load2DTexture(image1.width(), image1.height(), image1.bits());
        m_idxFbo=fragRenderForOtherThreadAgain("Basic"
                        , NULL, 0, texture
                        , 1, 1, 0, true, true);
        if(m_idxFbo<0)
        {
            qInfo()<<"fragRenderForOtherThread error";
        }
    }
#endif
    int dt = startTime.msecsTo(QTime::currentTime());
    //fbo to glwidget
    {
        GLuint displayTexture;
        displayTexture = GetTexture(m_idxFbo);
        idxFbo=fragRenderForOtherThreadAgain("Basic"
                , NULL, 0, displayTexture
                , 1, 1, 0, false);
        if(idxFbo!=0)
        {
            qInfo()<<"GLWidget::paintGL error ret: "<<idxFbo;
        }
    }
    int alldt = startTime.msecsTo(QTime::currentTime());
    qDebug()<<"GLWidget::paintGL dt: "<<dt<<" alldt: "<<alldt;
    //doneCurrent();
}
void GLWidget::resizeEvent(QResizeEvent *evt)
{
    qDebug()<<"GLWidget::resizeEvent evt: "<<evt;
    QGLWidget::resizeEvent(evt);
}
void GLWidget::showEvent(QShowEvent *event)
{
    qDebug()<<"GLWidget::showEvent evt: "<<event;
    QGLWidget::showEvent(event);
}
void GLWidget::closeEvent(QCloseEvent *evt)
{
    qDebug()<<"GLWidget::closeEvent evt: "<<evt;
    QGLWidget::closeEvent(evt);
}
#if 0
bool GLWidget::event(QEvent *e)
{
    //qDebug()<<"GLWidget::event evt: "<<e;
    return QGLWidget::event(e);
}
#endif
GLuint GLWidget::buildShader(const GLchar *shader_source, GLenum type) 
{
    //makeCurrent();
    GLuint shader = glCreateShader(type);
    qDebug()<<"GLWidget::buildShader glcreate shader: "<<shader;
    if (!shader || !glIsShader(shader)) {
        qInfo()<<"error buildShader type: "<<type;
        //doneCurrent();
        return 0;
    }
    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        qInfo()<<"error glGetShaderiv type: "<<type<<" status: "<<status;
        int length;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        log =(char*) malloc(length);//new char[length];
        glGetShaderInfoLog(shader, length, &length, log);
        //av_log(NULL, AV_LOG_ERROR, "build_shader error: %s\n", log);
        qInfo()<<"error buildShader: "<<log;
        free  (log);
    }
    //doneCurrent();
    return status == GL_TRUE ? shader : 0;
}
int GLWidget::createProgram(QString effectname)
{
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif

    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::createProgram error. no such effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    //av_log(NULL, AV_LOG_ERROR, "Fragment 2.1\n");
    //QString filePathPre=":/fragment/";
    //QString filePathPre="../opengl_off/";
    QString& fileName=fragInfo.fragFile; //filePathPre+effectname+".frag";
    QByteArray fragSource;
    if(fragVertex.isEmpty())
    {
        //QString fileName=":/Effects/vertex.frag";
        QString fileName=":/Effects/vertex.frag"; //for test. uncomplete
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qInfo()<<"GLWidget::createProgram error can't read theme file: "<<fileName;
            return -2;
        }
        fragVertex = file.readAll();
        file.close();
    }
    {
        QString& fileName=fragInfo.fragFile; //filePathPre+effectname+".frag";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qInfo()<<"GLWidget::createProgram error can't read theme file: "<<fileName;
            return -2;
        }
        fragSource = file.readAll();
        file.close();
    }
    //makeCurrent();
    // build_program
    GLuint v_shader, f_shader;
    //v_shader = buildShader(v_shader_source, GL_VERTEX_SHADER);
    v_shader = buildShader(fragVertex.constData(), GL_VERTEX_SHADER);
    //gs->f_shader = buildShader(f_shader_source, GL_FRAGMENT_SHADER);
    f_shader = buildShader(fragSource.constData(), GL_FRAGMENT_SHADER);
    if (!(v_shader && f_shader )) {
        // av_log(NULL, AV_LOG_ERROR, "build_shader error\n");
        // return -1;
        qInfo()<<"GLWidget::createProgram error. build_shader";
        //doneCurrent();
        return -3;
    }
    //av_log(NULL, AV_LOG_ERROR, "Fragment 3\n");

    GLuint program = glCreateProgram();
    qDebug()<<"GLWidget::createProgram glcreate program: "<<program;
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        qInfo()<<"GLWidget::createProgram error createProgram glGetProgramiv,"
            <<" maybe frag is wrong status: "<<status ;
        glDeleteShader(v_shader);
        glDeleteShader(f_shader);
        glDeleteProgram(program);
        qDebug()<<"GLWidget::createProgram gldelete v_shader: "<<v_shader;
        qDebug()<<"GLWidget::createProgram gldelete f_shader: "<<f_shader;
        qDebug()<<"GLWidget::createProgram gldelete program: "<<program;
        int length;
        char *log;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if(length>0)
        {
            log =(char*) malloc(length);//new char[length];
            glGetProgramInfoLog (program, length, &length, log);
            //av_log(NULL, AV_LOG_ERROR, "build_shader error: %s\n", log);
            qInfo()<<"GLWidget::createProgram error glLinkProgram: "<<log;
            free  (log);
        }
        //doneCurrent();
        return -4;
    }
    else
    {
        qDebug()<<"GLWidget::createProgram m_mapEffectProgram insert effectname: "
            <<effectname<<" program: "<<program;
        //m_mapEffectProgram.insert(effectname, program);
        fragInfo.program=program;
        fragInfo.vshader=v_shader;
        fragInfo.fshader=f_shader;
    }
    //end build_program
#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "GLWidget::createProgram waste_time createProgram. time: " << dt
        <<" start_time: "<<startTime.toString();
#endif

    //doneCurrent();
    return 0;
}
void GLWidget::createBindFramebufferTexture(int w, int h)
{
    //makeCurrent();
    QTime startTime = QTime::currentTime();
#if 0
    for (int ii=0; ii<framebuffers.size(); ++ii)
    {
        glDeleteFramebuffers(1, &framebuffers[ii]);
    }
    for (int ii=0; ii<fbotextures.size(); ++ii)
    {
        glDeleteTextures(1, &fbotextures[ii]);
    }
    framebuffers.clear();
    fbotextures.clear();
#endif
    for (int ii=0; ii<vFboInfo.size(); ++ii)
    {
        qDebug()<<"GLWidget::createBindFramebufferTexture gldelete fb: "<<vFboInfo[ii].framebuffer;
        glDeleteFramebuffers(1, &vFboInfo[ii].framebuffer);
        qDebug()<<"GLWidget::createBindFramebufferTexture gldelete fbtexture: "<<vFboInfo[ii].fbotexture;
        glDeleteTextures(1, &vFboInfo[ii].fbotexture);
        //delete vFboInfo[ii].qfbo;
    }
    vFboInfo.clear();
    idxFrameBuf=0;//usedFboCnt=0;
    for (int ii = 0; ii < maxFrameBuf; ++ii) 
    {
        GLuint texture = createAndSetupTexture();
        //fbotextures.push_back(texture);

        //设置纹理大小和图像大小一致
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        // 创建一个帧缓冲
        // https://www.khronos.org/opengl/wiki/Framebuffer_Object
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        qDebug()<<"GLWidget::createBindFramebufferTexture glcreate fbo: "<<fbo;
        //framebuffers.push_back(fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 绑定纹理到帧缓冲
        //     gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, texture, 0);

        vFboInfo.push_back(STFboInfo(ii, fbo, texture, w, h));
        //STFboInfo& fboinfo=vFboInfo.back();
        //fboinfo.qfbo = new QOpenGLFramebufferObject(w, h);
        qDebug()<<"GLWidget::createBindFramebufferTexture insert vFboInfo. idx: "<<ii
            <<" fbo: "<<fbo<<" texture: "<<texture;
    }
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<<"GLWidget::createBindFramebufferTexture waste: "<<dt;
    //doneCurrent();
}
GLint GLWidget::load2DTexture(int w, int h, const unsigned char *pixels)
{
    qDebug()<<"GLWidget::load2DTexture width: "<<w<<" height: "<<h;
    //makeCurrent();
    QTime startTime = QTime::currentTime();
    GLuint textureId1=createAndSetupTexture();
    // Step4 加载纹理
    glTexImage2D(/*GL_TEXTURE_RECTANGLE*/ GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            //GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        //GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<<"GLWidget::load2DTexture waste: "<<dt<<" w: "<<w<<" h: "<<h
        <<" pixels: "<<(void*)pixels;
    //doneCurrent();
    return textureId1;
}
int GLWidget::LoadUniformAllTexture(const QString& effectname, const STFragmentInfo& fragInfo
        , GLuint texture
        , GLuint texture2)
{
    //makeCurrent();
    //GLuint textureId[MAX_TEXTURES_CNT];
    //int cnt=0;
    textureCnt=0;
    //STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    GLuint program=fragInfo.program;

    textureId[textureCnt++]=texture;//GLWidget::load2DTexture(width, height, bits);
    if(texture2>0)
    {
        textureId[textureCnt++]=texture2;//GLWidget::load2DTexture(width2, height2, bits1);
    }
    for(int idx=0; idx<fragInfo.textures.size(); idx++)
    {
        std::shared_ptr<QImage> qimage=fragInfo.textures[idx].image;
        textureId[textureCnt++]=GLWidget::load2DTexture(qimage->width(), qimage->height(), (const unsigned char*)qimage->bits());
        qDebug()<<"GLWidget::LoadUniformAllTexture qimage->width: "<<qimage->width()
            <<" qimage->height: "<<qimage->height()<<" filename: "<<fragInfo.textures[idx].filename;
    }
    for(int idx=0; idx<textureCnt; idx++)
    {
        glActiveTexture(GL_TEXTURE0+idx); //必须从0开始
        glBindTexture(GL_TEXTURE_2D, textureId[idx]);
        glUniform1i(glGetUniformLocation(program, arrTexturesVar[idx].constData()), idx);
        qDebug()<<"GLWidget::LoadUniformAllTexture glUniform1i idx: "<<idx
            <<" GL_TEXTURE: "<<(GL_TEXTURE0+idx)<<" textureId: "<<textureId[idx];
    }
    //doneCurrent();
    return 0;
}

int GLWidget::UniformAllParament(const QString& effectname, const STFragmentInfo& fragInfo)
{
    //makeCurrent();
    //STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    GLuint program=fragInfo.program;

    QMap<QString, STParaInfo >::const_iterator it;
    for(it=fragInfo.paras.begin(); it!=fragInfo.paras.end(); ++it)
    //for(int idx=0; idx<fragInfo.paras.size(); idx++)
    {
        //STParaInfo& para = fragInfo.paras[idx];
        const STParaInfo& para = it.value();
        if(para.type.compare("float")==0)
        {
            qDebug()<<"GLWidget::UniformAllParament paras: "<<((void*)&fragInfo.paras)<<" type: "<<para.type<<"name: "
                <<para.name<<" deflt: " <<para.deflt;
            glUniform1f(glGetUniformLocation(program, para.name.constData()), para.deflt.toFloat());
        }
        else if(para.type.compare("int")==0)
        {
            qDebug()<<"GLWidget::UniformAllParament paras: "<<((void*)&fragInfo.paras)<<" type: "<<para.type<<"name: "
                <<para.name<<" deflt: " <<para.deflt;
            glUniform1i(glGetUniformLocation(program, para.name.constData()), para.deflt.toInt());
        }
        else
        {
            qInfo()<<"GLWidget::UniformAllParament error type: "<<para.type<<" effectname: "<<effectname;
        }
    }
    //doneCurrent();
    return 0;
}
int GLWidget::ParseConfCreateProgram(const QString pathpre, const QString& effectname)
{
    if(m_mapEffectProgram.contains(effectname))
    {
        m_mapEffectProgram[effectname].refcnt++;
        qDebug()<<"GLWidget::ParseConfCreateProgram effect is exist. effectname: "<<effectname
            <<" refcnt: "<<m_mapEffectProgram[effectname].refcnt;
        return 0;
    }
    QString fileName=pathpre+effectname+".conf";
    QFile fileconf(fileName);
    if (!fileconf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo()<<"GLWidget::ParseConfCreateProgram error can't read theme file: "<<fileName;
        return -1;
    }
    QString conf = fileconf.readAll();
    fileconf.close();

    QJsonParseError error;
    QJsonDocument doc=QJsonDocument::fromJson(conf.toUtf8(), &error);
    if (error.error) {
        qInfo() << "GLWidget::ParseConfCreateProgram error. Failed to parse text message as JSON object. filename :" 
            << fileName << "Error is:" << error.errorString();
        return -1;
    } else if (!doc.isObject()) {
        qInfo() << "GLWidget::ParseConfCreateProgram error. Received JSON message that is not an object: filename: " 
            << fileName;
        return -2;
    }
    QVariantMap result = doc.toVariant().toMap();
    //qDebug() << "GLWidget::ParseConfCreateProgram test:" << result["test"].toString();
    //qDebug() << "GLWidget::ParseConfCreateProgram SHADER:" << result["SHADER"].toString();
    m_mapEffectProgram.insert(effectname, STFragmentInfo());
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];//减少拷贝

    fragInfo.fragFile=result["SHADER"].toString();
    if(fragInfo.fragFile.isEmpty())
    {
        qInfo() << "GLWidget::ParseConfCreateProgram error. fragFile is empty. effectname: "
           <<effectname; 
        return -1;
    }
    fragInfo.fragFile=pathpre+fragInfo.fragFile;
    qDebug()<<"GLWidget::ParseConfCreateProgram fragFile: "<<fragInfo.fragFile;
    //QMap<QString/*effectname*/, STFragmentInfo> m_mapEffectProgram;
    foreach (QVariant texture, result["TEXTURES"].toList()) 
    {
        //qDebug() << "\t-" << texture.toString();
        if(fragInfo.textures.size()>MAX_TEXTURES_CNT)
        {
            qInfo() << "GLWidget::ParseConfCreateProgram error. great MAX_TEXTURES_CNT. effectname: "
                <<effectname<<"MAX_TEXTURES_CNT: "<<MAX_TEXTURES_CNT; 
            return -1;
        }
        fragInfo.textures.push_back(STTextureInfo());
        STTextureInfo& textInfo=fragInfo.textures.back();

        QVariantMap textureinfo = texture.toMap();
        if(!textureinfo.contains("INDEX") || !textureinfo.contains("FILE"))
        {
            qInfo() << "GLWidget::ParseConfCreateProgram error. no index or file. effectname: "
                <<effectname;
            return -1;
        }
        if(textureinfo["TYPE"].toString().compare("image")!=0)
        {
            qInfo() << "GLWidget::ParseConfCreateProgram error. type isnot image. effectname: "
                <<effectname<<" TYPE:" << result["TYPE"].toString();
            return -3;
        }
        textInfo.index = textureinfo["INDEX"].toInt();
        textInfo.filename = pathpre+"texture/"+textureinfo["FILE"].toString();
#if 0
        if(image_to_avframe(textInfo.filename.toStdString().c_str(), m_gs->w, m_gs->h, AV_PIX_FMT_RGBA, textInfo.avFrame))
        {
            qInfo()<<"GLWidget::ParseConfCreateProgram error image_to_avframe. filename: "
                <<textInfo.filename;
            return -4;
        }
#endif
#if 0
        QFile fileimage(textInfo.filename);
        if (!fileimage.open(QIODevice::ReadOnly ))
        {
            qInfo()<<"GLWidget::ParseConfCreateProgram error canot read image. effectname: "
                <<effectname<<" file: "<<textInfo.filename;
            return -4;
        }
        textInfo.arrData = fileimage.readAll();//scale
        fileimage.close();
#endif
        std::shared_ptr<QImage>& image=textInfo.image;
        image = std::shared_ptr<QImage>(new QImage());
		if(!image->load(textInfo.filename))
        {
            qInfo()<<"GLWidget::ParseConfCreateProgram error canot load image file: "
                <<textInfo.filename;
            return -5;
        }
#if 0
        if(m_gs && (image->width()!=m_gs->w || image->height()!=m_gs->h))
        {
            qDebug()<<"GLWidget::ParseConfCreateProgram scale image. image_w: "
                <<image->width()<<" image_h: "<<image->height()<<" to w: "<<m_gs->w
                <<" h: "<<m_gs->h;
			//image = std::shared_ptr<QImage>(new QImage(image->scaled(m_gs->w, m_gs->h, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
			image = std::shared_ptr<QImage>(new QImage(image->scaled(m_gs->w, m_gs->h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            qDebug()<<"GLWidget::ParseConfCreateProgram after scale image. image_w: "
                <<image->width()<<" image_h: "<<image->height()<<" to w: "<<m_gs->w
                <<" h: "<<m_gs->h;
        }
#endif
        if(image->format()!=QImage::Format_RGBA8888)
        {
            qDebug()<<"GLWidget::ParseConfCreateProgram converFormat";
			image = std::shared_ptr<QImage>(new QImage(image->convertToFormat(QImage::Format_RGBA8888)));
        }

        qDebug()<<"GLWidget::ParseConfCreateProgram effectname: "<<effectname
            <<" index: "<<textInfo.index<<" type: "
            << textureinfo["TYPE"].toString()<<" file: "<<textInfo.filename;
    }
    foreach (QVariant singlepara, result["PARAMS"].toList()) 
    {
        QVariantMap parainfo = singlepara.toMap();
        if(!parainfo.contains("NAME") || !parainfo.contains("TYPE") || !parainfo.contains("DEFAULT"))
        {
            qInfo() << "GLWidget::ParseConfCreateProgram error. no name or type or default."
                <<" effectname: " <<effectname;
            return -1;
        }

        //fragInfo.paras.push_back(STParaInfo());
        fragInfo.paras.insert(parainfo["NAME"].toString(), STParaInfo());
        //STParaInfo& para=fragInfo.paras.back();
        STParaInfo& para=fragInfo.paras[parainfo["NAME"].toString()];

        para.name=QByteArray("u_")+parainfo["NAME"].toByteArray();
        para.type=parainfo["TYPE"].toString();
        para.deflt=parainfo["DEFAULT"];
        para.min=parainfo["MIN"];
        para.max=parainfo["MAX"];
        qDebug()<<"GLWidget::ParseConfCreateProgram effectname: "<<effectname<<" name: "<<para.name
            <<" type: "<<para.type<<" deflt: "<<para.deflt<<" min: "<<para.min<<" max: "<<para.max;
    }
#if 0
    QJsonObject jo = doc.object();
    QJsonValue jv = jo.value(QStringLiteral("SHADER"));
    const QString shader = jv.toString();
    qDebug()<<"GLWidget::ParseConfCreateProgram SHADER: "<<shader;
#endif
    return createProgram(effectname);
}
GLint GLWidget::createAndSetupTexture()
{
    //makeCurrent();
    GLuint textureId1=0;
    // Step1 创建并绑定纹理对象
    glGenTextures(1, &textureId1);
    qDebug()<<"GLWidget::createAndSetupTexture glcreate textureId: "<<textureId1;
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    // Step2 设定wrap参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Step3 设定filter参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //doneCurrent();
    return textureId1;
}
int GLWidget::fragRenderForOtherThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2/*=0*/
        , bool useFbo/*=true*/, bool oneFrameFirstgl/*=false*/)
{
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::fragRenderForOtherThreadAgain error. no such effect: "<<effectname;
        return -1;
    }
    return fragRenderForOtherThreadAgain(effectname, m_mapEffectProgram[effectname], ori_spos
            , ori_spossize, texture, globaltime, totaltime, texture2, useFbo, oneFrameFirstgl);
}
int GLWidget::fragRenderForOtherThreadAgain(const QString& effectname
        , const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2/*=0*/
        , bool useFbo/*=true*/, bool oneFrameFirstgl/*=false*/)
{
    QMutexLocker locker(&m_mutexRender); 
    //makeCurrent();
    if(!useFbo)
    {
        //glFlush();
    }
    qDebug()<<"GLWidget::fragRenderForOtherThreadAgain effectname: "<<effectname
        <<" texture: "<<texture<<" texture2: "<<texture2
        <<" idxFrameBuf: "<<idxFrameBuf<<" oneFrameFirstgl: "<<oneFrameFirstgl
        <<" globaltime: "<<globaltime<<" totaltime: "<<totaltime;
    if(texture<=0 || !glIsTexture(texture))
    {
        qInfo()<<"GLWidget::fragRenderForOtherThreadAgain error texture: "<<texture;
        return -1;
    }
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

    QTime startTime = QTime::currentTime();
    //int startTime2=(int)(glfwGetTime()*1000.0f);
    STFboInfo* fboinfo=NULL;
    STFboInfo* finalFboinfo=NULL;
    int dt, dtall;
    int iRtn=0;
    GLuint        program;
    GLint status;
    //GLuint fbo ;
    //VAO
    GLuint        pos_buf=0, tex_coord_buf=0;

    float w=glw, h=glh;
#if 0
    const float coordpos[12] = { 
        0.0f, h,
        w, h,
        0.0f, 0.0f,
        0.0f, 0.0f,
        w, h,
        w, 0.0f
    };
#endif
    const float coordpos[12] = {
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
    float static_spos[12] = { 
        0.0f, h,
        w, h,
        0.0f, 0.0f,
        0.0f, 0.0f,
        w, h,
        w, 0.0f
    };
    float* spos;
    int spossize;
    if(ori_spos && ori_spossize>0)
    {
        spos=ori_spos;
        spossize=ori_spossize;
    }
    else
    {
        spos=static_spos;
        spossize=sizeof(static_spos);
    }
        
#if 0
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::fragRenderForOtherThread error. no such effectname: "<<effectname;
        return -1;
    }
    program=m_mapEffectProgram[effectname].program;
#endif
    program=fragInfo.program;
    if (!glIsProgram(program)) {
        qInfo()<<"GLWidget::fragRenderForOtherThread error program is not program"<< " effectname:"
            <<effectname;
        iRtn=-2;
        goto END_fragRenderForOtherThread;
    }
    // 清除颜色缓冲区 重置为指定颜色 -- no
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    if(useFbo)
    {
        finalFboinfo = getNewFboInfo();
        if(!finalFboinfo)
        {
            iRtn=-4;
            goto END_fragRenderForOtherThread;
        }
#if 0
        if(oneFrameFirstgl)
            fragRenderForFbo(finalFboinfo);
#endif
        qDebug()<<"GLWidget::fragRenderForOtherThreadAgain texture: "<<texture<<"texture2:"
            <<texture2<<" to final_idxframe: "<<finalFboinfo->idxFrameBuf
            <<" final_fbo: "<<finalFboinfo->framebuffer
            <<" final_texture: "<<finalFboinfo->fbotexture;
        //fbo = framebuffers[idxFrameBuf % framebuffers.size()];
        glBindFramebuffer(GL_FRAMEBUFFER, finalFboinfo->framebuffer);
        //finalFboinfo->qfbo->bind();
        glViewport(0, 0, glw, glh);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, glw, glh);
        //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClear( GL_COLOR_BUFFER_BIT );
    }
    glUseProgram(program);

    glUniform1f(glGetUniformLocation(program, "u_first"), oneFrameFirstgl?1.0f:0.0f);
    ///oneFrameFirstgl=0.0f;
    glUniform1f(glGetUniformLocation(program, "u_global_time"), globaltime);
    glUniform1f(glGetUniformLocation(program, "u_total_time"), totaltime);
    glUniform2f(glGetUniformLocation(program, "u_resolution"), glw, glh);

    //VAO
    {
    glGenBuffers(1, &pos_buf);
    qDebug()<<"GLWidget::fragRenderForOtherThreadAgain glcreate pos_buf: "<<pos_buf;
    glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(spos), spos, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, spossize, spos, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    {
    glGenBuffers(1, &tex_coord_buf);
    qDebug()<<"GLWidget::fragRenderForOtherThreadAgain glcreate tex_coord_buf: "<<tex_coord_buf;
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordpos), coordpos, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "a_texCoord");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //texture
    LoadUniformAllTexture(effectname, fragInfo, texture, texture2);

    //parament
    UniformAllParament(effectname, fragInfo);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
    if(useFbo)
    {
#if 0
        QImage imagefrag=finalFboinfo->qfbo->toImage();
        static int iImage=0;
        bool bwrite=imagefrag.save(QString("fbofrag%1.jpg").arg(++iImage));
        qDebug()<<"GLWidget::fragRenderForOtherThreadAgain save image :" <<iImage<<" bwrite: "
            <<bwrite<<" width: "<<imagefrag.width()<<" height: "<<imagefrag.height()
            <<" bits: "<<imagefrag.bits();
        Display(imagefrag);
        finalFboinfo->qfbo->bindDefault();
#endif
    }
    if(!glIsTexture(texture))
    {
        qInfo()<<"GLWidget::fragRenderForOtherThreadAgain error texture: "<<texture;
    }

    dt = startTime.msecsTo(QTime::currentTime());
    //dt = (int)(glfwGetTime()*1000.0f)-startTime2;
    qDebug()<< "GLWidget::fragRenderForOtherThread waste_time frag. time: " << dt
        <<"idxFrameBuf: "<<(finalFboinfo?finalFboinfo->idxFrameBuf:0);
    if(!useFbo)
    {
        ////glfwSwapBuffers(m_gs->window); //uncomplete
        swapBuffers();
        dtall = startTime.msecsTo(QTime::currentTime());
        //dtall = (int)(glfwGetTime()*1000.0f)-startTime2;
        qDebug()<< "GLWidget::fragRenderForOtherThread waste_time glfwSwapBuffers. time: " 
            << dtall-dt;
    }

END_fragRenderForOtherThread:
#if 1
    if(pos_buf) {
        qDebug()<<"GLWidget::fragRenderForOtherThreadAgain gldelete pos_buf: "<<pos_buf;
        glDeleteBuffers(1, &pos_buf);
    }
    if(tex_coord_buf) {
        qDebug()<<"GLWidget::fragRenderForOtherThreadAgain gldelete tex_coord_buf: "<<tex_coord_buf;
        glDeleteBuffers(1, &tex_coord_buf);
    }
    for(int idx=texture2>0?2:1; idx<textureCnt; idx++)
    {
        qDebug()<<"GLWidget::fragRenderForOtherThreadAgain gldelete textureId: "<<textureId[idx];
        glDeleteTextures(1, &textureId[idx]);
    }
#endif
    //doneCurrent();
    return (!iRtn && finalFboinfo)?finalFboinfo->idxFrameBuf:iRtn;
}
STFboInfo* GLWidget::getNewFboInfo()
{
    STFboInfo* fboinfo=NULL;
    int fbosize=vFboInfo.size();
#if 0
    if(usedFboCnt>=fbosize)
    {
        qInfo()<<"GLWidget::getNewFboInfo error. usedFboCnt: "<<usedFboCnt
            <<" greate vFboInfo.size: "<<fbosize;
        return fboinfo;
    }
#endif
    int iloop=0;
    for(int iloop=0; iloop<fbosize; iloop++)
    {
        if(!vFboInfo[idxFrameBuf%fbosize].bUsed)
        {
            fboinfo=&vFboInfo[idxFrameBuf];
            fboinfo->bUsed=true;
            idxFrameBuf=(idxFrameBuf+1)%fbosize;
            break;
        }
        idxFrameBuf=(idxFrameBuf+1)%fbosize;
    }
    if(fboinfo)
    {
        qDebug()<<"GLWidget::getNewFboInfo idxFrameBuf: "<<fboinfo->idxFrameBuf
            << "framebuffer: "<<fboinfo->framebuffer
            <<" fbotexture: "<<fboinfo->fbotexture<<" qfbotexture: "//<<fboinfo->qfbo->texture()
            <<" fbosize: "<<fbosize;
    }
    else
    {
        qInfo()<<"GLWidget::getNewFboInfo error. no enough fbo. iloop: "<<iloop
            <<" fbosize: "<<fbosize;
        //expandFramebuffer(glw, glh);
    }
    return fboinfo;
}
STFboInfo* GLWidget::getFboInfo(int idx)
{
    int fbosize=vFboInfo.size();
    if(idx>=fbosize || idx<0)
    {
        qInfo()<<"GLWidget::getFboInfo error. idx: "<<idx
            <<" greate vFboInfo.size: "<<fbosize;
        return NULL;
    }
    if(!vFboInfo[idx].bUsed)
    {
        qInfo()<<"GLWidget::getFboInfo error. the fboinfo not use idx: "<<idx;
    }
    return &vFboInfo[idx];
}
GLuint GLWidget::GetTexture(int idx)
{
    STFboInfo* fboinfo=getFboInfo(idx);
    if(fboinfo)
    {
        qDebug()<<"GLWidget::GetTexture idx: "<<idx<<" fbotexture: "<<fboinfo->fbotexture
            <<" qfbotexture: "/*<<fboinfo->qfbo->texture()*/;
        return fboinfo->fbotexture;
        //return fboinfo->qfbo->texture();
    }
    else
    {
        qInfo()<<"GLWidget::GetTexture error. no such fboinfo. idx: "<<idx;
    }
    return 0;
}
void GLWidget::resetFboUsed(int idx)
{
    STFboInfo* fboinfo=getFboInfo(idx);
    if(fboinfo)
    {
        qDebug()<<"GLWidget::resetFboUsed idx: "<<idx<<" fbotexture: "<<fboinfo->fbotexture;
#if 1
        fboinfo->bUsed=false;
        fragRenderForFbo(fboinfo);
#endif
    }
    else
    {
        qInfo()<<"GLWidget::resetFboUsed error. no such fboinfo. idx: "<<idx;
    }
}
int GLWidget::fragRenderForFbo(STFboInfo* finalFboinfo)
{
    if(!finalFboinfo)
    {
        qInfo()<<"GLWidget::fragRenderForFbo error finalFboinfo is NULL";
        return -1;
    }
    const QString effectname="Background";
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::fragRenderForFbo error. no such effect: "<<effectname;
        return -2;
    }
    //makeCurrent();
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];

    QTime startTime = QTime::currentTime();
    int dt, dtall;
    int iRtn=0;
    GLuint        program;
    GLint status;
    //GLuint fbo ;
    //VAO
    GLuint        pos_buf=0, tex_coord_buf=0;
    float w=glw, h=glh;
    const float coordpos[12] = {
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
    float static_spos[12] = { 
        0.0f, h,
        w, h,
        0.0f, 0.0f,
        0.0f, 0.0f,
        w, h,
        w, 0.0f
    };
    float* spos;
    int spossize;
    {
        spos=static_spos;
        spossize=sizeof(static_spos);
    }
    program=fragInfo.program;
    if (!glIsProgram(program)) {
        qInfo()<<"GLWidget::fragRenderForFbo error program is not program"<< " effectname:"
            <<effectname;
        iRtn=-2;
        goto END_fragRenderForOtherThread;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, finalFboinfo->framebuffer);
    glViewport(0, 0, glw, glh);
    glUseProgram(program);
    glUniform2f(glGetUniformLocation(program, "u_resolution"), glw, glh);
    //VAO
    {
    glGenBuffers(1, &pos_buf);
    qDebug()<<"GLWidget::fragRenderForFbo glcreate pos_buf: "<<pos_buf;
    glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(spos), spos, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, spossize, spos, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    {
    glGenBuffers(1, &tex_coord_buf);
    qDebug()<<"GLWidget::fragRenderForFbo glcreate tex_coord_buf: "<<tex_coord_buf;
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordpos), coordpos, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "a_texCoord");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
    dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "GLWidget::fragRenderForFbo waste_time frag. time: " << dt
        <<"idxFrameBuf: "<<(finalFboinfo?finalFboinfo->idxFrameBuf:0);
END_fragRenderForOtherThread:
#if 1
    if(pos_buf) {
        qDebug()<<"GLWidget::fragRenderForFbo gldelete pos_buf: "<<pos_buf;
        glDeleteBuffers(1, &pos_buf);
    }
    if(tex_coord_buf) {
        qDebug()<<"GLWidget::fragRenderForFbo gldelete tex_coord_buf: "<<tex_coord_buf;
        glDeleteBuffers(1, &tex_coord_buf);
    }
#endif
    //doneCurrent();
    return (!iRtn && finalFboinfo)?finalFboinfo->idxFrameBuf:iRtn;
}
STFboInfo::STFboInfo()
{
}
STFboInfo::STFboInfo(int idx, GLuint fbo, GLuint texture, int w, int h){
    idxFrameBuf=idx;
    framebuffer=fbo;
    fbotexture=texture;
    //qfbo = new QOpenGLFramebufferObject(w, h);
    //bFboUsed=false;
    //bTextureUsed=false;
}
STFboInfo::~STFboInfo(){
    qDebug()<<"STFboInfo~STFboInfo";
}
