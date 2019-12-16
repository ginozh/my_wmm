/******************************************************************************
    QtAV:  Multimedia framework based on Qt and FFmpeg
    Copyright (C) 2012-2017 Wang Bin <wbsecg1@gmail.com>
*   This file is part of QtAV
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/
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
#include "opengl/OpenGLHelper.h"
#include "QtAV/VideoRenderer.h"
#include "QtAV/private/VideoRenderer_p.h"
#include "QtAV/Filter.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include "QtAV/Statistics.h"
#include "QtAV/private/factory.h"
#include "QtAV/private/mkid.h"
#include "utils/Logger.h"
namespace QtAV {
FACTORY_DEFINE(VideoRenderer)
VideoRendererId VideoRendererId_OpenGLWindow = mkid::id32base36_6<'Q', 'O', 'G', 'L', 'W', 'w'>::value;
VideoRenderer::VideoRenderer()
    :AVOutput(*new VideoRendererPrivate)
{
    // can not do 'if (widget()) connect to update()' because widget() is virtual
}
VideoRenderer::VideoRenderer(VideoRendererPrivate &d)
    :AVOutput(d)
{
}
VideoRenderer::~VideoRenderer()
{
}
bool VideoRenderer::receive(const VideoFrame &frame)
{
    DPTR_D(VideoRenderer);
    const qreal dar_old = d.source_aspect_ratio;
    d.source_aspect_ratio = frame.displayAspectRatio();
    if (dar_old != d.source_aspect_ratio)
        sourceAspectRatioChanged(d.source_aspect_ratio);
    setInSize(frame.width(), frame.height());
    QMutexLocker locker(&d.img_mutex);
    Q_UNUSED(locker); //TODO: double buffer for display/dec frame to avoid mutex
    return receiveFrame(frame);
}
bool VideoRenderer::setPreferredPixelFormat(VideoFormat::PixelFormat pixfmt)
{
    DPTR_D(VideoRenderer);
    if (d.preferred_format == pixfmt)
        return false;
    if (!isSupported(pixfmt)) {
        qWarning("pixel format '%s' is not supported", VideoFormat(pixfmt).name().toUtf8().constData());
        return false;
    }
    VideoFormat::PixelFormat old = d.preferred_format;
    d.preferred_format = pixfmt;
    if (!onSetPreferredPixelFormat(pixfmt)) {
        qWarning("onSetPreferredPixelFormat failed");
        d.preferred_format = old;
        return false;
    }
    return true;
}
bool VideoRenderer::onSetPreferredPixelFormat(VideoFormat::PixelFormat pixfmt)
{
    Q_UNUSED(pixfmt);
    return true;
}

VideoFormat::PixelFormat VideoRenderer::preferredPixelFormat() const
{
    return d_func().preferred_format;
}

void VideoRenderer::forcePreferredPixelFormat(bool force)
{
    DPTR_D(VideoRenderer);
    if (d.force_preferred == force)
        return;
    bool old = d.force_preferred;
    d.force_preferred = force;
    if (!onForcePreferredPixelFormat(force)) {
        qWarning("onForcePreferredPixelFormat failed");
        d.force_preferred = old;
    }
}

bool VideoRenderer::onForcePreferredPixelFormat(bool force)
{
    Q_UNUSED(force);
    return true;
}

bool VideoRenderer::isPreferredPixelFormatForced() const
{
    return d_func().force_preferred;
}

qreal VideoRenderer::sourceAspectRatio() const
{
    return d_func().source_aspect_ratio;
}

void VideoRenderer::setOutAspectRatioMode(OutAspectRatioMode mode)
{
    DPTR_D(VideoRenderer);
    if (mode == d.out_aspect_ratio_mode)
        return;
    d.aspect_ratio_changed = true;
    d.out_aspect_ratio_mode = mode;
    if (mode == RendererAspectRatio) {
        QRect out_rect0(d.out_rect);
        //compute out_rect
        d.out_rect = QRect(0, 0, d.renderer_width, d.renderer_height); //remove? already in computeOutParameters()
        setOutAspectRatio(qreal(d.renderer_width)/qreal(d.renderer_height));
        if (out_rect0 != d.out_rect) {
            Q_EMIT videoRectChanged();
            Q_EMIT contentRectChanged();
        }
        //is that thread safe?
    } else if (mode == VideoAspectRatio) {
        setOutAspectRatio(d.source_aspect_ratio);
    }
    onSetOutAspectRatioMode(mode);
    Q_EMIT outAspectRatioModeChanged();
}

void VideoRenderer::onSetOutAspectRatioMode(OutAspectRatioMode mode)
{
    Q_UNUSED(mode);
}

VideoRenderer::OutAspectRatioMode VideoRenderer::outAspectRatioMode() const
{
    return d_func().out_aspect_ratio_mode;
}

void VideoRenderer::setOutAspectRatio(qreal ratio)
{
    DPTR_D(VideoRenderer);
    bool ratio_changed = d.out_aspect_ratio != ratio;
    d.out_aspect_ratio = ratio;
    //indicate that this function is called by user. otherwise, called in VideoRenderer
    if (!d.aspect_ratio_changed) {
        if (d.out_aspect_ratio_mode != CustomAspectRation) {
            d.out_aspect_ratio_mode = CustomAspectRation;
            Q_EMIT outAspectRatioModeChanged();
        }
    }
    d.aspect_ratio_changed = false; //TODO: when is false?
    if (d.out_aspect_ratio_mode != RendererAspectRatio) {
        d.update_background = true; //can not fill the whole renderer with video
    }
    //compute the out out_rect
    if (d.computeOutParameters(ratio)) {
        Q_EMIT videoRectChanged();
        Q_EMIT contentRectChanged();
    }
    if (ratio_changed) {
        onSetOutAspectRatio(ratio);
        Q_EMIT outAspectRatioChanged();
    }
    updateUi();
}

void VideoRenderer::onSetOutAspectRatio(qreal ratio)
{
    Q_UNUSED(ratio);
}

qreal VideoRenderer::outAspectRatio() const
{
    return d_func().out_aspect_ratio;
}

void VideoRenderer::setQuality(Quality q)
{
    DPTR_D(VideoRenderer);
    if (d.quality == q)
        return;
    Quality old = quality();
    d.quality = q;
    if (!onSetQuality(q)) {
        d.quality = old;
    } else {
        updateUi();
    }
}

bool VideoRenderer::onSetQuality(Quality q)
{
    Q_UNUSED(q);
    return true;
}

VideoRenderer::Quality VideoRenderer::quality() const
{
    return d_func().quality;
}

void VideoRenderer::setInSize(const QSize& s)
{
    setInSize(s.width(), s.height());
}

void VideoRenderer::setInSize(int width, int height)
{
    DPTR_D(VideoRenderer);
    if (d.src_width != width || d.src_height != height) {
        d.aspect_ratio_changed = true; //?? for VideoAspectRatio mode
        d.src_width = width;
        d.src_height = height;
        Q_EMIT videoFrameSizeChanged();
    }
    if (!d.aspect_ratio_changed)// && (d.src_width == width && d.src_height == height))
        return;
    //d.source_aspect_ratio = qreal(d.src_width)/qreal(d.src_height);
    qDebug("%s => calculating aspect ratio from converted input data(%f)", __FUNCTION__, d.source_aspect_ratio);
    //see setOutAspectRatioMode
    if (d.out_aspect_ratio_mode == VideoAspectRatio) {
        //source_aspect_ratio equals to original video aspect ratio here, also equals to out ratio
        setOutAspectRatio(d.source_aspect_ratio);
    }
    d.aspect_ratio_changed = false; //TODO: why graphicsitemrenderer need this? otherwise aspect_ratio_changed is always true?
}

void VideoRenderer::resizeRenderer(const QSize &size)
{
    resizeRenderer(size.width(), size.height());
}

void VideoRenderer::resizeRenderer(int width, int height)
{
    DPTR_D(VideoRenderer);
    if (width == 0 || height == 0 || (d.renderer_width == width && d.renderer_height == height))
        return;
    d.renderer_width = width;
    d.renderer_height = height;
    if (d.out_aspect_ratio_mode == RendererAspectRatio)
        Q_EMIT outAspectRatioChanged();
    if (d.computeOutParameters(d.out_aspect_ratio)) {
        Q_EMIT videoRectChanged();
        Q_EMIT contentRectChanged();
    }
    onResizeRenderer(width, height); //TODO: resize widget
}

void VideoRenderer::onResizeRenderer(int width, int height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
}

QSize VideoRenderer::rendererSize() const
{
    DPTR_D(const VideoRenderer);
    return QSize(d.renderer_width, d.renderer_height);
}

int VideoRenderer::rendererWidth() const
{
    return d_func().renderer_width;
}

int VideoRenderer::rendererHeight() const
{
    return d_func().renderer_height;
}

void VideoRenderer::setOrientation(int value)
{
    DPTR_D(VideoRenderer);
    // currently only supports a multiple of 90
    value = (value + 360) % 360;
    if (value % 90)
        return;
    if (d.orientation == value)
        return;
    int old = orientation();
    d.orientation = value;
    if (!onSetOrientation(value)) {
        d.orientation = old;
    } else {
        orientationChanged();
        if (d.computeOutParameters(d.out_aspect_ratio)) {
            Q_EMIT videoRectChanged();
            Q_EMIT contentRectChanged();
        }
        onSetOutAspectRatio(outAspectRatio());
        updateUi();
    }
}

int VideoRenderer::orientation() const
{
    DPTR_D(const VideoRenderer);
    return d.orientation;
}

// only qpainter and opengl based renderers support orientation.
bool VideoRenderer::onSetOrientation(int value)
{
    Q_UNUSED(value);
    return false;
}

QSize VideoRenderer::videoFrameSize() const
{
    DPTR_D(const VideoRenderer);
    return QSize(d.src_width, d.src_height);
}

QRect VideoRenderer::videoRect() const
{
    return d_func().out_rect;
}

QRectF VideoRenderer::regionOfInterest() const
{
    return d_func().roi;
}

void VideoRenderer::setRegionOfInterest(qreal x, qreal y, qreal width, qreal height)
{
    setRegionOfInterest(QRectF(x, y, width, height));
}

void VideoRenderer::setRegionOfInterest(const QRectF &roi)
{
    DPTR_D(VideoRenderer);
    if (d.roi == roi)
        return;
    QRectF old = regionOfInterest();
    d.roi = roi;
    if (!onSetRegionOfInterest(roi)) {
        d.roi = old;
    } else {
        Q_EMIT regionOfInterestChanged();
        updateUi();
    }
    // TODO: how to fill video? what's out_rect now?
}

bool VideoRenderer::onSetRegionOfInterest(const QRectF &roi)
{
    Q_UNUSED(roi);
    return true;
}

QRect VideoRenderer::realROI() const
{
    DPTR_D(const VideoRenderer);
    if (!d.roi.isValid()) {
        return QRect(QPoint(), d.video_frame.size());
    }
    QRect r = d.roi.toRect();
    // nomalized x, y < 1
    bool normalized = false;
    if (qAbs(d.roi.x()) < 1) {
        normalized = true;
        r.setX(d.roi.x()*qreal(d.src_width)); //TODO: why not video_frame.size()? roi not correct
    }
    if (qAbs(d.roi.y()) < 1) {
        normalized = true;
        r.setY(d.roi.y()*qreal(d.src_height));
    }
    // whole size use width or height = 0, i.e. null size
    // nomalized width, height <= 1. If 1 is normalized value iff |x|<1 || |y| < 1
    if (qAbs(d.roi.width()) < 1)
        r.setWidth(d.roi.width()*qreal(d.src_width));
    if (qAbs(d.roi.height()) < 1)
        r.setHeight(d.roi.height()*qreal(d.src_height));
    if (d.roi.width() == 1.0 && normalized) {
        r.setWidth(d.src_width);
    }
    if (d.roi.height() == 1.0 && normalized) {
        r.setHeight(d.src_height);
    }
    //TODO: insect with source rect?
    return r;
}

QRectF VideoRenderer::normalizedROI() const
{
    DPTR_D(const VideoRenderer);
    if (!d.roi.isValid()) {
        return QRectF(0, 0, 1, 1);
    }
    QRectF r = d.roi;
    bool normalized = false;
    if (qAbs(r.x()) >= 1)
        r.setX(r.x()/qreal(d.src_width));
    else
        normalized = true;
    if (qAbs(r.y()) >= 1)
        r.setY(r.y()/qreal(d.src_height));
    else
        normalized = true;
    if (r.width() > 1 || (!normalized && r.width() == 1))
        r.setWidth(r.width()/qreal(d.src_width));
    if (r.height() > 1 || (!normalized && r.width() == 1)) {
        r.setHeight(r.height()/qreal(d.src_height));
    }
    return r;
}

QPointF VideoRenderer::mapToFrame(const QPointF &p) const
{
    return onMapToFrame(p);
}

// TODO: orientation
QPointF VideoRenderer::onMapToFrame(const QPointF &p) const
{
    QRectF roi = realROI();
    // zoom=roi.w/roi.h>vo.w/vo.h?roi.w/vo.w:roi.h/vo.h
    qreal zoom = qMax(roi.width()/rendererWidth(), roi.height()/rendererHeight());
    QPointF delta = p - QPointF(rendererWidth()/2, rendererHeight()/2);
    return roi.center() + delta * zoom;
}

QPointF VideoRenderer::mapFromFrame(const QPointF &p) const
{
    return onMapFromFrame(p);
}

QPointF VideoRenderer::onMapFromFrame(const QPointF &p) const
{
    QRectF roi = realROI();
    // zoom=roi.w/roi.h>vo.w/vo.h?roi.w/vo.w:roi.h/vo.h
    qreal zoom = qMax(roi.width()/rendererWidth(), roi.height()/rendererHeight());
    // (p-roi.c)/zoom + c
    QPointF delta = p - roi.center();
    return QPointF(rendererWidth()/2, rendererHeight()/2) + delta / zoom;
}

QRegion VideoRenderer::backgroundRegion() const
{
    return QRegion(0, 0, rendererWidth(), rendererHeight()) - QRegion(d_func().out_rect);
}

void VideoRenderer::drawBackground()
{
}
#if 0
void VideoRenderer::handlePaintEvent()
{float time_use=0; struct timeval start; struct timeval end; gettimeofday(&start,NULL); //storm
    DPTR_D(VideoRenderer);
    d.setupQuality();
    //begin paint. how about QPainter::beginNativePainting()?
    {
        //lock is required only when drawing the frame
        QMutexLocker locker(&d.img_mutex);
        Q_UNUSED(locker);
        // do not apply filters if d.video_frame is already filtered. e.g. rendering an image and resize window to repaint
        if (!d.video_frame.metaData(QStringLiteral("gpu_filtered")).toBool() && !d.filters.isEmpty() && d.statistics) {
            // vo filter will not modify video frame, no lock required
            foreach(Filter* filter, d.filters) {
                VideoFilter *vf = static_cast<VideoFilter*>(filter);
                if (!vf) {
                    qWarning("a null filter!");
                    //d.filters.removeOne(filter);
                    continue;
                }
                if (!vf->isEnabled())
                    continue;
                // qpainter on video frame always runs on video thread. qpainter on renderer's paint device can work on rendering thread
                // Here apply filters on frame on video thread, for example, GPU filters

                //vf->prepareContext(d.filter_context, d.statistics, 0);
                //if (!vf->context() || vf->context()->type() != VideoFilterContext::OpenGL)
                if (!vf->isSupported(VideoFilterContext::OpenGL))
                    continue;
                vf->apply(d.statistics, &d.video_frame); //painter and paint device are ready, pass video frame is ok.
                d.video_frame.setMetaData(QStringLiteral("gpu_filtered"), true);
            }
        }
        /* begin paint. how about QPainter::beginNativePainting()?
         * fill background color when necessary, e.g. renderer is resized, image is null
         * if we access d.data which will be modified in AVThread, the following must be
         * protected by mutex. otherwise, e.g. QPainterRenderer, it's not required if drawing
         * on the shared data is safe
         */
        drawBackground();
        /*
         * NOTE: if data is not copyed in receiveFrame(), you should always call drawFrame()
         */
        if (d.video_frame.isValid()) {
            drawFrame();
            //qDebug("render elapsed: %lld", et.elapsed());
            if (d.statistics) {
                d.statistics->video_only.frameDisplayed(d.video_frame.timestamp());
                d.statistics->video.current_time = QTime(0, 0, 0).addMSecs(int(d.video_frame.timestamp() * 1000.0));
            }
        }
    }
    hanlePendingTasks();
    //TODO: move to AVOutput::applyFilters() //protected?
    if (!d.filters.isEmpty() && d.filter_context && d.statistics) {
        // vo filter will not modify video frame, no lock required
        foreach(Filter* filter, d.filters) {
            VideoFilter *vf = static_cast<VideoFilter*>(filter);
            if (!vf) {
                qWarning("a null filter!");
                //d.filters.removeOne(filter);
                continue;
            }
            if (!vf->isEnabled())
                continue;
            // qpainter rendering on renderer's paint device. only supported by none-null paint engine
            if (!vf->context() || vf->context()->type()  == VideoFilterContext::OpenGL)
                continue;
            if (vf->prepareContext(d.filter_context, d.statistics, 0)) {
                if (!vf->isSupported(d.filter_context->type()))
                    continue;
                vf->apply(d.statistics, &d.video_frame); //painter and paint device are ready, pass video frame is ok.
            }
        }
    }//gettimeofday(&end,NULL);time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);qDebug()<<"VideoRenderer::handlePaintEvent zero copy waste:"<<time_use;//storm
    //end paint. how about QPainter::endNativePainting()?
}
#endif
qreal VideoRenderer::brightness() const
{
    return d_func().brightness;
}

