#include <QDebug>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QVariant> 
#include <QImage> 
#include "MMComm.h"


static const GLchar *v_shader_source =
"attribute vec2 position;\n"
"varying vec2 texCoord;\n"
"varying vec2 textureCoord;\n"
"void main(void) {\n"
"  gl_Position = vec4(position, 0, 1);\n"
"  //texCoord = position;\n"
"  //textureCoord = position;\n"
"  texCoord = position* 0.5 + 0.5;\n"
"  textureCoord = position* 0.5 + 0.5;\n"
"}\n";
#ifndef __UBUNTU__
static GLuint buildShader(const GLchar *shader_source, GLenum type) {
    GLuint shader = glCreateShader(type);
    if (!shader || !glIsShader(shader)) {
        qInfo()<<"error buildShader type: "<<type;
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
    return status == GL_TRUE ? shader : 0;
}
#endif
int MMGlobalContext::createProgram(QString effectname)
{
#ifdef __UBUNTU__
    return -1;
#else
    if(!m_gs)
    {
        qInfo()<<"MMGlobalContext::createProgram error createProgram m_gs is NULL";
        return -1;
    }
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    //GenericShaderContext* gs=m_gs;

    if(!mapEffectProgram.contains(effectname))
    {
        qInfo()<<"MMGlobalContext::createProgram error. no such effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=mapEffectProgram[effectname];
    //av_log(NULL, AV_LOG_ERROR, "Fragment 2.1\n");
    //QString filePathPre=":/fragment/";
    //QString filePathPre="../opengl_off/";
    QString& fileName=fragInfo.fragFile; //filePathPre+effectname+".frag";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo()<<"MMGlobalContext::createProgram error can't read theme file: "<<fileName;
        return -2;
    }
    QString fragSource = file.readAll();
#if 0
    QString fragSource = 
        "vec4 INPUT(vec2 tc);\n"
        "\n" + file.readAll()+
        "uniform sampler2D tex;\n"
        "varying vec2 texCoord;\n"
        "vec4 INPUT(vec2 tc)\n"
        "{\n"
        "//return texture2D(tex, texCoord * 0.5 + 0.5);\n"
        "return texture2D(tex, tc);\n"
        "}\n"
        "void main() {\n"
        "//gl_FragColor = texture2D(tex, texCoord * 0.5 + 0.5);\n"
        "gl_FragColor = FUNCNAME(texCoord * 0.5 + 0.5);\n"
        "//gl_FragColor = FUNCNAME(texCoord);\n"
        "}\n";
#endif
    file.close();
    // build_program
    GLuint v_shader, f_shader;
    v_shader = buildShader(v_shader_source, GL_VERTEX_SHADER);
    //gs->f_shader = buildShader(f_shader_source, GL_FRAGMENT_SHADER);
    f_shader = buildShader(fragSource.toLocal8Bit().constData(), GL_FRAGMENT_SHADER);
    if (!(v_shader && f_shader )) {
        // av_log(NULL, AV_LOG_ERROR, "build_shader error\n");
        // return -1;
        qInfo()<<"MMGlobalContext::createProgram error. build_shader";
        return -3;
    }
    //av_log(NULL, AV_LOG_ERROR, "Fragment 3\n");

    GLuint program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        qInfo()<<"MMGlobalContext::createProgram error createProgram glGetProgramiv, status: "<<status;
        int length;
        char *log;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        log =(char*) malloc(length);//new char[length];
        glGetProgramInfoLog (program, length, &length, log);
        //av_log(NULL, AV_LOG_ERROR, "build_shader error: %s\n", log);
        qInfo()<<"MMGlobalContext::createProgram error glLinkProgram: "<<log;
        free  (log);
        return -4;
    }
    else
    {
        qDebug()<<"MMGlobalContext::createProgram mapEffectProgram insert effectname: "
            <<effectname<<" program: "<<program;
        //mapEffectProgram.insert(effectname, program);
        fragInfo.program=program;
    }
    //end build_program
#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "MMGlobalContext::createProgram waste_time createProgram. time: " << dt
        <<" start_time: "<<startTime.toString();
#endif

    return 0;
#endif
}

