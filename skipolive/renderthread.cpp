#include "renderthread.h"

#include <QApplication>
#include <QImage>
#include <QOpenGLFunctions>
#include <QDebug>

#include "OpenGLWidget.h"
#include "Clip.h"
#include "project/effect.h"
#include "ui/collapsiblewidget.h"

#include "io/math.h"


void saveOpenglBuffer(QString prefix); //storm
#define GL_DEFAULT_BLENDt glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE)

GLuint draw_clipt(QOpenGLContext* ctx, QOpenGLFramebufferObject* fbo, GLuint texture, bool clear) 
{
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);

	GLint current_fbo = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_fbo);

	fbo->bind();

	if (clear) glClear(GL_COLOR_BUFFER_BIT);

	// get current blend mode
	GLint src_rgb, src_alpha, dst_rgb, dst_alpha;
	glGetIntegerv(GL_BLEND_SRC_RGB, &src_rgb);
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &src_alpha);
	glGetIntegerv(GL_BLEND_DST_RGB, &dst_rgb);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &dst_alpha);

	ctx->functions()->glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); // top left
	glVertex2f(0, 0); // top left
	glTexCoord2f(1, 0); // top right
	glVertex2f(1, 0); // top right
	glTexCoord2f(1, 1); // bottom right
	glVertex2f(1, 1); // bottom right
	glTexCoord2f(0, 1); // bottom left
	glVertex2f(0, 1); // bottom left
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

//	fbo->release();
	ctx->functions()->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_fbo);

	// restore previous blendFunc
	ctx->functions()->glBlendFuncSeparate(src_rgb, dst_rgb, src_alpha, dst_alpha);

	//if (default_fbo != nullptr) default_fbo->bind();

	glPopMatrix();
	return fbo->texture();
}
/*
void process_effectt(QOpenGLContext* ctx,
					Clip* c,
					Effect* e,
					double timecode,
					GLTextureCoords& coords,
					GLuint& composite_texture,
					bool& fbo_switcher,
					bool& texture_failed,
					int data) 
{
	if (e->is_enabled()) {
		if (e->enable_coords) {
			e->process_coords(timecode, coords, data);
		}
		if ((e->enable_shader && shaders_are_enabled) || e->enable_superimpose) {
			e->startEffect();
			if ((e->enable_shader && shaders_are_enabled) && e->is_glsl_linked()) {
				e->process_shader(timecode, coords);
				composite_texture = draw_clipt(ctx, c->fbo[fbo_switcher], composite_texture, true);
				fbo_switcher = !fbo_switcher;
			}
			if (e->enable_superimpose) {
				GLuint superimpose_texture = e->process_superimpose(timecode);
				if (superimpose_texture == 0) {
					qWarning() << "Superimpose texture was nullptr, retrying...";
					texture_failed = true;
				} else {
					composite_texture = draw_clipt(ctx, c->fbo[!fbo_switcher], superimpose_texture, false);
				}
			}
			e->endEffect();
		}
	}
}
*/
RenderThread::RenderThread() :
	frameBuffer(0),
	texColorBuffer(0),
	gizmos(nullptr),
	share_ctx(nullptr),
	ctx(nullptr),
	queued(false),
	texture_failed(false),
	running(true)
{
	surface.create();
}

RenderThread::~RenderThread() {
	surface.destroy();
}

