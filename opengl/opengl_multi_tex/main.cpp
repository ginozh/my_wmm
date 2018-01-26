#include "comm.h"
#include <QApplication>
#include <QDebug>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QImage>
#include <memory>
#include <QFile>
#include <QTime>


#include <fstream>
#include <iostream>
using namespace std;

#if 0
int render(GenericShaderContext *gs, const uchar* bits, const uchar* bits1)
{
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    // 清除颜色缓冲区 重置为指定颜色
    glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 这里填写场景绘制代码
    //glBindVertexArray(VAOId);
    {
        glGenBuffers(1, &gs->pos_buf);
        glBindBuffer(GL_ARRAY_BUFFER, gs->pos_buf);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

        // Step4: 指定解析方式  并启用顶点属性
        // 顶点位置属性
        GLint loc = glGetAttribLocation(gs->program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    //shader.use();
    glUseProgram(gs->program);
    // 启用多个纹理单元 绑定纹理对象
    GLuint textureId0, textureId1;
    textureId0=GlobalContext::load2DTexture(512, 384, bits);
    textureId1=GlobalContext::load2DTexture(512, 384, bits1);
    // 启用多个纹理单元 绑定纹理对象
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    //glUniform1i(glGetUniformLocation(shader.programId, "tex1"), 0); // 设置纹理单元为0号
    glUniform1i(glGetUniformLocation(gs->program, "mSample0"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glUniform1i(glGetUniformLocation(gs->program, "mSample1"), 1);
    
    glUniform1f(glGetUniformLocation(gs->program, "u_global_time"), 1.0);
    glUniform1f(glGetUniformLocation(gs->program, "u_total_time"), 2.0);

    glUniform2f(glGetUniformLocation(gs->program, "u_resolution"), 512, 384);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //glBindVertexArray(0);
    glUseProgram(0);

#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< " waste_time frag. time: " << dt
        <<" start_time: "<<startTime.toString();
#endif
    return 0;
}
#endif
void opengl()
{
    static int i=0;
    if(i>=1)
    {
        //qDebug()<<"i: "<<i;
        return;
    }
    i++;
    GenericShaderContext* gs=GlobalContext::instance()->m_gs;
    if(!gs)
    {
        qDebug()<<"gs is null ";
        return;
    }
#if 0
    qDebug()<<"gs->program: "<<gs->program<<" gs->v_shader: "<<gs->v_shader;
    if (!glIsShader(gs->f_shader)) {
        qInfo()<<"error gs->f_shader is not shader";
    }
    if (!glIsProgram(gs->program)) {
        qInfo()<<"error gs->program is not program";
    }
#if 0
    gs->program = glCreateProgram();
    glAttachShader(gs->program, gs->v_shader);
    glAttachShader(gs->program, gs->f_shader);
    glLinkProgram(gs->program);
#endif

    GLint status;
    glGetProgramiv(gs->program, GL_LINK_STATUS, &status);
    qDebug()<<"Fragment 6";
    if (status != GL_TRUE) {
        qInfo()<<"error glGetProgramiv";
    }
#endif
    {
        QString fileName="c:\\shareproject\\jpg\\512img001.jpg";
        QImage image(fileName);
        if (image.isNull()) {
            qDebug()<<"image.isNull";
            return;
        }
        image = image.convertToFormat(QImage::Format_RGBA8888);
        //image = image.convertToFormat(QImage::Format_RGB888);
        unsigned char *pixels = (unsigned char *) image.bits();

        QString fileName2="c:\\shareproject\\jpg\\512img002.jpg";
        QImage image2(fileName2);
        if (image2.isNull()) {
            qDebug()<<"image2.isNull";
            return;
        }
        image2 = image2.convertToFormat(QImage::Format_RGBA8888);
        //image2 = image2.convertToFormat(QImage::Format_RGB888);
        unsigned char *pixels2 = (unsigned char *) image2.bits();
        //f->glReadPixels(0,0,gs->w,gs->h,GL_RGBA,GL_UNSIGNED_BYTE, imagefrag.bits());
        //render(gs, pixels, pixels2);
        GlobalContext::instance()->fragRenderForOtherThread("ColourDistance", pixels, 1.0, 2.0, pixels2);
        
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gs->w, gs->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, pixels);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glFlush();
        //glFinish();
        //QImage imagefrag(gs->w,gs->h,QImage::Format_RGB888);
        QImage imagefrag(gs->w,gs->h,QImage::Format_RGBA8888);
        unsigned char *mask_pixels = (unsigned char *) imagefrag.bits();
        glReadPixels(0, 0, gs->w, gs->h, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
        //glReadPixels(0, 0, gs->w, gs->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
        imagefrag.save("c:\\shareproject\\jpg\\512img004_frag.jpg");
        //screenshot("c:\\shareproject\\jpg\\512img005_frag.bmp");
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GlobalContext::instance()->initialOpengl(512,384);
    GlobalContext::instance()->createProgram("ColourDistance");

    opengl();
    return NULL;//app.exec();
}