//void MMGlobalContext::initialOpengl(int width, int height)
void MMGlobalContext::initialOpengl(int width, int height,bool bForce)
{
#ifndef __UBUNTU__
    //if(m_gs && m_gs->w==width && m_gs->h==height)
    if(!bForce && m_gs )//&& m_gs->w==width && m_gs->h==height
    {
        qDebug()<<"MMGlobalContext::initialOpengl same, no change. width: "<<width<<" height: "<<height;
        return;
    }
    GenericShaderContext* &gs=m_gs;
    qDebug()<<"MMGlobalContext::initialOpengl width: "<<width<<" height: "<<height;
    if(gs)
    {
        qDebug()<<"MMGlobalContext::initialOpengl clean windows";
        if(gs->window)
        {
            glfwDestroyWindow(gs->window);
        }
        if(gs->threadWin)
        {
            glfwDestroyWindow(gs->threadWin);
        }
        delete gs;
        gs=NULL;
    }
    //GenericShaderContext* gs = new GenericShaderContext;
    gs = new GenericShaderContext;
    gs->w=width;
    gs->h=height;
    if (!glfwInit())
    {
        qInfo()<<"MMGlobalContext::initialOpengl glfwInit. error ";
    }
    glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
    if(!(gs->threadWin = glfwCreateWindow( gs->w, gs->h, "Thread Window", NULL, NULL )))
    {
        qInfo()<<"MMGlobalContext::initialOpengl error. glfwCreateWindow thread";
    }
    //glfwWindowHint(GLFW_VISIBLE, 0);
    glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
    //if(!(gs->window = glfwCreateWindow(gs->w, gs->h, "", NULL, NULL)))
    if(!(gs->window = glfwCreateWindow(gs->w, gs->h, "", NULL, gs->threadWin)))
    {
        qInfo()<<"MMGlobalContext::initialOpengl error. glfwCreateWindow";
    }


    glfwMakeContextCurrent(gs->window);
#ifdef __APPLE__
    ;
#else
#ifdef __UBUNTU__
    ;
#else
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        //av_log(NULL, AV_LOG_ERROR, "glewInit. error: %s\n", glewGetErrorString(err));
        qInfo()<<"error. glewInit: "<<glewGetErrorString(err);
    }
    //av_log(NULL, AV_LOG_ERROR, "Fragment 2\n");
#endif
#endif

    glViewport(0, 0, gs->w, gs->h);

    //createProgram("Aibao");
    //createProgram("ColourDistance");
#endif
}

