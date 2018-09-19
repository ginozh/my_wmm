#ifndef _GLHiddenWidgetGL_cpp
#define _GLHiddenWidgetGL_cpp

#include <QFile>
#include <QTime>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject> 
#include "GLHiddenWidget.h"
#include "GLDisplayWidget.h"
int GLHiddenWidget::m_gputype = GPUT_NOINIT;
int GLHiddenWidget::getGPUInfo()
{
    if(m_gputype!=GPUT_NOINIT)
    {
        return m_gputype;
    }
    m_gputype=GPUT_UNKOWN;
    QString version=glGetString(GL_VERSION)==NULL?"":((char*)glGetString(GL_VERSION));
    QString vendor=glGetString(GL_VENDOR)==NULL?"":((char*)glGetString(GL_VENDOR));
    QString renderer=glGetString(GL_RENDERER)==NULL?"":((char*)glGetString(GL_RENDERER));
    vendor=vendor.toLower();
    if(vendor.indexOf("intel")>=0)
    {
        m_gputype=GPUT_INTEL;
    }
    else if(vendor.indexOf("nvidia")>=0)
    {
        m_gputype=GPUT_NVIDIA;
    }
    else if(vendor.indexOf("amd")>=0)
    {
        m_gputype=GPUT_AMD;
    }

    ////m_gputype = GPUT_UNKOWN; //去掉此特性
    qInfo()<<"GLHiddenWidget::getGPUInfo GL_VERSION: "<<version<<" GL_VENDOR: "
        <<vendor<<" GL_RENDERER: "<< renderer<<" m_gputype: "<<m_gputype;
    return m_gputype;
}
GLuint GLHiddenWidget::buildShader(const GLchar *shader_source, GLenum type) 
{
    //makeCurrent();
    GLuint shader = glCreateShader(type);
    qDebug()<<"GLHiddenWidget::buildShader glcreate shader: "<<shader;
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

int GLHiddenWidget::createProgram(QString effectname)
{
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif

    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLHiddenWidget::createProgram error. no such effectname: "<<effectname;
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
            qInfo()<<"GLHiddenWidget::createProgram error can't read theme file: "<<fileName;
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
            qInfo()<<"GLHiddenWidget::createProgram error can't read theme file: "<<fileName;
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
        qInfo()<<"GLHiddenWidget::createProgram error. build_shader";
        //doneCurrent();
        if(v_shader)
            glDeleteShader(v_shader);
        if(f_shader)
            glDeleteShader(f_shader);
        return -3;
    }
    //av_log(NULL, AV_LOG_ERROR, "Fragment 3\n");

    GLuint program = glCreateProgram();
    qDebug()<<"GLHiddenWidget::createProgram glcreate program: "<<program;
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        qInfo()<<"GLHiddenWidget::createProgram error createProgram glGetProgramiv,"
            <<" maybe frag is wrong status: "<<status ;
        glDeleteShader(v_shader);
        glDeleteShader(f_shader);
        glDeleteProgram(program);
        qDebug()<<"GLHiddenWidget::createProgram gldelete v_shader: "<<v_shader;
        qDebug()<<"GLHiddenWidget::createProgram gldelete f_shader: "<<f_shader;
        qDebug()<<"GLHiddenWidget::createProgram gldelete program: "<<program;
        int length;
        char *log;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if(length>0)
        {
            log =(char*) malloc(length);//new char[length];
            glGetProgramInfoLog (program, length, &length, log);
            //av_log(NULL, AV_LOG_ERROR, "build_shader error: %s\n", log);
            qInfo()<<"GLHiddenWidget::createProgram error glLinkProgram: "<<log;
            free  (log);
        }
        //doneCurrent();
        return -4;
    }
    else
    {
        qDebug()<<"GLHiddenWidget::createProgram m_mapEffectProgram insert effectname: "
            <<effectname<<" program: "<<program;
        //m_mapEffectProgram.insert(effectname, program);
        fragInfo.program=program;
        fragInfo.vshader=v_shader;
        fragInfo.fshader=f_shader;
    }
    //end build_program
