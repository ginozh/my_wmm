#include <QFile>
#include <QTime>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QVariant> 
#include <QImage>
#include <QDebug>
#include "GLWidget.h"

//GLWidget::GLWidget(QGLFormat format, QWidget *parent)
GLWidget::GLWidget(const QGLFormat &format, QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(format, parent, shareWidget)
{
#if 0
	setAutoBufferSwap(false);
	doneCurrent();
#endif
}

GLWidget::~GLWidget()
{
    qDebug()<<"GLWidget::~GLWidget";
}
#if 0
void GLWidget::glInit()
{
    qDebug()<<"GLWidget::glInit";
}

void GLWidget::glDraw()
{
    qDebug()<<"GLWidget::glDraw";
}
#endif
void GLWidget::initializeGL()
{
    qDebug()<<"GLWidget::initializeGL context: "<<context()
         <<" isValid: "<<isValid();
    initializeGLFunctions();
}

void GLWidget::resizeGL(int width, int height)
{
    qDebug()<<"GLWidget::resizeGL width: "<<width<<" height: "<<height<<"  context: "<<context()<<" isValid: "<<isValid();
    QGLWidget::resizeGL(width, height);
}

void GLWidget::paintGL()
{
    qDebug()<<"GLWidget::paintGL context: "<<context()<<" isValid: "<<isValid();
    QGLWidget::paintGL();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    qDebug()<<"GLWidget::paintEvent context: "<<context()<<" isValid: "<<isValid();
    QGLWidget::paintEvent(event);
}
#if 0
void GLWidget::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"GLWidget::resizeEvent context: "<<context()<<" isValid: "<<isValid();
    QGLWidget::resizeEvent(event);
}
#endif
void GLWidget::showEvent(QShowEvent *event)
{
    qDebug()<<"GLWidget::showEvent context: "<<context()<<" isValid: "<<isValid();
    QGLWidget::showEvent(event);
}
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

    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
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

int GLWidget::RMEffectRef(const QString& effectname)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::RMEffectRef error. effect is not exist. effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    if(fragInfo.refcnt<=0)
    {
        qInfo()<<"GLWidget::RMEffectRef refcnt is not right. effectname: "
            <<effectname<<" refcnt: "<<fragInfo.refcnt;
        return -2;
    }
    fragInfo.refcnt--;
    qDebug()<<"GLWidget::RMEffectRef effectname: "<<effectname<<" refcnt: "<<fragInfo.refcnt;
    return 0;
}

int GLWidget::RMEffectProgram(const QString& effectname)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::RMEffectProgram error. effect is not exist. effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    if(fragInfo.refcnt<=0)
    {
        qInfo()<<"GLWidget::RMEffectProgram refcnt is not right. effectname: "
            <<effectname<<" refcnt: "<<fragInfo.refcnt;
        return -2;
    }
    fragInfo.refcnt--;
    qDebug()<<"GLWidget::RMEffectProgram effectname: "<<effectname<<" refcnt: "<<fragInfo.refcnt;
    if(fragInfo.refcnt<=0)
    {
        qDebug()<<"GLWidget::RMEffectProgram rm effectname: "<<effectname<<" refcnt: "<<fragInfo.refcnt;
        //makeCurrent();
        glDeleteProgram(fragInfo.program);
        qDebug()<<"GLWidget::RMEffectProgram gldelete program: "<<fragInfo.program;
        glDeleteShader(fragInfo.vshader);
        qDebug()<<"GLWidget::RMEffectProgram gldelete vshader: "<<fragInfo.vshader;
        glDeleteShader(fragInfo.fshader);
        qDebug()<<"GLWidget::RMEffectProgram gldelete fshader: "<<fragInfo.fshader;
        //doneCurrent();
        m_mapEffectProgram.remove(effectname);
    }
    return 0;
}

int GLWidget::ParseConfCreateProgram(const QString pathpre, const QString& effectname)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
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
void GLWidget::clearOpenglColor()
{
    //makeCurrent();
    // 清除颜色缓冲区 重置为黑色
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
    //glClear(GL_COLOR_BUFFER_BIT); 
#if 0
    for (int ii=0; ii<vFboInfo.size(); ++ii)
    {
        if(vFboInfo[ii].bUsed)
        {
            vFboInfo[ii].bUsed=false;//glDeleteFramebuffers, glDeleteTextures //uncomplete
#if 0
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, vFboInfo[ii].framebuffer);
            //glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, vFboInfo[ii].fbotexture, 0); //Only need to do this once.
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, vFboInfo[ii].fbotexture, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0); //Only need to do this once.
            GLuint clearColor[4] = {0, 0, 0, 0};
            glClearBufferuiv(GL_COLOR, 0, clearColor);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
#endif
#if 0
            std::vector<GLubyte> emptyData(m_gs->w * m_gs->h * 4, 0);
            glBindTexture(GL_TEXTURE_2D, vFboInfo[ii].fbotexture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_gs->w, m_gs->h, GL_BGRA, GL_UNSIGNED_BYTE, &emptyData[0]);
#endif
        }
    }
    usedFboCnt=idxFrameBuf=0;
#endif
    //createBindFramebufferTexture(m_gs->w, m_gs->h);
    ////oneFrameFirstgl = 1.0f;
    //doneCurrent();
#if 0
    if(glwidget)
        glwidget->iMakeCurrent=0;
    if(hiddenwidget)
        hiddenwidget->iMakeCurrent=0;