bool VideoRenderer::setBrightness(qreal brightness)
{
    DPTR_D(VideoRenderer);
    if (d.brightness == brightness)
        return true;
    if (!onSetBrightness(brightness))
        return false;
    d.brightness = brightness;
    Q_EMIT brightnessChanged(brightness);
    updateUi();
    return true;
}

qreal VideoRenderer::contrast() const
{
    return d_func().contrast;
}

bool VideoRenderer::setContrast(qreal contrast)
{
    DPTR_D(VideoRenderer);
    if (d.contrast == contrast)
        return true;
    if (!onSetContrast(contrast))
        return false;
    d.contrast = contrast;
    Q_EMIT contrastChanged(contrast);
    updateUi();
    return true;
}

qreal VideoRenderer::hue() const
{
    return d_func().hue;
}

bool VideoRenderer::setHue(qreal hue)
{
    DPTR_D(VideoRenderer);
    if (d.hue == hue)
        return true;
    if (!onSetHue(hue))
        return false;
    d.hue = hue;
    Q_EMIT hueChanged(hue);
    updateUi();
    return true;
}

qreal VideoRenderer::saturation() const
{
    return d_func().saturation;
}

bool VideoRenderer::setSaturation(qreal saturation)
{
    DPTR_D(VideoRenderer);
    if (d.saturation == saturation)
        return true;
    if (!onSetSaturation(saturation))
        return false;
    d.saturation = saturation;
    Q_EMIT saturationChanged(saturation);
    updateUi();
    return true;
}