#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qInfo()<< "GLHiddenWidget::createProgram waste_time createProgram. time: " << dt
        <<" start_time: "<<startTime.toString();
#endif

    //doneCurrent();
    return 0;
}
GLint GLHiddenWidget::createAndSetupTexture()
{
    //makeCurrent();
    GLuint textureId1=0;
    // Step1 创建并绑定纹理对象
    glGenTextures(1, &textureId1);
    qDebug()<<"GLHiddenWidget::createAndSetupTexture glcreate textureId: "<<textureId1;
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
GLint GLHiddenWidget::load2DTextureInOut(int w, int h, const unsigned char *pixels)
{
    qDebug()<<"GLHiddenWidget::load2DTextureInOut width: "<<w<<" height: "<<h;
    //makeCurrent();
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    GLuint textureId1=createAndSetupTexture();
    // Step4 加载纹理
    glTexImage2D(/*GL_TEXTURE_RECTANGLE*/ GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            //GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
            GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        //GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qInfo()<<"GLHiddenWidget::load2DTextureInOut waste: "<<dt<<" w: "<<w<<" h: "<<h
        <<" pixels: "<<(void*)pixels;
#endif
    //doneCurrent();
    {
        QMutexLocker locker(&m_mutexTexture); 
        qDebug()<<"GLHiddenWidget::load2DTextureInOut vNeedDelTexture.size: "<<vNeedDelTexture.size()<<" textureId: "<<textureId1;
        for (int ii=0; ii<vNeedDelTexture.size(); ++ii)
        {
            glDeleteTextures(1, &vNeedDelTexture[ii]);
        }
        vNeedDelTexture.clear();
    }
    return textureId1;
}
int GLHiddenWidget::LoadUniformAllTexture(const QString& /*effectname*/, const STFragmentInfo& fragInfo
        , GLuint texture
        , GLuint texture2)
{
    //makeCurrent();
    //GLuint textureId[MAX_TEXTURES_CNT];
    //int cnt=0;
    textureCnt=0;
    //STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    GLuint program=fragInfo.program;

    textureId[textureCnt++]=texture;//GLHiddenWidget::load2DTextureInOut(width, height, bits);
    if(texture2>0)
    {
        textureId[textureCnt++]=texture2;//GLHiddenWidget::load2DTextureInOut(width2, height2, bits1);
    }
    for(int idx=0; idx<fragInfo.textures.size() && textureCnt<MAX_TEXTURES_CNT; idx++)
    {
        std::shared_ptr<QImage> qimage=fragInfo.textures[idx].image;
        textureId[textureCnt++]=GLHiddenWidget::load2DTextureInOut(qimage->width(), qimage->height(), (const unsigned char*)qimage->bits());
        qDebug()<<"GLHiddenWidget::LoadUniformAllTexture qimage->width: "<<qimage->width()
            <<" qimage->height: "<<qimage->height()<<" filename: "<<fragInfo.textures[idx].filename;
    }
    if(textureCnt>=MAX_TEXTURES_CNT)
    {
        qInfo()<<"GLHiddenWidget::LoadUniformAllTexture error. textureCnt: "<<textureCnt
            <<" MAX_TEXTURES_CNT: "<<MAX_TEXTURES_CNT;
    }
    for(int idx=0; idx<textureCnt; idx++)
    {
        glActiveTexture(GL_TEXTURE0+idx); //必须从0开始
        glBindTexture(GL_TEXTURE_2D, textureId[idx]);
        glUniform1i(glGetUniformLocation(program, arrTexturesVar[idx].constData()), idx);
        qDebug()<<"GLHiddenWidget::LoadUniformAllTexture glUniform1i idx: "<<idx
            <<" GL_TEXTURE: "<<(GL_TEXTURE0+idx)<<" textureId: "<<textureId[idx];
    }
    //doneCurrent();
    return 0;
}

int GLHiddenWidget::UniformAllParament(const QString& effectname, const STFragmentInfo& fragInfo)
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
            qDebug()<<"GLHiddenWidget::UniformAllParament paras: "<<((void*)&fragInfo.paras)<<" type: "<<para.type<<"name: "
                <<para.name<<" deflt: " <<para.deflt;
            glUniform1f(glGetUniformLocation(program, para.name.constData()), para.deflt.toFloat());
        }
        else if(para.type.compare("int")==0)
        {
            qDebug()<<"GLHiddenWidget::UniformAllParament paras: "<<((void*)&fragInfo.paras)<<" type: "<<para.type<<"name: "
                <<para.name<<" deflt: " <<para.deflt;
            glUniform1i(glGetUniformLocation(program, para.name.constData()), para.deflt.toInt());
        }
        else
        {
            qInfo()<<"GLHiddenWidget::UniformAllParament error type: "<<para.type<<" effectname: "<<effectname;
        }
    }
    //doneCurrent();
    return 0;
}