MMGlobalContext* MMGlobalContext::m_pInstance = NULL;
MMGlobalContext::MMGlobalContext()
{
    for(int idx=0; idx<MAX_TEXTURES_CNT; idx++)
    {
        arrTexturesVar[idx].clear();
        arrTexturesVar[idx].append(QString("mSample%1").arg(idx));
    }
#if 0
    SDL_memset(&wanted_spec, 0, sizeof(wanted_spec));
#endif
#if 0
    allEffects.insert("Motion050", STEffectsInfo(MMET_MOTION_FFMPEG, "panupleft"
                , new ZPMincontext("1.4","0","max((zoom-1)*(1-on/duration)*ih,0)", 0, 0, 0, 0, "0", "(zoom-1)*ih" )));
    allEffects.insert("Motion051", STEffectsInfo(MMET_MOTION_FFMPEG, "panup"
                , new ZPMincontext("1.4","(iw-mw)/2","max((zoom- 1)*(1-on/duration)*ih,0)", 0, 0, 1, 0, "0", "(zoom-1)*ih" )));
    allEffects.insert("Motion052", STEffectsInfo(MMET_MOTION_FFMPEG, "panupright"
                , new ZPMincontext("1.4","(zoom-1)*iw","max((zoom-1)*(1-on/duration)*ih,0)", 0, 0, 0, 0, "0", "(zoom-1)*ih" )));
    allEffects.insert("Motion053", STEffectsInfo(MMET_MOTION_FFMPEG, "pandownleft"
                , new ZPMincontext("1.4","0","min(on*(zoom-1)*ih/duration,(zoom-1)*ih/2)", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Motion054", STEffectsInfo(MMET_MOTION_FFMPEG, "pandown"
                , new ZPMincontext("1.4","(iw-mw)/2","min(on*(zoom-1)*ih/duration,(zoom-1)*ih)", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Motion055", STEffectsInfo(MMET_MOTION_FFMPEG, "pandownright"
                , new ZPMincontext("1.4","(zoom-1)*iw","min(on*(zoom-1)*ih/duration,(zoom-1)*ih/2)", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Motion056", STEffectsInfo(MMET_MOTION_FFMPEG, "panrighttop"
                , new ZPMincontext("1.4","min(on*(zoom-1)*iw/duration,iw)","0", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Motion057", STEffectsInfo(MMET_MOTION_FFMPEG, "panright"
                , new ZPMincontext("1.4","min(on*(zoom-1)*iw/duration,(zoom-1)*iw/2)","(ih-mh)/2", 0, 0, 0, 1, "0", "0" )));
    allEffects.insert("Motion058", STEffectsInfo(MMET_MOTION_FFMPEG, "panrightbottom"
                , new ZPMincontext("1.4","min(on*(zoom-1)*iw/duration,(zoom-1)*iw/2)","(zoom-1)*ih/2", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Motion059", STEffectsInfo(MMET_MOTION_FFMPEG, "panlefttop"
                , new ZPMincontext("1.4","max(floor((zoom-1)*(1-on/duration)*iw),0)","0", 0, 0, 0, 0, "0.2*iw", "0" )));
    allEffects.insert("Motion061", STEffectsInfo(MMET_MOTION_FFMPEG, "panleft"
                , new ZPMincontext("1.4","max(floor((zoom-1)*(1-on/duration)*iw),0)","(zoom-1)", 0, 0, 0, 0, "0.2*iw", "0" )));
    allEffects.insert("Motion062", STEffectsInfo(MMET_MOTION_FFMPEG, "panleftbottom"
                , new ZPMincontext("1.4","max(floor((zoom-1)*(1-on/duration)*iw),0)","(zoom-1)*ih/2", 0, 0, 0, 0, "0.2*iw", "0" )));
    allEffects.insert("Motion063", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomincenter"
                , new ZPMincontext("1+0.01*on","(iw-mw)/2","(ih-mh)/2", 0, 0, 1, 1, "0", "0" )));
    allEffects.insert("Motion064", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomintopleft"
                , new ZPMincontext("1+0.01*on","(iw-mw)/2","0", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Motion065", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomintopright"
                , new ZPMincontext("1+0.01*on","iw-mw","0", 1, 0, 0, 0, "0", "0" )));
    allEffects.insert("Motion066", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominright"
                , new ZPMincontext("1+0.01*on","(iw-mw)/2","0", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Motion067", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominbottomright"
                , new ZPMincontext("1+0.01*on","iw-mw","ih-mh", 1, 1, 0, 0, "0", "0" )));
    allEffects.insert("Motion068", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominbottom"
                , new ZPMincontext("1+0.01*on","(iw-mw)/2","ih-mh", 0, 1, 1, 0, "0", "0" )));
    allEffects.insert("Motion069", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominbottomleft"
                , new ZPMincontext("1+0.01*on","0","ih-mh", 0, 1, 0, 0, "0", "0" )));
    allEffects.insert("Motion070", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominleft"
                , new ZPMincontext("1+0.01*on","0","(ih-mh)/2", 0, 0, 0, 1, "0", "0" )));
    allEffects.insert("Motion071", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutcenter"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","(iw-mw)/2","(ih-mh)/2", 0, 0, 1, 1, "0", "0" )));
    allEffects.insert("Motion072", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomouttopleft"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","0","0", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Motion073", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomouttop"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","(iw-mw)/2","0", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Motion074", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutright"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","iw-mw","(ih-mh)/2", 1, 0, 0, 1, "0", "0" )));
    allEffects.insert("Motion075", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutbottomright"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","iw-mw","ih-mh", 1, 1, 0, 0, "0", "0" )));
    allEffects.insert("Motion076", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutbottom"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","(iw-mw)/2","ih-mh", 0, 1, 1, 0, "0", "0" )));
    allEffects.insert("Motion077", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutbottomleft"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","0","ih-mh", 0, 1, 0, 0, "0", "0" )));
    allEffects.insert("Motion078", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutleft"
                , new ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","0","(ih-mh)/2", 0, 0, 0, 1, "0", "0" )));

    //trasition
    allEffects.insert("Trans013", STEffectsInfo(MMET_TRANSITION_FRAG,"CrossZoom"));
    allEffects.insert("Trans019", STEffectsInfo(MMET_TRANSITION_FRAG,"Fadegrayscale"));
    allEffects.insert("Trans020", STEffectsInfo(MMET_TRANSITION_FRAG,"ButterFlyWaveScrawler"));
    allEffects.insert("Trans021", STEffectsInfo(MMET_TRANSITION_FRAG,"ColourDistance"));
    allEffects.insert("Trans022", STEffectsInfo(MMET_TRANSITION_FRAG,"CrazyParammetricFun"));
    allEffects.insert("Trans023", STEffectsInfo(MMET_TRANSITION_FRAG,"Cube"));
    allEffects.insert("Trans024", STEffectsInfo(MMET_TRANSITION_FRAG,"Dispersionblur"));
    allEffects.insert("Trans026", STEffectsInfo(MMET_TRANSITION_FRAG,"Evaporate1"));
    allEffects.insert("Trans027", STEffectsInfo(MMET_TRANSITION_FRAG,"Evaporate2"));
    //allEffects.insert("Trans028", STEffectsInfo(MMET_TRANSITION_FRAG,"Flash"));
    allEffects.insert("Trans029", STEffectsInfo(MMET_TRANSITION_FRAG,"Heart"));
    allEffects.insert("Trans031", STEffectsInfo(MMET_TRANSITION_FRAG,"morph"));
    allEffects.insert("Trans034", STEffectsInfo(MMET_TRANSITION_FRAG,"PageCurl"));
    allEffects.insert("Trans035", STEffectsInfo(MMET_TRANSITION_FRAG,"Pinwheel"));
    allEffects.insert("Trans036", STEffectsInfo(MMET_TRANSITION_FRAG,"PolkaDot"));
    allEffects.insert("Trans037", STEffectsInfo(MMET_TRANSITION_FRAG,"ripper"));
    allEffects.insert("Trans038", STEffectsInfo(MMET_TRANSITION_FRAG,"Star"));
    allEffects.insert("Trans039", STEffectsInfo(MMET_TRANSITION_FRAG,"swap"));
    allEffects.insert("Trans040", STEffectsInfo(MMET_TRANSITION_FRAG,"swirl"));
    allEffects.insert("Trans042", STEffectsInfo(MMET_TRANSITION_FRAG,"undulating"));
    allEffects.insert("Trans044", STEffectsInfo(MMET_TRANSITION_FRAG,"Flyeye"));
    allEffects.insert("Trans046", STEffectsInfo(MMET_TRANSITION_FRAG,"DoorWay"));
    allEffects.insert("Trans056", STEffectsInfo(MMET_TRANSITION_FRAG,"Pixelate"));
    allEffects.insert("Trans058", STEffectsInfo(MMET_TRANSITION_FRAG,"Roll_anti_clockwise"));
    allEffects.insert("Trans059", STEffectsInfo(MMET_TRANSITION_FRAG,"Roll_clockwise"));
    allEffects.insert("Trans065", STEffectsInfo(MMET_TRANSITION_FRAG,"Row_split_2"));
    allEffects.insert("Trans071", STEffectsInfo(MMET_TRANSITION_FRAG,"sweep_anti_clockwise"));
    allEffects.insert("Trans072", STEffectsInfo(MMET_TRANSITION_FRAG,"sweep_clockwise"));
    allEffects.insert("Trans073", STEffectsInfo(MMET_TRANSITION_FRAG,"Twrl"));
    allEffects.insert("Trans074", STEffectsInfo(MMET_TRANSITION_FRAG,"wave"));
    allEffects.insert("Trans081", STEffectsInfo(MMET_TRANSITION_FRAG,"Round_zoom_in"));
    allEffects.insert("Trans082", STEffectsInfo(MMET_TRANSITION_FRAG,"Round_zoom_out"));
    allEffects.insert("Trans083", STEffectsInfo(MMET_TRANSITION_FRAG,"Zoom"));

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

    //Filter
    allEffects.insert("Filter004", STEffectsInfo(MMET_FILTER_FRAG,"Aibao"));
    allEffects.insert("Filter018", STEffectsInfo(MMET_FILTER_FRAG,"BW_Noise"));
    allEffects.insert("Filter025", STEffectsInfo(MMET_FILTER_FRAG,"FuzzyImg"));
    allEffects.insert("Filter035", STEffectsInfo(MMET_FILTER_FRAG,"Oldvideo"));
    allEffects.insert("Filter037", STEffectsInfo(MMET_FILTER_FRAG,"Rainbow1"));
    allEffects.insert("Filter038", STEffectsInfo(MMET_FILTER_FRAG,"Rainbow2"));
    allEffects.insert("Filter046", STEffectsInfo(MMET_FILTER_FRAG,"VCRdistortion"));
    allEffects.insert("Filter051", STEffectsInfo(MMET_FILTER_FRAG,"WS_OLD_PHOTO"));
    allEffects.insert("Filter053", STEffectsInfo(MMET_FILTER_FRAG,"ChromaticAberration"));
    allEffects.insert("Filter056", STEffectsInfo(MMET_FILTER_FRAG,"Fisheye"));
    allEffects.insert("Filter059", STEffectsInfo(MMET_FILTER_FRAG,"Kaleidoscope"));
    allEffects.insert("Filter064", STEffectsInfo(MMET_FILTER_FRAG,"Ribbon"));
    allEffects.insert("Filter068", STEffectsInfo(MMET_FILTER_FRAG,"Water"));
    allEffects.insert("Filter073", STEffectsInfo(MMET_FILTER_FRAG,"Flushed"));
    allEffects.insert("Filter080", STEffectsInfo(MMET_FILTER_FRAG,"Vintage"));
    allEffects.insert("Filter082", STEffectsInfo(MMET_FILTER_FRAG,"1977"));
    allEffects.insert("Filter083", STEffectsInfo(MMET_FILTER_FRAG,"Aegean"));
    allEffects.insert("Filter084", STEffectsInfo(MMET_FILTER_FRAG,"Amaro"));
    allEffects.insert("Filter085", STEffectsInfo(MMET_FILTER_FRAG,"Brannan"));
    allEffects.insert("Filter086", STEffectsInfo(MMET_FILTER_FRAG,"Earlybird"));
    allEffects.insert("Filter088", STEffectsInfo(MMET_FILTER_FRAG,"Hefe"));
    allEffects.insert("Filter090", STEffectsInfo(MMET_FILTER_FRAG,"Hudson"));
    allEffects.insert("Filter091", STEffectsInfo(MMET_FILTER_FRAG,"InkWell"));
    allEffects.insert("Filter092", STEffectsInfo(MMET_FILTER_FRAG,"Lomo"));
    allEffects.insert("Filter093", STEffectsInfo(MMET_FILTER_FRAG,"LordKelvin"));
    allEffects.insert("Filter094", STEffectsInfo(MMET_FILTER_FRAG,"Nashville"));
    allEffects.insert("Filter097", STEffectsInfo(MMET_FILTER_FRAG,"Sierra"));
    allEffects.insert("Filter099", STEffectsInfo(MMET_FILTER_FRAG,"Sunrise"));
    allEffects.insert("Filter100", STEffectsInfo(MMET_FILTER_FRAG,"SunSet"));
    allEffects.insert("Filter101", STEffectsInfo(MMET_FILTER_FRAG,"Sutro"));
    allEffects.insert("Filter102", STEffectsInfo(MMET_FILTER_FRAG,"Toaster"));
    allEffects.insert("Filter103", STEffectsInfo(MMET_FILTER_FRAG,"Valencia"));
    allEffects.insert("Filter104", STEffectsInfo(MMET_FILTER_FRAG,"Walden"));
    allEffects.insert("Filter105", STEffectsInfo(MMET_FILTER_FRAG,"XproII"));
    allEffects.insert("Filter109", STEffectsInfo(MMET_FILTER_FRAG,"Vivid"));
    allEffects.insert("Filter110", STEffectsInfo(MMET_FILTER_FRAG,"Canvas"));
    allEffects.insert("Filter112", STEffectsInfo(MMET_FILTER_FRAG,"Classic"));
    allEffects.insert("Filter113", STEffectsInfo(MMET_FILTER_FRAG,"ColorSketch"));
    allEffects.insert("Filter115", STEffectsInfo(MMET_FILTER_FRAG,"Shadow"));
    allEffects.insert("Filter116", STEffectsInfo(MMET_FILTER_FRAG,"Sharp"));
    //allEffects.insert("Filter117", STEffectsInfo(MMET_FILTER_FRAG,"Sketch"));
    allEffects.insert("Filter118", STEffectsInfo(MMET_FILTER_FRAG,"Textile"));
    allEffects.insert("Filter119", STEffectsInfo(MMET_FILTER_FRAG,"Wash"));
    allEffects.insert("Filter121", STEffectsInfo(MMET_FILTER_FRAG,"BrightLights"));
    allEffects.insert("Filter122", STEffectsInfo(MMET_FILTER_FRAG,"Metropolis"));
    allEffects.insert("Filter123", STEffectsInfo(MMET_FILTER_FRAG,"September"));
#endif
}
MMGlobalContext* MMGlobalContext::instance()  
{  
    if(m_pInstance == NULL)  //判断是否第一次调用  
        m_pInstance = new MMGlobalContext();  
    return m_pInstance;  
}  
GLint MMGlobalContext::load2DTexture(int w, int h, const unsigned char *pixels)
{
    GLuint textureId1=0;
#ifndef __UBUNTU__
    // Step1 创建并绑定纹理对象
    glGenTextures(1, &textureId1);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    // Step2 设定wrap参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Step3 设定filter参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Step4 加载纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            //GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        //GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
#endif
    return textureId1;
}