void RenderThread::run() 
{
	mutex.lock();

	while (running) 
    {
		if (!queued) 
        {
			waitCond.wait(&mutex);
		}
		if (!running) 
        {
			break;
		}
        ///emit start_create_effect_ui(clip);
        if(clip->effects.size()<=0) clip->createEffect();
		queued = false;


		if (share_ctx != nullptr) 
        {
			if (ctx != nullptr) 
            {
                qDebug()<<"RenderThread::run";
				ctx->makeCurrent(&surface);

				// gen fbo
				if (frameBuffer == 0) 
                {
					delete_fbo();
					ctx->functions()->glGenFramebuffers(1, &frameBuffer);
				}

				// bind
				ctx->functions()->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

				// gen texture
				if (texColorBuffer == 0 ) 
                {
					delete_texture();
					glGenTextures(1, &texColorBuffer);
                    qDebug()<<"RenderThread::run glGenTextures texColorBuffer: "<<texColorBuffer<<" frameBuffer: "<<frameBuffer;
					glBindTexture(GL_TEXTURE_2D, texColorBuffer);
					glTexImage2D(
						GL_TEXTURE_2D, 0, GL_RGB, m_glwidget->glw, m_glwidget->glh, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
					);
					//tex_width = seq->width;
					//tex_height = seq->height;
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					ctx->functions()->glFramebufferTexture2D(
						GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
					);
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				// draw
				paint();

				// flush changes
//				glFlush();
				glFinish();
                ////saveOpenglBuffer("RenderThread_run_afterpaint"); //storm
				// release
				ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER, 0);

				emit ready();
			}
		}
	}

	delete_ctx();

	mutex.unlock();
}
GLuint RenderThread::compose_sequencet() 
{
    GLint current_fbo = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_fbo);
    qDebug()<<"RenderThread::compose_sequencet current_fbo: "<<current_fbo;

    int half_width = m_glwidget->glw/2;//s->width/2;
    int half_height = m_glwidget->glh/2;//s->height/2;

    glPushMatrix();
    glLoadIdentity();
    glOrtho(-half_width, half_width, -half_height, half_height, -1, 10);
    ctx->functions()->glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
    glColor4f(1.0, 1.0, 1.0, 1.0);

    GLuint textureID = 0;
    int video_width = 1024;//1280;//c->getWidth();
    int video_height = 768;//720;//c->getHeight();
    // set up opengl texture
    if (texture == nullptr) 
    {
        texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->setSize(video_width, video_height);
        texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
        texture->setMipLevels(texture->maximumMipLevels());
        texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        texture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
        qDebug()<<"texture new QOpenGLTexture. width: "<<video_width<<" height: "<<video_height
            <<" format: "<<QOpenGLTexture::RGBA8_UNorm<<" fixfmt: "<<QOpenGLTexture::RGBA;
    }
#if 0
    get_clip_frame(c, qMax(playhead, c->timeline_in), texture_failed);
#else
    {
        static QImage* image=nullptr;
        if(image==nullptr)
        {
            image=new QImage("\\\\Mac\\Home\\Desktop\\upan\\jpg\\1_ori_scale.jpg");
            //image=new QImage("\\\\Mac\\Home\\Desktop\\upan\\jpg\\5.jpg");
            image=new QImage(image->convertToFormat(QImage::Format_RGBA8888));
        }
        texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,image->bits());
    }