int GLHiddenWidget::RMEffectRef(const QString& effectname)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLHiddenWidget::RMEffectRef error. effect is not exist. effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    if(fragInfo.refcnt<=0)
    {
        qInfo()<<"GLHiddenWidget::RMEffectRef refcnt is not right. effectname: "
            <<effectname<<" refcnt: "<<fragInfo.refcnt;
        return -2;
    }
    fragInfo.refcnt--;
    qDebug()<<"GLHiddenWidget::RMEffectRef effectname: "<<effectname<<" refcnt: "<<fragInfo.refcnt;
    return 0;
}

int GLHiddenWidget::RMEffectProgram(const QString& effectname)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLHiddenWidget::RMEffectProgram error. effect is not exist. effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];
    if(fragInfo.refcnt<=0)
    {
        qInfo()<<"GLHiddenWidget::RMEffectProgram refcnt is not right. effectname: "
            <<effectname<<" refcnt: "<<fragInfo.refcnt;
        return -2;
    }
    fragInfo.refcnt--;
    qDebug()<<"GLHiddenWidget::RMEffectProgram effectname: "<<effectname<<" refcnt: "<<fragInfo.refcnt;
    if(fragInfo.refcnt<=0)
    {
        qDebug()<<"GLHiddenWidget::RMEffectProgram rm effectname: "<<effectname<<" refcnt: "<<fragInfo.refcnt;
        //makeCurrent();
        glDeleteProgram(fragInfo.program);
        qDebug()<<"GLHiddenWidget::RMEffectProgram gldelete program: "<<fragInfo.program;
        glDeleteShader(fragInfo.vshader);
        qDebug()<<"GLHiddenWidget::RMEffectProgram gldelete vshader: "<<fragInfo.vshader;
        glDeleteShader(fragInfo.fshader);
        qDebug()<<"GLHiddenWidget::RMEffectProgram gldelete fshader: "<<fragInfo.fshader;
        //doneCurrent();
        m_mapEffectProgram.remove(effectname);
    }
    return 0;
}