int MMGlobalContext::fragRenderForOtherThread(const QString& effectname, 
        const unsigned char* bits, float globaltime, float totaltime, 
        const unsigned char* bits1
        )
{
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    int iRtn=0;
#ifndef __UBUNTU__
    GLuint        program;
    //GLuint textureId0, textureId1, textureId2, textureId3, textureId4, textureId5;
    GLuint textureId[MAX_TEXTURES_CNT];
    int cnt=0;
    GLint status;
#if 0
    QMap<QString, GLuint>::iterator i = mapEffectProgram.find(m_effectName);
    if (i != mapEffectProgram.end() && i.key() == m_effectName) {
        program=i.value() ;
    }
    else
    {
        qInfo()<<"MMGlobalContext::fragRenderForOtherThread error. no such effectid: "<<effectid;
        return -1;
    }
#endif
    if(!mapEffectProgram.contains(effectname))
    {
        qInfo()<<"MMGlobalContext::fragRenderForOtherThread error. no such effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=mapEffectProgram[effectname];
    program=fragInfo.program;
    
    // 判断
    qDebug()<<"MMGlobalContext::fragRenderForOtherThread program: "<<program<<" effectname:"<<effectname;
#if 0
    if (!glIsShader(gs->f_shader)) {
        qInfo()<<"error gs->f_shader is not shader";
    }
#endif
    if (!glIsProgram(program)) {
        qInfo()<<"MMGlobalContext::fragRenderForOtherThread error program is not program"<< " effectname:"
            <<effectname;
        iRtn=-2;
        goto END_fragRenderForOtherThread;
    }
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        qInfo()<<"MMGlobalContext::fragRenderForOtherThread error glGetProgramiv"<< " effectname:"
            <<effectname;
        iRtn=-3;
        goto END_fragRenderForOtherThread;
    }
    // 清除颜色缓冲区 重置为指定颜色
    glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 这里填写场景绘制代码
    //glBindVertexArray(VAOId);
    {
        GLuint        pos_buf;
        glGenBuffers(1, &pos_buf);
        glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

        // Step4: 指定解析方式  并启用顶点属性
        // 顶点位置属性
        GLint loc = glGetAttribLocation(program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    //shader.use();
    glUseProgram(program);
    // 启用多个纹理单元 绑定纹理对象
    //textureId0=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, bits);
    textureId[cnt++]=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, bits);
    if(bits1)
    {
        //textureId1=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, bits1);
        textureId[cnt++]=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, bits1);
    }
    for(int idx=0; idx<fragInfo.textures.size(); idx++)
    {
        //textureId[cnt++]=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, (const unsigned char*)fragInfo.textures[idx].arrData.constData());
        //textureId[cnt++]=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, (const unsigned char*)fragInfo.textures[idx].avFrame->data[0]);
        textureId[cnt++]=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, (const unsigned char*)fragInfo.textures[idx].image->bits());
    }
