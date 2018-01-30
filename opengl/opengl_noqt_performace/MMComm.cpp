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
"  //textureCoord = position* 0.5 + 0.5;\n"
"  textureCoord = gl_Position.xy * 0.5 + 0.5;\n"
"  //textureCoord = position* 0.5 + 1.0;\n"
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
//#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
    int dt;
//#endif
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
    QString fragVertex, fragSource;
    {
        QString fileName="vertex.frag";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qInfo()<<"MMGlobalContext::createProgram error can't read theme file: "<<fileName;
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
            qInfo()<<"MMGlobalContext::createProgram error can't read theme file: "<<fileName;
            return -2;
        }
        fragSource = file.readAll();
        file.close();
    }
    // build_program
    GLuint v_shader, f_shader;
    //v_shader = buildShader(v_shader_source, GL_VERTEX_SHADER);
    v_shader = buildShader(fragVertex.toLocal8Bit().constData(), GL_VERTEX_SHADER);
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
QVector<GLuint> MMGlobalContext::textures;
QVector<GLuint> MMGlobalContext::framebuffers;
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
}
MMGlobalContext* MMGlobalContext::instance()  
{  
    if(m_pInstance == NULL)  //判断是否第一次调用  
        m_pInstance = new MMGlobalContext();  
    return m_pInstance;  
}  
GLint MMGlobalContext::createAndSetupTexture()
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
#endif
    return textureId1;
}
// 创建两个纹理绑定到帧缓冲
void MMGlobalContext::createBindFramebufferTexture(int w, int h)
{
    for (int ii = 0; ii < 2; ++ii) {
        GLuint texture = createAndSetupTexture();
        textures.push_back(texture);

        //设置纹理大小和图像大小一致
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        // 创建一个帧缓冲
        // https://www.khronos.org/opengl/wiki/Framebuffer_Object
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        framebuffers.push_back(fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 绑定纹理到帧缓冲
        //     gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, texture, 0);
    }
}
GLint MMGlobalContext::load2DTexture(int w, int h, const unsigned char *pixels)
{
    GLuint textureId1=createAndSetupTexture();
#ifndef __UBUNTU__
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
        const unsigned char* bits, int width, int height, float globaltime, float totaltime, 
        const unsigned char* bits1, int width2, int height2)
{
    int iRtn=0;
    GLuint        program;
    GLint status;

    //VAO
    GLuint        pos_buf, tex_coord_buf;
    //float spos[12] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
#if 1
    // 512*384 -> 960*720 overlay 1280*720
    float spos[12] = { 
        160.0f, 0.0f,
        160.0f, 720.0f,
        1120.0f, 0.0f,
        160.0f, 720.0f,
        1120.0f, 0.0f,
        1120.0f, 720.0f
    };
#if 0
    float coordpos[12] = { 
        0.0f, 0.0f,
        0.0f, 720.0f,
        1280.0f, 0.0f,
        0.0f, 720.0f,
        1280.0f, 0.0f,
        1280.0f, 720.0f
    };
#endif
    float coordpos[12] = { 
        -1.0f, 1.0f, 
        -1.0f, -1.0f, 
        1.0f, 1.0f, 
        -1.0f, -1.0f, 
        1.0f, 1.0f, 
        1.0f, -1.0f
    };
#endif
    //texture
    GLuint textureId[MAX_TEXTURES_CNT];
    GLuint textureId2[MAX_TEXTURES_CNT];
    int cnt=0;
    int cnt2=0;


    if(!mapEffectProgram.contains(effectname))
    {
        qInfo()<<"MMGlobalContext::fragRenderForOtherThread error. no such effectname: "<<effectname;
        return -1;
    }
    STFragmentInfo& fragInfo=mapEffectProgram[effectname];
    program=fragInfo.program;
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

    glUseProgram(program);

    glUniform1f(glGetUniformLocation(program, "u_global_time"), globaltime);
    glUniform1f(glGetUniformLocation(program, "u_total_time"), totaltime);
    glUniform2f(glGetUniformLocation(program, "u_resolution"), m_gs->w, m_gs->h);

    //VAO
    {
    glGenBuffers(1, &pos_buf);
    glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spos), spos, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    {
    glGenBuffers(1, &tex_coord_buf);
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
    LoadUniformAllTexture(effectname, bits, width, height, bits1, width2, height2);

    //parament
    UniformAllParament(effectname);

    // 清除颜色缓冲区 重置为指定颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glViewport(0, 0, m_gs->w, m_gs->h);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteBuffers(1, &pos_buf);

END_fragRenderForOtherThread:
    return iRtn;
}

int MMGlobalContext::fragRenderForOtherThread(const QString& effectname, 
        const unsigned char* bits, float globaltime, float totaltime, 
        const unsigned char* bits1
        )
{
    QString m_effectName2="WS_OLD_PHOTO";
    int ii = 0;
    GLuint fbo ;
//#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
    int dt;
//#endif
    int iRtn=0;
#ifndef __UBUNTU__
    GLuint        program;
    //GLuint textureId0, textureId1, textureId2, textureId3, textureId4, textureId5;
    GLuint textureId[MAX_TEXTURES_CNT];
    GLuint textureId2[MAX_TEXTURES_CNT];
    int cnt=0;
    int cnt2=0;
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
        //textureId[cnt++]=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, (const unsigned char*)fragInfo.textures[idx].image->bits());
        std::shared_ptr<QImage> qimage=fragInfo.textures[idx].image;
        textureId[cnt++]=MMGlobalContext::load2DTexture(qimage->width(), qimage->height(), (const unsigned char*)qimage->bits());
        qDebug()<<"MMGlobalContext::fragRenderForOtherThread qimage->width: "<<qimage->width()<<" qimage->height: "<<qimage->height();
    }
#if 0
    if(fragInfo.textures.size()>=1)
    {
        textureId2=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, bits1);
    }
#endif
    startTime = QTime::currentTime();
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

    fbo = framebuffers[ii % 2];
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, m_gs->w, m_gs->h);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "MMGlobalContext::fragRenderForOtherThread waste_time frag. time: " << dt;

    //glBindVertexArray(0);
    ////glUseProgram(0);

    {
    //GLuint        program;
    if(!mapEffectProgram.contains(m_effectName2))
    {
        qInfo()<<"MMGlobalContext::fragRenderForOtherThread error. no such effectname: "<<m_effectName2;
        return -1;
    }
    program=mapEffectProgram[m_effectName2].program;
    glUseProgram(program);

    textureId2[cnt2++]=textures[ii % 2];
    for(int idx=0; idx<mapEffectProgram[m_effectName2].textures.size(); idx++)
    {
        //textureId2[cnt2++]=MMGlobalContext::load2DTexture(m_gs->w, m_gs->h, (const unsigned char*)mapEffectProgram[m_effectName2].textures[idx].image->bits());
        std::shared_ptr<QImage> qimage=mapEffectProgram[m_effectName2].textures[idx].image;
        textureId2[cnt2++]=MMGlobalContext::load2DTexture(qimage->width(), qimage->height(), (const unsigned char*)qimage->bits());
    }
    startTime = QTime::currentTime();
    for(int idx=0; idx<cnt2; idx++)
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
        glBindTexture(GL_TEXTURE_2D, textureId2[idx]);
        glUniform1i(glGetUniformLocation(program, arrTexturesVar[idx].constData()), idx);
    }

    glUniform1f(glGetUniformLocation(program, "u_global_time"), globaltime);
    glUniform1f(glGetUniformLocation(program, "u_total_time"), totaltime);

    glUniform2f(glGetUniformLocation(program, "u_resolution"), m_gs->w, m_gs->h);

    
    //fbo = framebuffers[ii % 2];
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //glViewport(0, 0, m_gs->w, m_gs->h);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);

    dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "MMGlobalContext::fragRenderForOtherThread waste_time frag. time: " << dt;
    }