int GLHiddenWidget::ParseConfCreateProgram(const QString pathpre, const QString& effectname)
{
    ///initializeGLFunctions();
    //context()->functions();
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(m_mapEffectProgram.contains(effectname))
    {
        m_mapEffectProgram[effectname].refcnt++;
        qDebug()<<"GLHiddenWidget::ParseConfCreateProgram effect is exist. effectname: "<<effectname
            <<" refcnt: "<<m_mapEffectProgram[effectname].refcnt;
        return 0;
    }
    QString fileName=pathpre+effectname+".conf";
    QFile fileconf(fileName);
    if (!fileconf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo()<<"GLHiddenWidget::ParseConfCreateProgram error can't read theme file: "<<fileName;
        return -1;
    }
    QString conf = fileconf.readAll();
    fileconf.close();

    QJsonParseError error;
    QJsonDocument doc=QJsonDocument::fromJson(conf.toUtf8(), &error);
    if (error.error) {
        qInfo() << "GLHiddenWidget::ParseConfCreateProgram error. Failed to parse text message as JSON object. filename :" 
            << fileName << "Error is:" << error.errorString();
        return -1;
    } else if (!doc.isObject()) {
        qInfo() << "GLHiddenWidget::ParseConfCreateProgram error. Received JSON message that is not an object: filename: " 
            << fileName;
        return -2;
    }
    QVariantMap result = doc.toVariant().toMap();
    //qDebug() << "GLHiddenWidget::ParseConfCreateProgram test:" << result["test"].toString();
    //qDebug() << "GLHiddenWidget::ParseConfCreateProgram SHADER:" << result["SHADER"].toString();
    m_mapEffectProgram.insert(effectname, STFragmentInfo());
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];//减少拷贝

    fragInfo.fragFile=result["SHADER"].toString();
    if(fragInfo.fragFile.isEmpty())
    {
        qInfo() << "GLHiddenWidget::ParseConfCreateProgram error. fragFile is empty. effectname: "
           <<effectname; 
        return -1;
    }
    fragInfo.fragFile=pathpre+fragInfo.fragFile;
    qDebug()<<"GLHiddenWidget::ParseConfCreateProgram fragFile: "<<fragInfo.fragFile;
    //QMap<QString/*effectname*/, STFragmentInfo> m_mapEffectProgram;
    foreach (QVariant texture, result["TEXTURES"].toList()) 
    {
        //qDebug() << "\t-" << texture.toString();
        if(fragInfo.textures.size()>MAX_TEXTURES_CNT)
        {
            qInfo() << "GLHiddenWidget::ParseConfCreateProgram error. great MAX_TEXTURES_CNT. effectname: "
                <<effectname<<"MAX_TEXTURES_CNT: "<<MAX_TEXTURES_CNT; 
            return -1;
        }
        fragInfo.textures.push_back(STTextureInfo());
        STTextureInfo& textInfo=fragInfo.textures.back();

        QVariantMap textureinfo = texture.toMap();
        if(!textureinfo.contains("INDEX") || !textureinfo.contains("FILE"))
        {
            qInfo() << "GLHiddenWidget::ParseConfCreateProgram error. no index or file. effectname: "
                <<effectname;
            return -1;
        }
        if(textureinfo["TYPE"].toString().compare("image")!=0)
        {
            qInfo() << "GLHiddenWidget::ParseConfCreateProgram error. type isnot image. effectname: "
                <<effectname<<" TYPE:" << result["TYPE"].toString();
            return -3;
        }
        textInfo.index = textureinfo["INDEX"].toInt();
        textInfo.filename = pathpre+"texture/"+textureinfo["FILE"].toString();
#if 0
        if(image_to_avframe(textInfo.filename.toStdString().c_str(), m_gs->w, m_gs->h, AV_PIX_FMT_RGBA, textInfo.avFrame))
        {
            qInfo()<<"GLHiddenWidget::ParseConfCreateProgram error image_to_avframe. filename: "
                <<textInfo.filename;
            return -4;
        }
#endif
#if 0
        QFile fileimage(textInfo.filename);
        if (!fileimage.open(QIODevice::ReadOnly ))
        {
            qInfo()<<"GLHiddenWidget::ParseConfCreateProgram error canot read image. effectname: "
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
            qInfo()<<"GLHiddenWidget::ParseConfCreateProgram error canot load image file: "
                <<textInfo.filename;
            return -5;
        }
#if 0
        if(m_gs && (image->width()!=m_gs->w || image->height()!=m_gs->h))
        {
            qDebug()<<"GLHiddenWidget::ParseConfCreateProgram scale image. image_w: "
                <<image->width()<<" image_h: "<<image->height()<<" to w: "<<m_gs->w
                <<" h: "<<m_gs->h;
			//image = std::shared_ptr<QImage>(new QImage(image->scaled(m_gs->w, m_gs->h, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
			image = std::shared_ptr<QImage>(new QImage(image->scaled(m_gs->w, m_gs->h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            qDebug()<<"GLHiddenWidget::ParseConfCreateProgram after scale image. image_w: "
                <<image->width()<<" image_h: "<<image->height()<<" to w: "<<m_gs->w
                <<" h: "<<m_gs->h;
        }
#endif
        if(image->format()!=QImage::Format_RGBA8888)
        {
            qDebug()<<"GLHiddenWidget::ParseConfCreateProgram converFormat";
			image = std::shared_ptr<QImage>(new QImage(image->convertToFormat(QImage::Format_RGBA8888)));
        }

        qDebug()<<"GLHiddenWidget::ParseConfCreateProgram effectname: "<<effectname
            <<" index: "<<textInfo.index<<" type: "
            << textureinfo["TYPE"].toString()<<" file: "<<textInfo.filename;
    }
    foreach (QVariant singlepara, result["PARAMS"].toList()) 
    {
        QVariantMap parainfo = singlepara.toMap();
        if(!parainfo.contains("NAME") || !parainfo.contains("TYPE") || !parainfo.contains("DEFAULT"))
        {
            qInfo() << "GLHiddenWidget::ParseConfCreateProgram error. no name or type or default."
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
        qDebug()<<"GLHiddenWidget::ParseConfCreateProgram effectname: "<<effectname<<" name: "<<para.name
            <<" type: "<<para.type<<" deflt: "<<para.deflt<<" min: "<<para.min<<" max: "<<para.max;
    }
#if 0
    QJsonObject jo = doc.object();
    QJsonValue jv = jo.value(QStringLiteral("SHADER"));
    const QString shader = jv.toString();
    qDebug()<<"GLHiddenWidget::ParseConfCreateProgram SHADER: "<<shader;
#endif
    int ret=createProgram(effectname);;
    if(ret)
    {
        m_mapEffectProgram.remove(effectname);
    }
    return ret;
}
void GLHiddenWidget::clearOpenglColor()
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
void GLHiddenWidget::saveOpenglBuffer(int requested_frame)
{
#if 0
    m_glwidget->frame=frame;
    m_glwidget->update();
    QThread::msleep(1000);
#endif
    //以下代码在2个widget情况下无效
    qDebug()<<"GLHiddenWidget::saveOpenglBuffer requested_frame: "<<requested_frame;
    //makeCurrent();
    QImage mask(glw, glh,QImage::Format_RGBA8888);
    unsigned char* mask_pixels = (unsigned char *) mask.bits();
    //glFlush();
    glFinish();
    glReadPixels(0, 0, glw, glh, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
    static int idx=0;
    mask.save(QString("saveOpenglBuffer%1_%2.png").arg(requested_frame).arg(++idx));
    //mask.save(QString("%1.jpg").arg(requested_frame));
    //doneCurrent();
}
int GLHiddenWidget::baseFragRenderForAllThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2/*=0*/
        , STFboInfo* finalFboinfo/*=NULL*/, bool oneFrameFirstgl/*=false*/)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain error. no such effect: "<<effectname;
        return -1;
    }
    return baseFragRenderForAllThreadAgain(effectname, m_mapEffectProgram[effectname], ori_spos
            , ori_spossize, texture, globaltime, totaltime, texture2, finalFboinfo, oneFrameFirstgl
            );
}
int GLHiddenWidget::baseFragRenderForAllThreadAgain(const QString& effectname
        , const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2/*=0*/
        , STFboInfo* finalFboinfo/*=NULL*/, bool oneFrameFirstgl/*=false*/)
{
    QMutexLocker locker(&m_mutexRender); 
    //makeCurrent();
    if(!finalFboinfo)
    {
        //glFlush();
    }
    qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain effectname: "<<effectname
        <<" texture: "<<texture<<" texture2: "<<texture2
        <<" oneFrameFirstgl: "<<oneFrameFirstgl
        <<" globaltime: "<<globaltime<<" totaltime: "<<totaltime<<" glw: "<<glw<<" glh: "<<glh;
        
    if(texture<=0 || !glIsTexture(texture))
    {
        qInfo()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain error texture: "<<texture;
        return -1;
    }
    if(!glw || !glh)
    {
        qInfo()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain error glw: "<<glw<<" glh: "<<glh;
        return -1;
    }
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
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
        qInfo()<<"GLHiddenWidget::fragRenderForOtherThread error. no such effectname: "<<effectname;
        return -1;
    }
    program=m_mapEffectProgram[effectname].program;
#endif
    program=fragInfo.program;
    if (!glIsProgram(program)) {
        qInfo()<<"GLHiddenWidget::fragRenderForOtherThread error program is not program"<< " effectname:"
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
        qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain texture: "<<texture<<"texture2:"
            <<texture2<<" to final_idxframe: "<<finalFboinfo->idxFrameBuf
            <<" final_fbo: "<<finalFboinfo->framebuffer
            <<" final_texture: "<<finalFboinfo->fbotexture;
        //fbo = framebuffers[idxFrameBuf % framebuffers.size()];
#ifdef USE_ORI_FBO
        glBindFramebuffer(GL_FRAMEBUFFER, finalFboinfo->framebuffer);
#else
        finalFboinfo->qfbo->bind();
#endif
        glViewport(0, 0, glw, glh);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ////glViewport(0, 0, width, height);
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
    qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain glcreate pos_buf: "<<pos_buf;
    glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(spos), spos, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, spossize, spos, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "position");
    qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain position loc: "<<loc;
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    {
    glGenBuffers(1, &tex_coord_buf);
    qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain glcreate tex_coord_buf: "<<tex_coord_buf;
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordpos), coordpos, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "a_texCoord");
    qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain a_texCoord loc: "<<loc;
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //texture
    LoadUniformAllTexture(effectname, fragInfo, texture, texture2);

    //parament
    UniformAllParament(effectname, fragInfo);

    qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain start glDrawArrays";
    glDrawArrays(GL_TRIANGLES, 0, 6);
    qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain end glDrawArrays";
    glUseProgram(0);
    if(finalFboinfo)
    {
#if 0
        QImage imagefrag=finalFboinfo->qfbo->toImage();
        static int iImage=0;
        bool bwrite=imagefrag.save(QString("fbofrag%1.jpg").arg(++iImage));
        qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain save image :" <<iImage<<" bwrite: "
            <<bwrite<<" width: "<<imagefrag.width()<<" height: "<<imagefrag.height()
            <<" bits: "<<imagefrag.bits();
        qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain idxFrameBuf: "<<finalFboinfo->idxFrameBuf
            << "qfbo: "<<finalFboinfo->qfbo
            <<" fbotexture: "<<finalFboinfo->fbotexture<<" qfbotexture: "<<(finalFboinfo->qfbo?finalFboinfo->qfbo->texture():0);
        //Display(imagefrag);
        glFlush();
#endif
        //if(getGPUInfo()==GPUT_UNKOWN || getGPUInfo()==GPUT_NVIDIA)
        {
#if 0
            glFlush();
            char tmp[10];
            glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)tmp);