#endif
    textureID = texture->textureId();
    glPushMatrix();

    // start preparing cache
    if (fbo == nullptr) 
    {
        fbo = new QOpenGLFramebufferObject* [2];
        fbo[0] = new QOpenGLFramebufferObject(video_width, video_height);
        fbo[1] = new QOpenGLFramebufferObject(video_width, video_height);
        ctx->functions()->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_fbo);
    }

    // clear fbos
    /*c->fbo[0]->bind();
      glClear(GL_COLOR_BUFFER_BIT);
      c->fbo[0]->release();
      c->fbo[1]->bind();
      glClear(GL_COLOR_BUFFER_BIT);
      c->fbo[1]->release();*/


    bool fbo_switcher = false;

    glViewport(0, 0, video_width, video_height);

    GLuint composite_texture;
    //composite_texture = draw_clipt(ctx, c->fbo[fbo_switcher], textureID, true);
    composite_texture = draw_clipt(ctx, fbo[fbo_switcher], textureID, true);

    fbo_switcher = !fbo_switcher;

    // set up default coords
    GLTextureCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.grid_size = 1;
    coords.vertexTopLeftX = coords.vertexBottomLeftX = -video_width/2;
    coords.vertexTopLeftY = coords.vertexTopRightY = -video_height/2;
    coords.vertexTopRightX = coords.vertexBottomRightX = video_width/2;
    coords.vertexBottomLeftY = coords.vertexBottomRightY = video_height/2;
    coords.vertexBottomLeftZ = coords.vertexBottomRightZ = coords.vertexTopLeftZ = coords.vertexTopRightZ = 1;
    coords.textureTopLeftY = coords.textureTopRightY = coords.textureTopLeftX = coords.textureBottomLeftX = 0.0;
    coords.textureBottomLeftY = coords.textureBottomRightY = coords.textureTopRightX = coords.textureBottomRightX = 1.0;
    coords.textureTopLeftQ = coords.textureTopRightQ = coords.textureTopLeftQ = coords.textureBottomLeftQ = 1;
    // EFFECT CODE START
    double timecode = 0;//get_timecode(c, playhead);

    //Effect* first_gizmo_effect = nullptr;
    //Effect* selected_effect = nullptr;
    qDebug()<<"clip: "<<clip;
    if(clip && clip->effects.size()>0)
    {
        Effect* e = clip->effects.at(0);
        //process_effectt(ctx, c, e, timecode, coords, composite_texture, fbo_switcher, texture_failed, TA_NO_TRANSITION);
        e->process_coords(timecode, coords, 0);
        gizmos = e;
        //crop
        {
			if(!glslProgram) glslProgram = new QOpenGLShaderProgram();
            if(!isOpen)
            {
                if (glslProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Effects/common.vert")
                        && glslProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Effects/crop.frag")
                   ) 
                {
                    qInfo() << "Vertex Fragment shader added successfully";
                    isOpen = true;
                }
                else
                {
                    qInfo()<<"Fragment add error";
                }
            }
            if (isOpen &&glslProgram->link()) 
            {
                qInfo() << "Shader program linked successfully";
                bool bound = glslProgram->bind();
                glslProgram->setUniformValue("resolution", m_glwidget->glw, m_glwidget->glh);
                glslProgram->setUniformValue("time", GLfloat(timecode));
                glslProgram->setUniformValue("left", GLfloat(50));
                glslProgram->setUniformValue("top", GLfloat(0));
                glslProgram->setUniformValue("right", GLfloat(0));
                glslProgram->setUniformValue("bottom", GLfloat(0));
                glslProgram->setUniformValue("feather", GLfloat(0));
                composite_texture = draw_clipt(ctx, fbo[fbo_switcher], composite_texture, true);
                fbo_switcher = !fbo_switcher;
                if (bound) glslProgram->release();
            }
            else
            {
                qInfo()<<"link error";
            }
        }
    }
    glViewport(0, 0, m_glwidget->glw, m_glwidget->glh);

    glBindTexture(GL_TEXTURE_2D, composite_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);

    ////GLint current_fbo1 = 0;
    ////glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_fbo1);

    if (coords.grid_size <= 1) {
        float z = 0.0f;

        glTexCoord2f(coords.textureTopLeftX, coords.textureTopLeftY); // top left
        glVertex3f(coords.vertexTopLeftX, coords.vertexTopLeftY, z); // top left
        glTexCoord2f(coords.textureTopRightX, coords.textureTopRightY); // top right
        glVertex3f(coords.vertexTopRightX, coords.vertexTopRightY, z); // top right
        glTexCoord2f(coords.textureBottomRightX, coords.textureBottomRightY); // bottom right
        glVertex3f(coords.vertexBottomRightX, coords.vertexBottomRightY, z); // bottom right
        glTexCoord2f(coords.textureBottomLeftX, coords.textureBottomLeftY); // bottom left
        glVertex3f(coords.vertexBottomLeftX, coords.vertexBottomLeftY, z); // bottom left
    } 
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
    /////saveOpenglBuffer("renderfunctions_compose_sequence"); //storm

    // prepare gizmos
    if (gizmos != nullptr
            /*&& nests.isEmpty()
              && (gizmos == first_gizmo_effect
              || (gizmos == selected_effect)*/) {
        gizmos->gizmo_draw(timecode, coords); // set correct gizmo coords
        gizmos->gizmo_world_to_screen(); // convert gizmo coords to screen coords
    }
    glPopMatrix();
    glPopMatrix();
    return 0;
}