#if 0
    if(fragInfo.textures.size()>=1)
    {
        textureId2=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, bits1);
    }
#endif
    for(int idx=0; idx<cnt; idx++)
    {
        switch(idx){
        case 0: glActiveTexture(GL_TEXTURE0); break;
        case 1: glActiveTexture(GL_TEXTURE1); break;
        case 2: glActiveTexture(GL_TEXTURE2); break;
        case 3: glActiveTexture(GL_TEXTURE3); break;
        case 4: glActiveTexture(GL_TEXTURE4); break;
        case 5: glActiveTexture(GL_TEXTURE5); break;
        case 6: glActiveTexture(GL_TEXTURE6); break;
        case 7: glActiveTexture(GL_TEXTURE7); break;
        case 8: glActiveTexture(GL_TEXTURE8); break;
        case 9: glActiveTexture(GL_TEXTURE9); break;
        }
        glBindTexture(GL_TEXTURE_2D, textureId[idx]);
        glUniform1i(glGetUniformLocation(program, arrTexturesVar[idx].constData()), idx);
    }
#if 0
    // 启用多个纹理单元 绑定纹理对象
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    //glBindTexture(GL_TEXTURE_2D, textureId[index++]);
    //glUniform1i(glGetUniformLocation(shader.programId, "tex1"), 0); // 设置纹理单元为0号
    glUniform1i(glGetUniformLocation(program, "mSample0"), 0);
    if(bits1)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureId1);
        glUniform1i(glGetUniformLocation(program, "mSample1"), 1);

    }