#else
            glFinish();
#endif
        }
#ifdef USE_ORI_FBO
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
#else
        qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain samples :" <<finalFboinfo->qfbo->format().samples()<<" fbo.size: "<<finalFboinfo->qfbo->size();
        ////finalFboinfo->qfbo->bindDefault();
        QGLFramebufferObject::bindDefault();
#endif
    }
    if(!glIsTexture(texture))
    {
        qInfo()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain error texture: "<<texture;
    }

#ifdef OUTPUT_WASTE
    dt = startTime.msecsTo(QTime::currentTime());
    //dt = (int)(glfwGetTime()*1000.0f)-startTime2;
    qInfo()<< "GLHiddenWidget::fragRenderForOtherThread waste_time frag. time: " << dt
        <<"idxFrameBuf: "<<(finalFboinfo?finalFboinfo->idxFrameBuf:0);
#endif
    if(!finalFboinfo)
    {
        ////glfwSwapBuffers(m_gs->window); //uncomplete
#ifdef OUTPUT_WASTE
        dtall = startTime.msecsTo(QTime::currentTime());
        //dtall = (int)(glfwGetTime()*1000.0f)-startTime2;
        qInfo()<< "GLHiddenWidget::fragRenderForOtherThread waste_time glfwSwapBuffers. time: " 
            << dtall-dt;
#endif
    }

