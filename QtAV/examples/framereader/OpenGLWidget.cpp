
#include "OpenGLWidget.h"

#include <QOpenGLFramebufferObject> //storm
#include <QOpenGLContext> 
#include <QThreadStorage> 
#include <QImage> 
#include <QMatrix4x4> 
#include <QSurface> 
#include <QScreen> 
#include "QtAV/VideoShader.h"
#include "QtAV/GeometryRenderer.h"
#include "QtAV/OpenGLVideo.h"
#include "QtAV/SurfaceInterop.h"
#include "opengl/ShaderManager.h"
#include <sys/time.h> //storm
#include <QDebug>

#include "opengl/OpenGLHelper.h"
#include "QtAV/VideoRenderer.h"
#include "QtAV/private/VideoRenderer_p.h"
#include "QtAV/Filter.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include "QtAV/Statistics.h"
#include "QtAV/private/factory.h"
#include "QtAV/private/mkid.h"
///#include "utils/Logger.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QDrag> 
#include <QMimeData> 
#include <QCursor> 

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    qInfo()<<"OpenGLWidget::OpenGLWidget";
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	setFormat(format);

	doneCurrent();
}

OpenGLWidget::~OpenGLWidget()
{
    qInfo()<<"OpenGLWidget::~OpenGLWidget";
}