void RenderThread::paint() 
{
	glLoadIdentity();

	texture_failed = false;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH);

	gizmos = nullptr;
	QVector<Clip*> nests;
	compose_sequencet();
    qDebug()<<"RenderThread::paint gizmos: "<<gizmos;
    ////static int idx=0;save_fn=QString("paint_after_compose_%1.png").arg(++idx);
	if (!save_fn.isEmpty()) {
		if (texture_failed) {
			// texture failed, try again
			queued = true;
		} else {
			ctx->functions()->glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
			QImage img(m_glwidget->glw, m_glwidget->glh, QImage::Format_RGBA8888);
			glReadPixels(0, 0, m_glwidget->glw, m_glwidget->glh, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
			img.save(save_fn);
			ctx->functions()->glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			save_fn = "";
		}
	}

	if (pixel_buffer != nullptr) {
		ctx->functions()->glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
		glReadPixels(0, 0, m_glwidget->glw, m_glwidget->glh, GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer);
		ctx->functions()->glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		pixel_buffer = nullptr;
	}

	glDisable(GL_DEPTH);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void RenderThread::start_render(QOpenGLContext *share, void *s, const QString& save, GLvoid* pixels, int idivider) {
    qDebug()<<"RenderThread::start_render save: "<<save<<" pixels: "<<pixels;
	///seq = s;

	// stall any dependent actions
	texture_failed = true;

	if (share != nullptr && (ctx == nullptr || ctx->shareContext() != share_ctx)) {
        qDebug()<<"RenderThread::start_render setShareContext";
		share_ctx = share;
		delete_ctx();
		ctx = new QOpenGLContext();
		ctx->setFormat(share_ctx->format());
		ctx->setShareContext(share_ctx);
		ctx->create();
		ctx->moveToThread(this);
	}

	save_fn = save;
	pixel_buffer = pixels;

	queued = true;
	waitCond.wakeAll();
    qDebug()<<"RenderThread::start_render waitCond.wakeAll";
}

bool RenderThread::did_texture_fail() {
	return texture_failed;
}

void RenderThread::cancel() {
	running = false;
	waitCond.wakeAll();
    qDebug()<<"RenderThread::cancel waitCond.wakeAll";
	wait();
}

void RenderThread::delete_texture() {
	if (texColorBuffer > 0) {
		ctx->functions()->glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0
		);
		glDeleteTextures(1, &texColorBuffer);
	}
	texColorBuffer = 0;
}

void RenderThread::delete_fbo() {
	if (frameBuffer > 0) {
		ctx->functions()->glDeleteFramebuffers(1, &frameBuffer);
	}
	frameBuffer = 0;
}

void RenderThread::delete_ctx() {
	if (ctx != nullptr) {
		delete_texture();
		delete_fbo();
		ctx->doneCurrent();
		delete ctx;
	}
	ctx = nullptr;
}
void RenderThread::setGLWidget(OpenGLWidget* widget)
{
    m_glwidget=widget;
    if(clip==nullptr)
    {
        clip = new Clip();
        clip->m_glwidget=this->m_glwidget;
    }
    connect(this, SIGNAL(start_create_effect_ui(Clip*)), m_glwidget, SLOT(create_effect_ui(Clip*)), Qt::QueuedConnection);
}