END_fragRenderForOtherThread:
#if 1
    if(pos_buf) {
        qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain gldelete pos_buf: "<<pos_buf;
        glDeleteBuffers(1, &pos_buf);
    }
    if(tex_coord_buf) {
        qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain gldelete tex_coord_buf: "<<tex_coord_buf;
        glDeleteBuffers(1, &tex_coord_buf);
    }
    for(int idx=texture2>0?2:1; idx<textureCnt; idx++)
    {
        qDebug()<<"GLHiddenWidget::baseFragRenderForAllThreadAgain gldelete textureId: "<<textureId[idx];
        glDeleteTextures(1, &textureId[idx]);
    }
#endif
    //doneCurrent();
    return (!iRtn && finalFboinfo)?finalFboinfo->idxFrameBuf:iRtn;
}
STFragmentInfo GLHiddenWidget::getFragmentInfo(const QString& effectname)
{
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLHiddenWidget::getFragmentInfo error. no such effect: "<<effectname;
        return STFragmentInfo();
    }
    return m_mapEffectProgram[effectname];
}
void GLHiddenWidget::deleteTextureOut(GLuint texture)
{
    qDebug()<<"GLHiddenWidget::deleteTextureOut gldelete texture: "<<texture;
    QMutexLocker locker(&m_mutexTexture); 
    vNeedDelTexture.push_back(texture);
    //makeCurrent();
    ////glDeleteTextures(1, &texture);
    //doneCurrent();
}
int GLHiddenWidget::makeCurrentOut()
{
    qDebug()<<"GLHiddenWidget::makeCurrentOut";
    if(QThread::currentThread() == getContextThread())
    {
        qDebug()<<"GLHiddenWidget::makeCurrentOut currentThread: "<<QThread::currentThread()
            <<" GLHiddenWidget thread: "<<getContextThread();
        makeCurrent();
    }
    else if(m_glwidget && QThread::currentThread() == m_glwidget->thread())
    {
        qDebug()<<"GLHiddenWidget::makeCurrentOut currentThread: "<<QThread::currentThread()
            <<" m_glwidget thread: "<<m_glwidget->thread();
        m_glwidget->makeCurrent();
    }
    else
    {
        qInfo()<<"GLHiddenWidget::makeCurrentOut error. no glhiddenwidget. currentThread: "
            <<QThread::currentThread()<<" GLDisplayWidget_thread: "<<m_glwidget->thread()<<" hiddencontex_thread: " <<getContextThread();
        return -1;
    }
    return 0;
}
int GLHiddenWidget::doneCurrentOut()
{
    qDebug()<<"GLHiddenWidget::doneCurrentOut";
    if(QThread::currentThread() == getContextThread())
    {
        qDebug()<<"GLHiddenWidget::doneCurrentOut currentThread: "<<QThread::currentThread()
            <<" glhiddenwidget thread: "<<getContextThread();
        doneCurrent();
    }
    else if(m_glwidget && QThread::currentThread() == m_glwidget->thread())
    {
        qDebug()<<"GLHiddenWidget::doneCurrentOut currentThread: "<<QThread::currentThread()
            <<" m_glwidget thread: "<<m_glwidget->thread();
        m_glwidget->doneCurrent();
    }
    else
    {
        qInfo()<<"GLHiddenWidget::doneCurrentOut error. no glhiddenwidget. currentThread: "<<QThread::currentThread()
            <<" GLDisplayWidget_thread: "<<m_glwidget->thread()<<" hiddencontex_thread: " <<getContextThread();
        return -1;
    }
    return 0;
}
void GLHiddenWidget::setGLDisplayWidget(GLDisplayWidget* glw)
{
    if(m_glwidget)
    {
        qInfo()<<"GLHiddenWidget::setGLDisplayWidget already have m_glwidget: "<<m_glwidget;
        return;
    }
    if(!glw)
    {
        qInfo()<<"GLHiddenWidget::setGLDisplayWidget glwidget: "<<glw<<" glhiddenwidget: "<<this;
        return;
    }
    m_glwidget=glw;
}
#endif