END_fragRenderForOtherThread:
//#ifdef OUTPUT_WASTE
    dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "MMGlobalContext::fragRenderForOtherThread waste_time frag. time: " << dt
        <<" start_time: "<<startTime.toString()<<" QThread::currentThreadId(): "<<QThread::currentThreadId();
//#endif

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
#if 0
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
#endif
        qDebug()<<"MMGlobalContext::ParseConfCreateProgram textInfo.filename :"<<textInfo.filename
            <<" image_w: " <<image->width()<<" image_h: "<<image->height()<<" gs_w: "<<m_gs->w
            <<" h: "<<m_gs->h;
        if(image->format()!=QImage::Format_RGBA8888)
        {
            qDebug()<<"MMGlobalContext::ParseConfCreateProgram converFormat format: "<<image->format()
                <<" QImage::Format_RGBA8888: "<<QImage::Format_RGBA8888;
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
int MMGlobalContext::LoadUniformAllTexture(const QString& effectname
        , const unsigned char* bits, int width, int height
        , const unsigned char* bits1, int width2, int height2)
{
    GLuint textureId[MAX_TEXTURES_CNT];
    int cnt=0;
    STFragmentInfo& fragInfo=mapEffectProgram[effectname];
    GLuint program=fragInfo.program;

    textureId[cnt++]=MMGlobalContext::load2DTexture(width, height, bits);
    if(bits1)
    {
        textureId[cnt++]=MMGlobalContext::load2DTexture(width2, height2, bits1);
    }
    for(int idx=0; idx<fragInfo.textures.size(); idx++)
    {
        std::shared_ptr<QImage> qimage=fragInfo.textures[idx].image;
        textureId[cnt++]=MMGlobalContext::load2DTexture(qimage->width(), qimage->height(), (const unsigned char*)qimage->bits());
        qDebug()<<"MMGlobalContext::LoadUniformAllTexture qimage->width: "<<qimage->width()<<" qimage->height: "<<qimage->height();
    }
    for(int idx=0; idx<cnt; idx++)
    {
        glActiveTexture(GL_TEXTURE0+idx);
        glBindTexture(GL_TEXTURE_2D, textureId[idx]);
        glUniform1i(glGetUniformLocation(program, arrTexturesVar[idx].constData()), idx);
    }
    return 0;
}
int MMGlobalContext::UniformAllParament(const QString& effectname)
{
    STFragmentInfo& fragInfo=mapEffectProgram[effectname];
    GLuint program=fragInfo.program;

    for(int idx=0; idx<fragInfo.paras.size(); idx++)
    {
        STParaInfo& para = fragInfo.paras[idx];
        if(para.type.compare("float")==0)
        {
            qDebug()<<"MMGlobalContext::UniformAllParament type: "<<para.type<<"name: "
                <<para.name<<" deflt: " <<para.deflt;
            glUniform1f(glGetUniformLocation(program, para.name.constData()), para.deflt.toFloat());
        }
        else if(para.type.compare("int")==0)
        {
            glUniform1i(glGetUniformLocation(program, para.name.constData()), para.deflt.toInt());
        }
        else
        {
            qInfo()<<"MMGlobalContext::UniformAllParament error type: "<<para.type<<" effectname: "<<effectname;
        }
    }
    return 0;
}