#endif
}
void GLWidget::saveOpenglBuffer(int requested_frame)
{
    //makeCurrent();
    QImage mask(glw, glh,QImage::Format_RGBA8888);
    unsigned char* mask_pixels = (unsigned char *) mask.bits();
    glFlush();
    glReadPixels(0, 0, glw, glh, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
    mask.save(QString("%1.png").arg(requested_frame));
    //mask.save(QString("%1.jpg").arg(requested_frame));
    //doneCurrent();
}
int GLWidget::fragRenderForOtherThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2/*=0*/
        , STFboInfo* finalFboinfo/*=NULL*/, bool oneFrameFirstgl/*=false*/, int width, int height)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::fragRenderForOtherThreadAgain error. no such effect: "<<effectname;
        return -1;
    }
    return fragRenderForOtherThreadAgain(effectname, m_mapEffectProgram[effectname], ori_spos
            , ori_spossize, texture, globaltime, totaltime, texture2, finalFboinfo, oneFrameFirstgl
            , width, height);
}
int GLWidget::fragRenderForOtherThreadAgain(const QString& effectname
        , const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2/*=0*/
        , STFboInfo* finalFboinfo/*=NULL*/, bool oneFrameFirstgl/*=false*/, int width, int height)
{
    //QMutexLocker locker(&m_mutexRender); 
    //makeCurrent();
    if(!finalFboinfo)
    {
        //glFlush();
    }
    qDebug()<<"GLWidget::fragRenderForOtherThreadAgain effectname: "<<effectname
        <<" texture: "<<texture<<" texture2: "<<texture2
        <<" oneFrameFirstgl: "<<oneFrameFirstgl
        <<" globaltime: "<<globaltime<<" totaltime: "<<totaltime<<" glw: "<<glw<<" glh: "<<glh
        <<" width: "<<width<<" height: "<<height;
    if(texture<=0 || !glIsTexture(texture))
    {
        qInfo()<<"GLWidget::fragRenderForOtherThreadAgain error texture: "<<texture;
        return -1;
    }
    if(!glw || !glh)
    {
        qInfo()<<"GLWidget::fragRenderForOtherThreadAgain error glw: "<<glw<<" glh: "<<glh;
        return -1;
    }
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

    QTime startTime = QTime::currentTime();
    //int startTime2=(int)(glfwGetTime()*1000.0f);
    //STFboInfo* fboinfo=NULL;
    int dt, dtall;
    int iRtn=0;
    GLuint        program;
    //GLint status;
    //GLuint fbo ;
    //VAO
    GLuint        pos_buf=0, tex_coord_buf=0;

    float w=glw, h=glh;
#if 0
    if(width>0 && height>0)
    {
        w=width, h=height;
    }
    else
    {
        w=glw, h=glh;
    }
#endif
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
    if(finalFboinfo)
    {
#if 0
        finalFboinfo = getNewFboInfo();
        if(!finalFboinfo)
        {
            iRtn=-4;
            goto END_fragRenderForOtherThread;
        }
#endif
#if 0
        if(oneFrameFirstgl)
            fragRenderForFbo(finalFboinfo);
#endif
        qDebug()<<"GLWidget::fragRenderForOtherThreadAgain texture: "<<texture<<"texture2:"
            <<texture2<<" to final_idxframe: "<<finalFboinfo->idxFrameBuf
            <<" final_fbo: "<<finalFboinfo->framebuffer
            <<" final_texture: "<<finalFboinfo->fbotexture;
        //fbo = framebuffers[idxFrameBuf % framebuffers.size()];
        ////glBindFramebuffer(GL_FRAMEBUFFER, finalFboinfo->framebuffer);
        finalFboinfo->qfbo->bind();
        glViewport(0, 0, glw, glh);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
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
    qDebug()<<"GLWidget::fragRenderForOtherThreadAgain position loc: "<<loc;
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
    qDebug()<<"GLWidget::fragRenderForOtherThreadAgain a_texCoord loc: "<<loc;
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
    if(finalFboinfo)
    {
#if 0
        QImage imagefrag=finalFboinfo->qfbo->toImage();
        static int iImage=0;
        bool bwrite=imagefrag.save(QString("fbofrag%1.jpg").arg(++iImage));
        qDebug()<<"GLWidget::fragRenderForOtherThreadAgain save image :" <<iImage<<" bwrite: "
            <<bwrite<<" width: "<<imagefrag.width()<<" height: "<<imagefrag.height()
            <<" bits: "<<imagefrag.bits();
        //Display(imagefrag);
#endif
        //finalFboinfo->qfbo->bindDefault();
        QGLFramebufferObject::bindDefault();
        ////glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    if(!glIsTexture(texture))
    {
        qInfo()<<"GLWidget::fragRenderForOtherThreadAgain error texture: "<<texture;
    }

    dt = startTime.msecsTo(QTime::currentTime());
    //dt = (int)(glfwGetTime()*1000.0f)-startTime2;
    qDebug()<< "GLWidget::fragRenderForOtherThread waste_time frag. time: " << dt
        <<"idxFrameBuf: "<<(finalFboinfo?finalFboinfo->idxFrameBuf:0);
    if(!finalFboinfo)
    {
        ////glfwSwapBuffers(m_gs->window); //uncomplete
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
STFragmentInfo GLWidget::getFragmentInfo(const QString& effectname)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLWidget::getFragmentInfo error. no such effect: "<<effectname;
        return STFragmentInfo();
    }
    return m_mapEffectProgram[effectname];
}
void GLWidget::deleteTexture(GLuint texture)
{
    qDebug()<<"GLWidget::deleteTexture gldelete texture: "<<texture;
    //makeCurrent();
    glDeleteTextures(1, &texture);
    //doneCurrent();
}
void GLWidget::setGLSize(int width, int height)
{
    glw=width;
    glh=height;
}