void OpenGLWidget::initializeGL()
{
    qInfo()<<"OpenGLWidget::initializeGL context: "<<context()
         <<" isValid: "<<isValid();
    initializeOpenGLFunctions();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    qInfo()<<"OpenGLWidget::resizeGL width: "<<width<<" height: "<<height<<"  context: "<<context()<<" isValid: "<<isValid();
    glvieww=width;
    glviewh=height;
    QOpenGLWidget::resizeGL(width, height);
}
void OpenGLWidget::paintGL()
{
    qInfo()<<"OpenGLWidget::paintGL start";
    static QOpenGLFramebufferObject *fbo=NULL;
    if(video_frame && video_frame.isValid())
    {
        VideoFrame *frame= &video_frame;
        static VideoMaterial *material=NULL;
        static ShaderManager *manager=NULL;
        static QMatrix4x4 matrix;
        static QRectF rect;
        GLint currentFbo = 0;
        DYGL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo));
        if(!fbo)
        {
            fbo = new QOpenGLFramebufferObject(frame->size(), GL_TEXTURE_2D); //TODO: prefer 16bit rgb
            QOpenGLContext *ctx = const_cast<QOpenGLContext*>(QOpenGLContext::currentContext()); //qt4 returns const
            //d.glv.setOpenGLContext(ctx);
            {
                qreal b = 0, c = 0, h = 0, s = 0;
                if (!material) {
                    material = new VideoMaterial();
                    material->setBrightness(b);
                    material->setContrast(c);
                    material->setHue(h);
                    material->setSaturation(s);
                }
                if (!manager)
                {
                    // TODO: what if ctx is delete?
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                    manager = new ShaderManager(ctx);
                    ////QObject::connect(ctx, SIGNAL(aboutToBeDestroyed()), this, SLOT(resetGL()), Qt::DirectConnection); // direct to make sure there is a valid context. makeCurrent in window.aboutToBeDestroyed()?
#endif
                    //manager->setObjectName(QStringLiteral("__qtav_shader_manager"));
                    /// get gl info here because context is current(qt ensure it)
                    //const QByteArray extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));
                }
                bool hasGLSL = QOpenGLShaderProgram::hasOpenGLShaderPrograms();
                qDebug("OpenGL version: %d.%d  hasGLSL: %d", ctx->format().majorVersion(), ctx->format().minorVersion(), hasGLSL);
            }
            qDebug("new fbo texture: %d %dx%d", fbo->texture(), fbo->width(), fbo->height());
        }
        fbo->bind();
        DYGL(glViewport(0, 0, fbo->width(), fbo->height()));
        ///d.glv.setCurrentFrame(*frame);
        {
            material->setCurrentFrame(*frame);
        }
        QMatrix4x4 mat; // flip vertical
        mat.scale(1, -1);
        ///d.glv.render(QRectF(), QRectF(), mat); 
        // src/opengl/OpenGLVideo.cpp  344
        static VideoShader *shader = NULL; 
        static qreal valiad_tex_width=0;
        static bool update_geo=false;
        //static bool update_geo=true;
        static int tex_target=0;
        static QSize video_size;
        static GeometryRenderer* gr=NULL;
        static OpenGLVideo::MeshType mesh_type=OpenGLVideo::RectMesh;
        static bool norm_viewport=true;
        static TexturedGeometry *geometry=NULL;
        {
            QRectF target=QRectF();
            QRectF roi=QRectF();
            QMatrix4x4 transform=mat;
            const qint64 mt = material->type();
            if(material->bind())
            {
                if (!shader)
                    shader = manager->prepareMaterial(material, mt); //TODO: print shader type name if changed. prepareMaterial(,sample_code, pp_code)
                DYGL(glViewport(0, 0, fbo->width(), fbo->height()));
                shader->update(material);
                shader->program()->setUniformValue(shader->matrixLocation(), transform*matrix);
                //d.updateGeometry(shader, target, roi);
                if(1){
                    QRectF t=target;
                    QRectF r=roi;

                    if (!gr) { // TODO: only update VAO, not the whole GeometryRenderer
                        GeometryRenderer *r = new GeometryRenderer(); // local var is captured by lambda 
                        gr = r;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && defined(Q_COMPILER_LAMBDA)
                        QObject::connect(QOpenGLContext::currentContext(), &QOpenGLContext::aboutToBeDestroyed, [r]{
                                qDebug("destroy GeometryRenderer %p", r);
                                delete r;
                                });
#endif
                    }
                    const bool roi_changed = valiad_tex_width != material->validTextureWidth() || roi != r || video_size != material->frameSize();
                    const int tc = shader->textureLocationCount();
                    QRectF target_rect = norm_viewport ? QRectF(-1, 1, 2, -2) : rect;
#if 1
                    if (tex_target != shader->textureTarget()) {
                        tex_target = shader->textureTarget();
                        update_geo = true;
                    }
                    if (target.isValid()) {
                        if (roi_changed || target != t) {
                            target = t;
                            update_geo = true;
                            //target_rect = target (if valid). // relate to gvf bug?
                        }
                    } else {
                        if (roi_changed) {
                            update_geo = true;
                        }
                    }
#endif
                    if (update_geo)
                    {
                        if( geometry ) delete geometry;
                        geometry = NULL;
                        if (mesh_type == OpenGLVideo::SphereMesh)
                            geometry = new Sphere();
                        else
                            geometry = new TexturedGeometry();
                        //qDebug("updating geometry...");
                        // setTextureCount may change the vertex data. Call it before setRect()
                        qDebug() << "target rect: " << target_rect ;
                        geometry->setTextureCount(shader->textureTarget() == GL_TEXTURE_RECTANGLE ? tc : 1);
                        geometry->setGeometryRect(target_rect);
                        geometry->setTextureRect(material->mapToTexture(0, roi));
                        if (shader->textureTarget() == GL_TEXTURE_RECTANGLE) {
                            for (int i = 1; i < tc; ++i) {
                                // tc can > planes, but that will compute chroma plane
                                geometry->setTextureRect(material->mapToTexture(i, roi), i);
                            }
                        }
                        geometry->create();
                        update_geo = false;
                        {
                            static int vbo_size=0; // QOpenGLBuffer.size() may get error 0x501
                            static QOpenGLBuffer vbo; //VertexBuffer
#if QT_VAO
                            static QOpenGLVertexArrayObject vao;
#endif //QT_VAO
                            static QOpenGLBuffer ibo;

                            // geometry characteristic
                            static int stride;
                            static QVector<Attribute> attrib;
                            Geometry *g=geometry; //static 会改变g的值?可能static变量太多了

                            //gr->updateGeometry(geometry); //create vbo, vao
                            // src/opengl/GeometryRenderer.cpp  83
                            // void GeometryRenderer::updateGeometry(Geometry *geo)
                            //    g = geo;
                            //    vbo.create(); // QOpenGLBuffer vbo
                            //    vbo.bind();
                            //    vao.create(); // QOpenGLVertexArrayObject vao
                            //    vao.bind();
                            //    QGLF(glVertexAttribPointer(an, a.tupleSize(), a.type(), a.normalize(), g->stride(), reinterpret_cast<const void *>(qptrdiff(a.offset())))); //TODO: in setActiveShader
                            //    QGLF(glEnableVertexAttribArray(an));
                            if(1){
                                static int support_map = 1;
                                if (!vbo.isCreated()) {
                                    qDebug("creating VBO...");
                                    if (!vbo.create())
                                        qWarning("VBO create error");
                                }
                                if (vbo.isCreated()) {
                                    vbo.bind();
                                    const int bs = g->vertexCount()*g->stride();
                                    if (bs == vbo_size) { // vbo.size() error 0x501 on rpi, and query gl value can be slow
                                        void* p = NULL;
                                        if (support_map )
                                            p = vbo.map(QOpenGLBuffer::WriteOnly);
                                        if (p) {
                                            memcpy(p, g->constVertexData(), bs);
                                            vbo.unmap();
                                        } else {
                                            vbo.write(0, g->constVertexData(), bs);
                                            vbo_size = bs;
                                        }
                                    } else {
                                        vbo.allocate(g->vertexData(), bs);
                                    }
                                    vbo.release();
                                }
#if QT_VAO
                                if (stride == g->stride() && attrib == g->attributes())
                                {
                                }
                                else
                                {
                                    stride = g->stride();
                                    attrib = g->attributes();

                                    if (!vao.isCreated()) {
                                        qDebug("creating VAO...");
                                        if (!vao.create())
                                            qDebug("VAO create error");
                                    }
                                    qDebug("vao updated");
                                    if (vao.isCreated()) // can not use vao binder because it will create a vao if necessary
                                        vao.bind();
                                    // can set data before vao bind
                                    if (vao.isCreated())
                                        qDebug("geometry attributes changed, rebind vao...");
                                }
                                // call once is enough if no feature and no geometry attribute is changed
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#define QGLF(f) QOpenGLContext::currentContext()->functions()->f
#else
#define QGLF(f) QGLFunctions(NULL).f
#endif
                                if (vbo.isCreated()) {
                                    vbo.bind();
                                    for (int an = 0; an < g->attributes().size(); ++an) {
                                        // FIXME: assume bind order is 0,1,2...
                                        const Attribute& a = g->attributes().at(an);
                                        QGLF(glVertexAttribPointer(an, a.tupleSize(), a.type(), a.normalize(), g->stride(), reinterpret_cast<const void *>(qptrdiff(a.offset())))); //TODO: in setActiveShader
                                        QGLF(glEnableVertexAttribArray(an));
                                    }
                                    vbo.release(); // unbind after vao unbind? http://www.zwqxin.com/archives/opengl/vao-and-vbo-stuff.html
                                } // TODO: bind pointers if vbo is disabled
                                // bind ibo to vao thus no bind is required later
                                if (ibo.isCreated())// if not bind here, glDrawElements(...,NULL) crashes and must use ibo data ptr, why?
                                    ibo.bind();
                                vao.release();
                                if (ibo.isCreated())
                                    ibo.release();
#endif
                                qDebug("geometry updated");
                            }
                            // gr->render();
                            // ./src/opengl/GeometryRenderer.cpp  264
                            // =>void GeometryRenderer::render()
                            //    bindBuffers();
                            //    =>void GeometryRenderer::bindBuffers()
                            //        vao.bind(); // vbo, ibo is ok now
                            //    DYGL(glDrawArrays(g->primitive(), 0, g->vertexCount()));
                            if(1){
                                {
                                    bool bind_vbo = vbo.isCreated();
                                    bool bind_ibo = ibo.isCreated();
                                    bool setv_skip = false;
#if QT_VAO
                                    if (vao.isCreated()) {
                                        vao.bind(); // vbo, ibo is ok now
                                        setv_skip = bind_vbo;
                                        bind_vbo = false;
                                        bind_ibo = false;
                                    }
#endif
                                    //qDebug("bind ibo: %d vbo: %d; set v: %d", bind_ibo, bind_vbo, !setv_skip);
                                    if (bind_ibo)
                                        ibo.bind();
                                    // no vbo: set vertex attributes
                                    // has vbo, no vao: bind vbo & set vertex attributes
                                    // has vbo, has vao: skip
                                    if (!setv_skip && g)
                                    {
                                        const char* vdata = static_cast<const char*>(g->vertexData());
                                        if (bind_vbo) {
                                            vbo.bind();
                                            vdata = NULL;
                                        }
                                        for (int an = 0; an < g->attributes().size(); ++an) {
                                            const Attribute& a = g->attributes().at(an);
                                            QGLF(glVertexAttribPointer(an, a.tupleSize(), a.type(), a.normalize(), g->stride(), vdata + a.offset()));
                                            QGLF(glEnableVertexAttribArray(an)); //TODO: in setActiveShader
                                        }
                                    }
                                }

                                if (g->indexCount() > 0) { qDebug("GeometryRenderer::render primitive: %d indexCount: %d indexType: %d indexData: %p", g->primitive(), g->indexCount(), g->indexType(), g->indexData());
                                    DYGL(glDrawElements(g->primitive(), g->indexCount(), g->indexType(), ibo.isCreated() ? NULL : g->indexData())); // null: data in vao or ibo. not null: data in memory
                                } else {
                                    DYGL(glDrawArrays(g->primitive(), 0, g->vertexCount()));
                                }
                                {
                                    bool unbind_vbo = vbo.isCreated();
                                    bool unbind_ibo = ibo.isCreated();
                                    bool unsetv_skip = false;
#if QT_VAO
                                    if (vao.isCreated()) {
                                        vao.release();
                                        unsetv_skip = unbind_vbo;
                                        unbind_vbo = false;
                                        unbind_ibo = false;
                                    }
#endif //QT_VAO
                                    //qDebug("unbind ibo: %d vbo: %d; unset v: %d", unbind_ibo, unbind_vbo, !unsetv_skip);
                                    if (unbind_ibo)
                                        ibo.release();
                                    // release vbo. qpainter is affected if vbo is bound
                                    if (unbind_vbo)
                                        vbo.release();
                                    // no vbo: disable vertex attributes
                                    // has vbo, no vao: unbind vbo & set vertex attributes
                                    // has vbo, has vao: skip
                                    if (!unsetv_skip && g)
                                    {
                                        for (int an = 0; an < g->attributes().size(); ++an) {
                                            QGLF(glDisableVertexAttribArray(an));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            material->unbind(); // => GLInteropResource::unmap
        }
        //{QImage img=fbo->toImage();static int idx=0;++idx;qDebug()<<"VideoRenderer::handlePaintEvent img idx: "<<idx<<" isNull: "<<img.isNull();/*if(idx==3 || idx==15)*/ img.save(QString("images%1.jpg").arg(idx));}// storm
        //{QImage img=fbo->toImage();static int idx=0;++idx;printf("VideoRenderer::handlePaintEvent img idx: %d  isNull: %d\n",idx,img.isNull());/*if(idx==3 || idx==15)*/ img.save(QString("images/%1.jpg").arg(idx));}// storm
        //gl().BindFramebuffer(GL_FRAMEBUFFER, (GLuint)currentFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)currentFbo);
        VideoFormat fmt(VideoFormat::Format_RGB32);
        VideoFrame f(fbo->width(), fbo->height(), fmt); //
        f.setBytesPerLine(fbo->width()*fmt.bytesPerPixel(), 0);
        // set interop;
        class GLTextureInterop : public VideoSurfaceInterop
        {
            GLuint tex;
            public:
            GLTextureInterop(GLuint id) : tex(id) {}
            void* map(SurfaceType, const VideoFormat &, void *handle, int plane) {
                Q_UNUSED(plane);
                GLuint* t = reinterpret_cast<GLuint*>(handle);
                *t = tex;
                return t;
            }
        };
        GLTextureInterop *interop = new GLTextureInterop(fbo->texture());
        f.setMetaData(QStringLiteral("surface_interop"), QVariant::fromValue(VideoSurfaceInteropPtr((interop))));
        *frame = f;

        //drawFrame();
        {
            QRect roi = QRect(QPoint(), video_frame.size());//realROI();
            //d.glv.setCurrentFrame(d.video_frame);
            {
                material->setCurrentFrame(*frame);
            }
            //d.glv.render(QRectF(), roi, d.matrix);
            {
                QRectF target=QRectF();
                ///QRectF roi=QRectF();
                QMatrix4x4 transform=mat;
                const qint64 mt = material->type();
                if(material->bind())
                {
                    if (!shader)
                        shader = manager->prepareMaterial(material, mt); //TODO: print shader type name if changed. prepareMaterial(,sample_code, pp_code)
                    DYGL(glViewport(0, 0, fbo->width(), fbo->height()));
                    shader->update(material);
                    shader->program()->setUniformValue(shader->matrixLocation(), transform*matrix);
                    //d.updateGeometry(shader, target, roi);
                    if(1){
                        QRectF t=target;
                        QRectF r=roi;

                        if (!gr) { // TODO: only update VAO, not the whole GeometryRenderer
                            GeometryRenderer *r = new GeometryRenderer(); // local var is captured by lambda 
                            gr = r;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && defined(Q_COMPILER_LAMBDA)
                            QObject::connect(QOpenGLContext::currentContext(), &QOpenGLContext::aboutToBeDestroyed, [r]{
                                    qDebug("destroy GeometryRenderer %p", r);
                                    delete r;
                                    });
#endif
                        }
                        const bool roi_changed = valiad_tex_width != material->validTextureWidth() || roi != r || video_size != material->frameSize();
                        const int tc = shader->textureLocationCount();
                        QRectF target_rect = norm_viewport ? QRectF(-1, 1, 2, -2) : rect;
#if 1
                        if (tex_target != shader->textureTarget()) {
                            tex_target = shader->textureTarget();
                            update_geo = true;
                        }
                        if (target.isValid()) {
                            if (roi_changed || target != t) {
                                target = t;
                                update_geo = true;
                                //target_rect = target (if valid). // relate to gvf bug?
                            }
                        } else {
                            if (roi_changed) {
                                update_geo = true;
                            }
                        }
#endif
                        if (update_geo)
                        {
                            if( geometry ) delete geometry;
                            geometry = NULL;
                            if (mesh_type == OpenGLVideo::SphereMesh)
                                geometry = new Sphere();
                            else
                                geometry = new TexturedGeometry();
                            //qDebug("updating geometry...");
                            // setTextureCount may change the vertex data. Call it before setRect()
                            qDebug() << "target rect: " << target_rect ;
                            geometry->setTextureCount(shader->textureTarget() == GL_TEXTURE_RECTANGLE ? tc : 1);
                            geometry->setGeometryRect(target_rect);
                            geometry->setTextureRect(material->mapToTexture(0, roi));
                            if (shader->textureTarget() == GL_TEXTURE_RECTANGLE) {
                                for (int i = 1; i < tc; ++i) {
                                    // tc can > planes, but that will compute chroma plane
                                    geometry->setTextureRect(material->mapToTexture(i, roi), i);
                                }
                            }
                            geometry->create();
                            update_geo = false;
                            {
                                static int vbo_size=0; // QOpenGLBuffer.size() may get error 0x501
                                static QOpenGLBuffer vbo; //VertexBuffer
#if QT_VAO
                                static QOpenGLVertexArrayObject vao;
#endif //QT_VAO
                                static QOpenGLBuffer ibo;

                                // geometry characteristic
                                static int stride;
                                static QVector<Attribute> attrib;
                                Geometry *g=geometry; //static 会改变g的值?可能static变量太多了

                                //gr->updateGeometry(geometry); //create vbo, vao
                                // src/opengl/GeometryRenderer.cpp  83
                                // void GeometryRenderer::updateGeometry(Geometry *geo)
                                //    g = geo;
                                //    vbo.create(); // QOpenGLBuffer vbo
                                //    vbo.bind();
                                //    vao.create(); // QOpenGLVertexArrayObject vao
                                //    vao.bind();
                                //    QGLF(glVertexAttribPointer(an, a.tupleSize(), a.type(), a.normalize(), g->stride(), reinterpret_cast<const void *>(qptrdiff(a.offset())))); //TODO: in setActiveShader
                                //    QGLF(glEnableVertexAttribArray(an));
                                if(1){
                                    static int support_map = 1;
                                    if (!vbo.isCreated()) {
                                        qDebug("creating VBO...");
                                        if (!vbo.create())
                                            qWarning("VBO create error");
                                    }
                                    if (vbo.isCreated()) {
                                        vbo.bind();
                                        const int bs = g->vertexCount()*g->stride();
                                        if (bs == vbo_size) { // vbo.size() error 0x501 on rpi, and query gl value can be slow
                                            void* p = NULL;
                                            if (support_map )
                                                p = vbo.map(QOpenGLBuffer::WriteOnly);
                                            if (p) {
                                                memcpy(p, g->constVertexData(), bs);
                                                vbo.unmap();
                                            } else {
                                                vbo.write(0, g->constVertexData(), bs);
                                                vbo_size = bs;
                                            }
                                        } else {
                                            vbo.allocate(g->vertexData(), bs);
                                        }
                                        vbo.release();
                                    }
#if QT_VAO
                                    if (stride == g->stride() && attrib == g->attributes())
                                    {
                                    }
                                    else
                                    {
                                        stride = g->stride();
                                        attrib = g->attributes();

                                        if (!vao.isCreated()) {
                                            qDebug("creating VAO...");
                                            if (!vao.create())
                                                qDebug("VAO create error");
                                        }
                                        qDebug("vao updated");
                                        if (vao.isCreated()) // can not use vao binder because it will create a vao if necessary
                                            vao.bind();
                                        // can set data before vao bind
                                        if (vao.isCreated())
                                            qDebug("geometry attributes changed, rebind vao...");
                                    }
                                    // call once is enough if no feature and no geometry attribute is changed
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#define QGLF(f) QOpenGLContext::currentContext()->functions()->f
#else
#define QGLF(f) QGLFunctions(NULL).f
#endif
                                    if (vbo.isCreated()) {
                                        vbo.bind();
                                        for (int an = 0; an < g->attributes().size(); ++an) {
                                            // FIXME: assume bind order is 0,1,2...
                                            const Attribute& a = g->attributes().at(an);
                                            QGLF(glVertexAttribPointer(an, a.tupleSize(), a.type(), a.normalize(), g->stride(), reinterpret_cast<const void *>(qptrdiff(a.offset())))); //TODO: in setActiveShader
                                            QGLF(glEnableVertexAttribArray(an));
                                        }
                                        vbo.release(); // unbind after vao unbind? http://www.zwqxin.com/archives/opengl/vao-and-vbo-stuff.html
                                    } // TODO: bind pointers if vbo is disabled
                                    // bind ibo to vao thus no bind is required later
                                    if (ibo.isCreated())// if not bind here, glDrawElements(...,NULL) crashes and must use ibo data ptr, why?
                                        ibo.bind();
                                    vao.release();
                                    if (ibo.isCreated())
                                        ibo.release();
#endif
                                    qDebug("geometry updated");
                                }
                                // gr->render();
                                // ./src/opengl/GeometryRenderer.cpp  264
                                // =>void GeometryRenderer::render()
                                //    bindBuffers();
                                //    =>void GeometryRenderer::bindBuffers()
                                //        vao.bind(); // vbo, ibo is ok now
                                //    DYGL(glDrawArrays(g->primitive(), 0, g->vertexCount()));
                                if(1){
                                    {
                                        bool bind_vbo = vbo.isCreated();
                                        bool bind_ibo = ibo.isCreated();
                                        bool setv_skip = false;
#if QT_VAO
                                        if (vao.isCreated()) {
                                            vao.bind(); // vbo, ibo is ok now
                                            setv_skip = bind_vbo;
                                            bind_vbo = false;
                                            bind_ibo = false;
                                        }
#endif
                                        //qDebug("bind ibo: %d vbo: %d; set v: %d", bind_ibo, bind_vbo, !setv_skip);
                                        if (bind_ibo)
                                            ibo.bind();
                                        // no vbo: set vertex attributes
                                        // has vbo, no vao: bind vbo & set vertex attributes
                                        // has vbo, has vao: skip
                                        if (!setv_skip && g)
                                        {
                                            const char* vdata = static_cast<const char*>(g->vertexData());
                                            if (bind_vbo) {
                                                vbo.bind();
                                                vdata = NULL;
                                            }
                                            for (int an = 0; an < g->attributes().size(); ++an) {
                                                const Attribute& a = g->attributes().at(an);
                                                QGLF(glVertexAttribPointer(an, a.tupleSize(), a.type(), a.normalize(), g->stride(), vdata + a.offset()));
                                                QGLF(glEnableVertexAttribArray(an)); //TODO: in setActiveShader
                                            }
                                        }
                                    }

                                    if (g->indexCount() > 0) { qDebug("GeometryRenderer::render primitive: %d indexCount: %d indexType: %d indexData: %p", g->primitive(), g->indexCount(), g->indexType(), g->indexData());
                                        DYGL(glDrawElements(g->primitive(), g->indexCount(), g->indexType(), ibo.isCreated() ? NULL : g->indexData())); // null: data in vao or ibo. not null: data in memory
                                    } else {
                                        DYGL(glDrawArrays(g->primitive(), 0, g->vertexCount()));
                                    }
                                    {
                                        bool unbind_vbo = vbo.isCreated();
                                        bool unbind_ibo = ibo.isCreated();
                                        bool unsetv_skip = false;
#if QT_VAO
                                        if (vao.isCreated()) {
                                            vao.release();
                                            unsetv_skip = unbind_vbo;
                                            unbind_vbo = false;
                                            unbind_ibo = false;
                                        }
#endif //QT_VAO
                                        //qDebug("unbind ibo: %d vbo: %d; unset v: %d", unbind_ibo, unbind_vbo, !unsetv_skip);
                                        if (unbind_ibo)
                                            ibo.release();
                                        // release vbo. qpainter is affected if vbo is bound
                                        if (unbind_vbo)
                                            vbo.release();
                                        // no vbo: disable vertex attributes
                                        // has vbo, no vao: unbind vbo & set vertex attributes
                                        // has vbo, has vao: skip
                                        if (!unsetv_skip && g)
                                        {
                                            for (int an = 0; an < g->attributes().size(); ++an) {
                                                QGLF(glDisableVertexAttribArray(an));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return;
    }
    GLuint displayTexture=0;
    if(fbo)
    {
        displayTexture=fbo->texture();
    }
    printf("OpenGLWidget::paintGL video_frame: %d displayTexture: %d\n", video_frame.isValid(), displayTexture);fflush(0);
    ////QMutexLocker locker(&m_mutexPaint); 
    //doneCurrent();
    makeCurrent();
    // clear to solid black

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(fbo)
    {
        //DYGL(glViewport(0, 0, fbo->width(), fbo->height()));
        glViewport(0, 0, glvieww, glviewh);
    }
    else
    {
        glViewport(0, 0, glvieww, glviewh);
    }

    glEnable(GL_TEXTURE_2D);

    // set screen coords to widget size
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);

    // draw texture from render thread

    glBindTexture(GL_TEXTURE_2D, displayTexture);

    glBegin(GL_QUADS);

    glVertex2f(0, 0);
    glTexCoord2f(0, 0);
    glVertex2f(0, 1);
    glTexCoord2f(1, 0);
    glVertex2f(1, 1);
    glTexCoord2f(1, 1);
    glVertex2f(1, 0);
    glTexCoord2f(0, 1);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    qInfo()<<"OpenGLWidget::paintGL end";
    return;
}