#endif
    glUniform1f(glGetUniformLocation(program, "u_global_time"), globaltime);
    glUniform1f(glGetUniformLocation(program, "u_total_time"), totaltime);

    glUniform2f(glGetUniformLocation(program, "u_resolution"), m_gs->w, m_gs->h);

    for(int idx=0; idx<fragInfo.paras.size(); idx++)
    {
        STParaInfo& para = fragInfo.paras[idx];
        if(para.type.compare("float")==0)
        {
            qDebug()<<"MMGlobalContext::fragRenderForOtherThread type: "<<para.type<<"name: "
                <<para.name<<" deflt: " <<para.deflt;
            glUniform1f(glGetUniformLocation(program, para.name.constData()), para.deflt.toFloat());
        }
        else if(para.type.compare("int")==0)
        {
            glUniform1i(glGetUniformLocation(program, para.name.constData()), para.deflt.toInt());
        }
        else
        {
            qInfo()<<"MMGlobalContext::fragRenderForOtherThread error type: "<<para.type<<" effectname: "<<effectname;
        }
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //glBindVertexArray(0);
    glUseProgram(0);

END_fragRenderForOtherThread:
#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "MMGlobalContext::fragRenderForOtherThread waste_time frag. time: " << dt
        <<" start_time: "<<startTime.toString()<<" QThread::currentThreadId(): "<<QThread::currentThreadId();
#endif

#endif
    return iRtn;
}