bool VideoRenderer::onSetBrightness(qreal b)
{
    Q_UNUSED(b);
    return false;
}

bool VideoRenderer::onSetContrast(qreal c)
{
    Q_UNUSED(c);
    return false;
}

bool VideoRenderer::onSetHue(qreal h)
{
    Q_UNUSED(h);
    return false;
}

bool VideoRenderer::onSetSaturation(qreal s)
{
    Q_UNUSED(s);
    return false;
}

QColor VideoRenderer::backgroundColor() const
{
    return d_func().bg_color;
}

void VideoRenderer::onSetBackgroundColor(const QColor &color)
{
    Q_UNUSED(color);
}

void VideoRenderer::setBackgroundColor(const QColor &c)
{
    DPTR_D(VideoRenderer);
    if (d.bg_color == c)
        return;
    onSetBackgroundColor(c);
    d.bg_color = c;
    Q_EMIT backgroundColorChanged();
    updateUi();
}

void VideoRenderer::updateUi()
{
    QObject *obj = (QObject*)widget();
    if (obj) {
        // UpdateRequest only sync backing store but do not shedule repainting. UpdateLater does
        // Copy from qwidget_p.h. QWidget::event() will convert UpdateLater to QUpdateLaterEvent and get it's region()
        class QUpdateLaterEvent : public QEvent
        {
        public:
            explicit QUpdateLaterEvent(const QRegion& paintRegion)
                : QEvent(UpdateLater), m_region(paintRegion)
            {}
            ~QUpdateLaterEvent() {}
            inline const QRegion &region() const { return m_region; }
        protected:
            QRegion m_region;
        };
        QCoreApplication::instance()->postEvent(obj, new QUpdateLaterEvent(QRegion(0, 0, rendererWidth(), rendererHeight())));
    } else {
        obj = (QObject*)qwindow();
        if (obj)
            QCoreApplication::instance()->postEvent(obj, new QEvent(QEvent::UpdateRequest));
    }
}
#if 1
void VideoRenderer::handlePaintEvent()
{float time_use=0; struct timeval start; struct timeval end; gettimeofday(&start,NULL); //storm
    DPTR_D(VideoRenderer);
    d.setupQuality();
    //begin paint. how about QPainter::beginNativePainting()?
    {
        //lock is required only when drawing the frame
        QMutexLocker locker(&d.img_mutex);
        Q_UNUSED(locker);
#if 1
        if(d.video_frame){
            {
                //qDebug()<<"VideoRenderer::handlePaintEvent pts:"<<d.video_frame.timestamp();//storm
            static QOpenGLFramebufferObject *fbo=NULL;
            VideoFrame *frame= &d.video_frame;
            static VideoMaterial *material=NULL;
            //static ShaderManager *manager=NULL;
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
#if 0
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
#endif
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
                ///const qint64 mt = material->type();
                if(material->bind())
                {
                    if (!shader)
                    {
                        //shader = manager->prepareMaterial(material, mt); //TODO: print shader type name if changed. prepareMaterial(,sample_code, pp_code)
                        shader = material->createShader();
                        shader->initialize();
                    }
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
#if 0
            GLuint displayTexture=0;
            if(fbo)
            {
                displayTexture=fbo->texture();
            }
            printf("OpenGLWidget::paintGL video_frame: %d displayTexture: %d\n", d.video_frame.isValid(), displayTexture);fflush(0);
            ////QMutexLocker locker(&m_mutexPaint); 
            //doneCurrent();
            //makeCurrent();
            // clear to solid black

            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            if(fbo)
            {
                DYGL(glViewport(0, 0, fbo->width(), fbo->height()));
                //glViewport(0, 0, glvieww, glviewh);
            }
            else
            {
                //glViewport(0, 0, glvieww, glviewh);
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
#endif
#if 1
            {QImage img=fbo->toImage();static int idx=0;++idx;qDebug()<<"VideoRenderer::handlePaintEvent img idx: "<<idx<<" isNull: "<<img.isNull();if(idx==1 || idx==15) img.save(QString("%1.jpg").arg(idx));}// storm
            gl().BindFramebuffer(GL_FRAMEBUFFER, (GLuint)currentFbo);
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
                QRect roi = realROI();
                //d.glv.setCurrentFrame(d.video_frame);
                {
                    material->setCurrentFrame(*frame);
                }
                //d.glv.render(QRectF(), roi, d.matrix);
                {
                QRectF target=QRectF();
                ///QRectF roi=QRectF();
                QMatrix4x4 transform=mat;
                //const qint64 mt = material->type();
                if(material->bind())
                {
                    if (!shader)
                    {
                        //shader = manager->prepareMaterial(material, mt); //TODO: print shader type name if changed. prepareMaterial(,sample_code, pp_code)
                        shader = material->createShader();
                        shader->initialize();
                    }
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
#endif
            }
#if 0
            // src/filter/GLSLFilter.cpp 84
            if (!QOpenGLContext::currentContext()) {
                qWarning() << "No current gl context for glsl filter: " << this;
                return;
            }
            // OpenGLVideo.cpp
            static QOpenGLFramebufferObject *fbo=NULL;
            VideoFrame *frame= &d.video_frame;
            static VideoMaterial *material=NULL;
            static ShaderManager *manager=NULL;
            static QMatrix4x4 matrix;
            static QRectF rect;
            if(!fbo)
            {
                fbo = new QOpenGLFramebufferObject(frame->size(), GL_TEXTURE_2D); //TODO: prefer 16bit rgb
                QOpenGLContext *ctx = const_cast<QOpenGLContext*>(QOpenGLContext::currentContext()); //qt4 returns const
                //d.glv.setOpenGLContext(ctx);
                {
                    qreal b = 0, c = 0, h = 0, s = 0;
                    if (material) {
                        b = material->brightness();
                        c = material->contrast();
                        h = material->hue();
                        s = material->saturation();
                        delete material;
                        material = 0;
                    }
                    material = new VideoMaterial();
                    material->setBrightness(b);
                    material->setContrast(c);
                    material->setHue(h);
                    material->setSaturation(s);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                    manager = ctx->findChild<ShaderManager*>(QStringLiteral("__qtav_shader_manager"));
#endif
                    ////updateViewport();
                    {
                        QSizeF surfaceSize = ctx->surface()->size();
                        surfaceSize *= ctx->screen()->devicePixelRatio();
                        //setProjectionMatrixToRect(QRectF(QPointF(), surfaceSize));
                        QRectF r=QRectF(QPointF(), surfaceSize);
                        // => setViewport
                        {
                            rect = r;
                            matrix.setToIdentity();
                            DYGL(glViewport(rect.x(), rect.y(), rect.width(), rect.height()));
                        }

                    }
                    if (!manager)
                    {
                        // TODO: what if ctx is delete?
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                        manager = new ShaderManager(ctx);
                        ////QObject::connect(ctx, SIGNAL(aboutToBeDestroyed()), this, SLOT(resetGL()), Qt::DirectConnection); // direct to make sure there is a valid context. makeCurrent in window.aboutToBeDestroyed()?
#endif
                        manager->setObjectName(QStringLiteral("__qtav_shader_manager"));
                        /// get gl info here because context is current(qt ensure it)
                        //const QByteArray extensions(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));
                    }
                    bool hasGLSL = QOpenGLShaderProgram::hasOpenGLShaderPrograms();
                    qDebug("OpenGL version: %d.%d  hasGLSL: %d", ctx->format().majorVersion(), ctx->format().minorVersion(), hasGLSL);
                }
                //d.glv.setProjectionMatrixToRect(QRectF(0, 0, d.fbo->width(), d.fbo->height()));
                //=>OpenGLVideo::setViewport(const QRectF &r)
                {
                    QRectF r = QRectF(0, 0, fbo->width(), fbo->height());
                    {
                        rect = r;
                        matrix.setToIdentity();
                        DYGL(glViewport(rect.x(), rect.y(), rect.width(), rect.height()));
                    }
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
            {
                QRectF target=QRectF();
                QRectF roi=QRectF();
                QMatrix4x4 transform=mat;
                const qint64 mt = material->type();
                if(material->bind())
                {
                    static VideoShader *shader = NULL; 
                    if (!shader)
                        shader = manager->prepareMaterial(material, mt); //TODO: print shader type name if changed. prepareMaterial(,sample_code, pp_code)
                    DYGL(glViewport(0, 0, fbo->width(), fbo->height()));
                    shader->update(material);
                    shader->program()->setUniformValue(shader->matrixLocation(), transform*matrix);
                    //d.updateGeometry(shader, target, roi);
                    {
                        QRectF t=target;
                        QRectF r=roi;
                        static qreal valiad_tex_width=0;
                        static bool update_geo=false;
                        static int tex_target=0;
                        static QSize video_size;
                        static GeometryRenderer* gr=NULL;
                        static OpenGLVideo::MeshType mesh_type=OpenGLVideo::RectMesh;
                        static bool norm_viewport=true;
                        static TexturedGeometry *geometry=NULL;

                        const bool roi_changed = valiad_tex_width != material->validTextureWidth() || roi != r || video_size != material->frameSize();
                        const int tc = shader->textureLocationCount();
                        if (roi_changed) {
                            roi = r;
                            valiad_tex_width = material->validTextureWidth();
                            video_size = material->frameSize();
                        }
                        if (tex_target != shader->textureTarget()) {
                            tex_target = shader->textureTarget();
                            update_geo = true;
                        }
                        bool update_gr = false;
                        static QThreadStorage<bool> new_thread;
                        if (!new_thread.hasLocalData())
                            new_thread.setLocalData(true);

                        update_gr = new_thread.localData();
                        if (!gr || update_gr) { // TODO: only update VAO, not the whole GeometryRenderer
                            update_geo = true;
                            new_thread.setLocalData(false);
                            GeometryRenderer *r = new GeometryRenderer(); // local var is captured by lambda 
                            gr = r;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && defined(Q_COMPILER_LAMBDA)
                            QObject::connect(QOpenGLContext::currentContext(), &QOpenGLContext::aboutToBeDestroyed, [r]{
                                    qDebug("destroy GeometryRenderer %p", r);
                                    delete r;
                                    });
#endif
                        }
                        // (-1, -1, 2, 2) must flip y
                        QRectF target_rect = norm_viewport ? QRectF(-1, 1, 2, -2) : rect;
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
                            gr->updateGeometry(geometry);
                            gr->render();
                        }
                    }
                }
                material->unbind(); // => GLInteropResource::unmap
                {QImage img=fbo->toImage();static int idx=0;++idx;qDebug()<<"VideoRenderer::handlePaintEvent img idx: "<<idx<<" isNull: "<<img.isNull();if(idx==3 || idx==15) img.save(QString("images/%1.jpg").arg(idx));}// storm
            }
#endif
#if 0
            {
                GLuint tex;
                // d.material->bind() // bind first because texture parameters(target) mapped from native buffer is unknown before it
                // src/opengl/VideoShader.cpp 592
                GLenum target=3553;
                {
                    //const int nb_planes = d.textures.size(); //number of texture id
                    const int nb_planes = 1;
                    //d.ensureTextures();
                        DYGL(glGenTextures(1, &tex));
                        //initTexture(tex, internal_format[p], data_format[p], data_type[p], texture_size[p].width(), texture_size[p].height());
                        GLint internal_format=6408; GLenum format=32993; GLenum dataType=5121; int width=fbo->width(); int height=fbo->height();
                        {
                            DYGL(glBindTexture(target, tex));
                            //setupQuality();
                                DYGL(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                                DYGL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                            // This is necessary for non-power-of-two textures
                            DYGL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                            DYGL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                            DYGL(glTexImage2D(target, 0, internal_format, width, height, 0/*border, ES not support*/, format, dataType, NULL));
                            DYGL(glBindTexture(target, 0));
                        }
                    //d.uploadPlane(p, d.update_texure);
                    //=>void VideoMaterialPrivate::uploadPlane(int p, bool updateTexture)
                    //  if (frame.map(GLTextureSurface, &tex, p)) 
                    //  src/VideoFrame.cpp  424
                    //  void *VideoFrame::map(SurfaceType type, void *handle, const VideoFormat& fmt, int plane)
                    //      return d->surface_interop->map(type, fmt, handle, plane);
                    //      src/directx/SurfaceInteropD3D9.cpp  129
                    //      =>void* SurfaceInterop::map(SurfaceType type, const VideoFormat &fmt, void *handle, int plane)
                    //          if (m_resource->map(m_surface, *((GLuint*)handle), frame_width, frame_height, plane))
                    //          // src/directx/SurfaceInteropD3D9GL.cpp  61
                    //          =>bool GLInteropResource::map(IDirect3DSurface9 *surface, GLuint tex, int w, int h, int)
                    for (int i = 0; i < nb_planes; ++i) {
                        const int p = (i + 1) % nb_planes; //0 must active at last?
                        //d.uploadPlane(p, d.update_texure);
                        gl().ActiveTexture(GL_TEXTURE0 + p); //0 must active?
                        if (frame->map(GLTextureSurface, &tex, p)) {
                            DYGL(glBindTexture(target, tex)); // glActiveTexture was called, but maybe bind to 0 in map
                        }
                    }
                }
                //const qint64 mt = d.material->type();
                const qint64 mt = 12;
                //VideoShader *shader = d.user_shader;
                static VideoShader *shader = NULL; 
                //shader = d.manager->prepareMaterial(d.material, mt); //TODO: print shader type name if changed. prepareMaterial(,sample_code, pp_code)
                //src/opengl/ShaderManager.cpp  29
                if(!shader){
                    //shader = material->createShader();
                    {
                    shader = new VideoShader();
                    // initialize shader
                    //shader->setVideoFormat(currentFormat());
                    shader->setVideoFormat(frame->format());
                    //shader->setTextureTarget(textureTarget());
                    shader->setTextureTarget(target);
                    //shader->setMaterialType(type());
                    shader->setMaterialType(mt);
                    }
                    shader->initialize();
                    const qint32 type = mt;
                }
                //DYGL(glViewport(d.rect.x(), d.rect.y(), d.rect.width(), d.rect.height())); 
                DYGL(glViewport(0, 0, fbo->width(), fbo->height()));
                //shader->update(d.material);
                //shader->program()->setUniformValue(shader->matrixLocation(), transform*d.matrix);
                // uniform end. attribute begin
                //d.updateGeometry(shader, target, roi);
                //d.gr->render();
                // src/opengl/GeometryRenderer.cpp  264
                //DYGL(glDrawElements(g->primitive(), g->indexCount(), g->indexType(), ibo.isCreated() ? NULL : g->indexData())); // null: data in vao or ibo. not null: data in memory
                //DYGL(glDrawElements(QtAV::Geometry::TriangleStrip, 0, QtAV::TypeU16, 0);
                DYGL(glDrawElements(0x0005, 0, 0x1403, 0));

                //d.material->unbind(); // => GLInteropResource::unmap
                //d.frame.unmap(&d.textures[p]);
                frame->unmap(&tex);
                qDebug()<<"VideoRenderer::handlePaintEvent tex: "<<tex;
            }
#endif
            return;
        }
#endif
        // do not apply filters if d.video_frame is already filtered. e.g. rendering an image and resize window to repaint
        if (!d.video_frame.metaData(QStringLiteral("gpu_filtered")).toBool() && !d.filters.isEmpty() && d.statistics) {
            // vo filter will not modify video frame, no lock required
            foreach(Filter* filter, d.filters) {
                VideoFilter *vf = static_cast<VideoFilter*>(filter);
                if (!vf) {
                    qWarning("a null filter!");
                    //d.filters.removeOne(filter);
                    continue;
                }
                if (!vf->isEnabled())
                    continue;
                // qpainter on video frame always runs on video thread. qpainter on renderer's paint device can work on rendering thread
                // Here apply filters on frame on video thread, for example, GPU filters

                //vf->prepareContext(d.filter_context, d.statistics, 0);
                //if (!vf->context() || vf->context()->type() != VideoFilterContext::OpenGL)
                if (!vf->isSupported(VideoFilterContext::OpenGL))
                    continue;
                vf->apply(d.statistics, &d.video_frame); //painter and paint device are ready, pass video frame is ok.
                d.video_frame.setMetaData(QStringLiteral("gpu_filtered"), true);
            }
        }
        /* begin paint. how about QPainter::beginNativePainting()?
         * fill background color when necessary, e.g. renderer is resized, image is null
         * if we access d.data which will be modified in AVThread, the following must be
         * protected by mutex. otherwise, e.g. QPainterRenderer, it's not required if drawing
         * on the shared data is safe
         */
        drawBackground();
        /*
         * NOTE: if data is not copyed in receiveFrame(), you should always call drawFrame()
         */
        if (d.video_frame.isValid()) {
            drawFrame();
            //qDebug("render elapsed: %lld", et.elapsed());
            if (d.statistics) {
                d.statistics->video_only.frameDisplayed(d.video_frame.timestamp());
                d.statistics->video.current_time = QTime(0, 0, 0).addMSecs(int(d.video_frame.timestamp() * 1000.0));
            }
        }
    }
    hanlePendingTasks();
    //TODO: move to AVOutput::applyFilters() //protected?
    if (!d.filters.isEmpty() && d.filter_context && d.statistics) {
        // vo filter will not modify video frame, no lock required
        foreach(Filter* filter, d.filters) {
            VideoFilter *vf = static_cast<VideoFilter*>(filter);
            if (!vf) {
                qWarning("a null filter!");
                //d.filters.removeOne(filter);
                continue;
            }
            if (!vf->isEnabled())
                continue;
            // qpainter rendering on renderer's paint device. only supported by none-null paint engine
            if (!vf->context() || vf->context()->type()  == VideoFilterContext::OpenGL)
                continue;
            if (vf->prepareContext(d.filter_context, d.statistics, 0)) {
                if (!vf->isSupported(d.filter_context->type()))
                    continue;
                vf->apply(d.statistics, &d.video_frame); //painter and paint device are ready, pass video frame is ok.
            }
        }
    }gettimeofday(&end,NULL);time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);qDebug()<<"VideoRenderer::handlePaintEvent zero copy waste:"<<time_use;//storm
    //end paint. how about QPainter::endNativePainting()?
}
#endif
} //namespace QtAV