int MMGlobalContext::ParseConfCreateProgram(const QString pathpre, const QString& effectname)
{
    QString fileName=pathpre+effectname+".conf";
    QFile fileconf(fileName);
    if (!fileconf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo()<<"MMGlobalContext::ParseConfCreateProgram error can't read theme file: "<<fileName;
        return -1;
    }
    QString conf = fileconf.readAll();
    fileconf.close();

    QJsonParseError error;
    QJsonDocument doc=QJsonDocument::fromJson(conf.toUtf8(), &error);
    if (error.error) {
        qInfo() << "MMGlobalContext::ParseConfCreateProgram error. Failed to parse text message as JSON object. filename :" 
            << fileName << "Error is:" << error.errorString();
        return -1;
    } else if (!doc.isObject()) {
        qInfo() << "MMGlobalContext::ParseConfCreateProgram error. Received JSON message that is not an object: filename: " 
            << fileName;
        return -2;
    }
    QVariantMap result = doc.toVariant().toMap();
    //qDebug() << "MMGlobalContext::ParseConfCreateProgram test:" << result["test"].toString();
    //qDebug() << "MMGlobalContext::ParseConfCreateProgram SHADER:" << result["SHADER"].toString();
    mapEffectProgram.insert(effectname, STFragmentInfo());
    STFragmentInfo& fragInfo=mapEffectProgram[effectname];//减少拷贝

    fragInfo.fragFile=result["SHADER"].toString();
    if(fragInfo.fragFile.isEmpty())
    {
        qInfo() << "MMGlobalContext::ParseConfCreateProgram error. fragFile is empty. effectname: "
           <<effectname; 
        return -1;
    }
    fragInfo.fragFile=pathpre+fragInfo.fragFile;
    qDebug()<<"MMGlobalContext::ParseConfCreateProgram fragFile: "<<fragInfo.fragFile;
    //QMap<QString/*effectname*/, STFragmentInfo> mapEffectProgram;
    foreach (QVariant texture, result["TEXTURES"].toList()) 
    {
        //qDebug() << "\t-" << texture.toString();
        if(fragInfo.textures.size()>MAX_TEXTURES_CNT)
        {
            qInfo() << "MMGlobalContext::ParseConfCreateProgram error. great MAX_TEXTURES_CNT. effectname: "
                <<effectname<<"MAX_TEXTURES_CNT: "<<MAX_TEXTURES_CNT; 
            return -1;
        }
        fragInfo.textures.push_back(STTextureInfo());
        STTextureInfo& textInfo=fragInfo.textures.back();

        QVariantMap textureinfo = texture.toMap();
        if(!textureinfo.contains("INDEX") || !textureinfo.contains("FILE"))
        {
            qInfo() << "MMGlobalContext::ParseConfCreateProgram error. no index or file. effectname: "
                <<effectname;
            return -1;
        }
        if(textureinfo["TYPE"].toString().compare("image")!=0)
        {
            qInfo() << "MMGlobalContext::ParseConfCreateProgram error. type isnot image. effectname: "
                <<effectname<<" TYPE:" << result["TYPE"].toString();
            return -3;
        }
        textInfo.index = textureinfo["INDEX"].toInt();
        textInfo.filename = pathpre+"texture/"+textureinfo["FILE"].toString();
#if 0
        if(image_to_avframe(textInfo.filename.toStdString().c_str(), m_gs->w, m_gs->h, AV_PIX_FMT_RGBA, textInfo.avFrame))
        {
            qInfo()<<"MMGlobalContext::ParseConfCreateProgram error image_to_avframe. filename: "
                <<textInfo.filename;
            return -4;
        }
#endif
#if 0
        QFile fileimage(textInfo.filename);
        if (!fileimage.open(QIODevice::ReadOnly ))
        {
            qInfo()<<"MMGlobalContext::ParseConfCreateProgram error canot read image. effectname: "
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
            qInfo()<<"MMGlobalContext::ParseConfCreateProgram error canot load image file: "
                <<textInfo.filename;
            return -5;
        }
        if(m_gs && (image->width()!=m_gs->w || image->height()!=m_gs->h))
        {
            qDebug()<<"MMGlobalContext::ParseConfCreateProgram scale image. image_w: "
                <<image->width()<<" image_h: "<<image->height()<<" to w: "<<m_gs->w
                <<" h: "<<m_gs->h;
			//image = std::shared_ptr<QImage>(new QImage(image->scaled(m_gs->w, m_gs->h, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
			image = std::shared_ptr<QImage>(new QImage(image->scaled(m_gs->w, m_gs->h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            qDebug()<<"MMGlobalContext::ParseConfCreateProgram after scale image. image_w: "
                <<image->width()<<" image_h: "<<image->height()<<" to w: "<<m_gs->w
                <<" h: "<<m_gs->h;
        }
        if(image->format()!=QImage::Format_RGBA8888)
        {
            qDebug()<<"MMGlobalContext::ParseConfCreateProgram converFormat";
			image = std::shared_ptr<QImage>(new QImage(image->convertToFormat(QImage::Format_RGBA8888)));
        }

        qDebug()<<"MMGlobalContext::ParseConfCreateProgram effectname: "<<effectname
            <<" index: "<<textInfo.index<<" type: "
            << textureinfo["TYPE"].toString()<<" file: "<<textInfo.filename;
    }
    foreach (QVariant singlepara, result["PARAMS"].toList()) 
    {
        QVariantMap parainfo = singlepara.toMap();
        if(!parainfo.contains("NAME") || !parainfo.contains("TYPE") || !parainfo.contains("DEFAULT"))
        {
            qInfo() << "MMGlobalContext::ParseConfCreateProgram error. no name or type or default."
                <<" effectname: " <<effectname;
            return -1;
        }

        fragInfo.paras.push_back(STParaInfo());
        STParaInfo& para=fragInfo.paras.back();

        para.name=QByteArray("u_")+parainfo["NAME"].toByteArray();
        para.type=parainfo["TYPE"].toString();
        para.deflt=parainfo["DEFAULT"];
        para.min=parainfo["MIN"];
        para.max=parainfo["MAX"];
        qDebug()<<"MMGlobalContext::ParseConfCreateProgram effectname: "<<effectname<<" name: "<<para.name
            <<" type: "<<para.type<<" deflt: "<<para.deflt<<" min: "<<para.min<<" max: "<<para.max;
    }
#if 0
    QJsonObject jo = doc.object();
    QJsonValue jv = jo.value(QStringLiteral("SHADER"));
    const QString shader = jv.toString();
    qDebug()<<"MMGlobalContext::ParseConfCreateProgram SHADER: "<<shader;
#endif
    return createProgram(effectname);
}
#if 0
STTextureInfo::~STTextureInfo()
{
    if(avFrame)
    {
        uint8_t* tmp=avFrame->data[0];
        av_frame_free(&avFrame);
        av_free(tmp);
    }
}
#endif
