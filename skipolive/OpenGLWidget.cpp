#include "OpenGLWidget.h"

#include "project/effect.h"
#include "renderthread.h"
#include "Clip.h"
#include <QPainter>
#include <QAudioOutput>
#include <QOpenGLShaderProgram>
#include <QtMath>
#include <QOpenGLFramebufferObject>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QOffscreenSurface>
#include <QFileDialog>
#include <QPolygon>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QApplication>
#include <QScreen>
#include <QMessageBox>

//void saveOpenglBuffer(QString prefix); //storm
OpenGLWidget::OpenGLWidget(QWidget *parent) :
	QOpenGLWidget(parent),
	//waveform(false),
	//waveform_zoom(1.0),
	//waveform_scroll(0),
	dragging(false),
	gizmos(nullptr),
	selected_gizmo(nullptr)
{
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	setFormat(format);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_context_menu()));

	renderer = new RenderThread();
	renderer->start(QThread::HighPriority);
    renderer->setGLWidget(this);
	connect(renderer, SIGNAL(ready()), this, SLOT(queue_repaint()));
	connect(renderer, SIGNAL(finished()), renderer, SLOT(deleteLater()));
}

OpenGLWidget::~OpenGLWidget() {
	renderer->cancel();
	delete renderer;
}

void OpenGLWidget::delete_function() {
}
#if 0
void OpenGLWidget::set_waveform_scroll(int s) {
}
#endif
void OpenGLWidget::show_context_menu() {
}

void OpenGLWidget::save_frame() {
}

void OpenGLWidget::queue_repaint() {
	update();
}

void OpenGLWidget::fullscreen_menu_action(QAction *action) {
}

void OpenGLWidget::set_fit_zoom() {
}

void OpenGLWidget::set_custom_zoom() {
}

void OpenGLWidget::set_menu_zoom(QAction* action) {
}

void OpenGLWidget::retry() {
	update();
}

void OpenGLWidget::initializeGL() {
    qDebug()<<"OpenGLWidget::initializeGL";
	initializeOpenGLFunctions();

	connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(context_destroy()), Qt::DirectConnection);

}

void OpenGLWidget::frame_update() {
    qDebug()<<"OpenGLWidget::frame_update";
    ///if(sequence)
    {
    doneCurrent();
    renderer->start_render(context(), nullptr);
    }
}

RenderThread *OpenGLWidget::get_renderer() {
	return renderer;
}

//void OpenGLWidget::resizeGL(int w, int h)
//{
//}

/*void OpenGLWidget::paintEvent(QPaintEvent *e) {
	if (!rendering) {
		QOpenGLWidget::paintEvent(e);
	}
}*/

void OpenGLWidget::seek_from_click(int x) {
}

void OpenGLWidget::context_destroy() {
	makeCurrent();
	//QMetaObject::invokeMethod(renderer, "delete_ctx", Qt::QueuedConnection);
	renderer->delete_ctx();
	doneCurrent();
}

EffectGizmo* OpenGLWidget::get_gizmo_from_mouse(int x, int y) {
	if (gizmos != nullptr) {
		double multiplier = double(glw) / double(width());
		QPoint mouse_pos(qRound(x*multiplier), qRound((height()-y)*multiplier));
		int dot_size = 2 * qRound(GIZMO_DOT_SIZE * multiplier);
		int target_size = 2 * qRound(GIZMO_TARGET_SIZE * multiplier);
		for (int i=0;i<gizmos->gizmo_count();i++) {
			EffectGizmo* g = gizmos->gizmo(i);

			switch (g->get_type()) {
			case GIZMO_TYPE_DOT:
				if (mouse_pos.x() > g->screen_pos[0].x() - dot_size
						&& mouse_pos.y() > g->screen_pos[0].y() - dot_size
						&& mouse_pos.x() < g->screen_pos[0].x() + dot_size
						&& mouse_pos.y() < g->screen_pos[0].y() + dot_size) {
                    qDebug()<<"OpenGLWidget::get_gizmo_from_mouse type: "<<GIZMO_TYPE_DOT<<" index: "<<i;
					return g;
				}
				break;
			case GIZMO_TYPE_POLY:
				if (QPolygon(g->screen_pos).containsPoint(mouse_pos, Qt::OddEvenFill)) {
					return g;
				}
				break;
			case GIZMO_TYPE_TARGET:
				if (mouse_pos.x() > g->screen_pos[0].x() - target_size
						&& mouse_pos.y() > g->screen_pos[0].y() - target_size
						&& mouse_pos.x() < g->screen_pos[0].x() + target_size
						&& mouse_pos.y() < g->screen_pos[0].y() + target_size) {
					return g;
				}
				break;
			}
		}
	}
	return nullptr;
}

void OpenGLWidget::move_gizmos(QMouseEvent *event, bool done) {
	if (selected_gizmo != nullptr) {
		double multiplier = double(glw) / double(width());

		int x_movement = qRound((event->pos().x() - drag_start_x)*multiplier);
		int y_movement = qRound((event->pos().y() - drag_start_y)*multiplier);

		//gizmos->gizmo_move(selected_gizmo, x_movement, y_movement, get_timecode(gizmos->parent_clip, gizmos->parent_clip->sequence->playhead), done);
		gizmos->gizmo_move(selected_gizmo, x_movement, y_movement, 0.0, done);

		gizmo_x_mvmt += x_movement;
		gizmo_y_mvmt += y_movement;

		drag_start_x = event->pos().x();
		drag_start_y = event->pos().y();
        qDebug()<<"OpenGLWidget::move_gizmos will call gizmos->field_changed=>viewer_widget->frame_update. drag_start_x: "<<drag_start_x<<" drag_start_y: "<<drag_start_y<<" gizmos->gizmo_count: "<<(gizmos?gizmos->gizmo_count():0)<<" x_movement: "<<x_movement<<" y_movement: "<<y_movement;
		gizmos->field_changed();
	}
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
	/*if (waveform) {
		seek_from_click(event->x());
	} else */if (event->buttons() & Qt::LeftButton) {
		drag_start_x = event->pos().x();
		drag_start_y = event->pos().y();

		gizmo_x_mvmt = 0;
		gizmo_y_mvmt = 0;

		selected_gizmo = get_gizmo_from_mouse(event->pos().x(), event->pos().y());
        qDebug()<<"OpenGLWidget::mousePressEvent drag_start_x: "<<drag_start_x<<" drag_start_y: "<<drag_start_y
            <<" gizmos: "<<gizmos<<" gizmos->gizmo_count: "<<(gizmos?gizmos->gizmo_count():0)<<" selected_gizmo: "<<selected_gizmo;
		if (selected_gizmo != nullptr) {
			selected_gizmo->set_previous_value();
		}
	}
	dragging = true;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
	unsetCursor();
	/*if (panel_timeline && panel_timeline->tool == TIMELINE_TOOL_HAND) {
		setCursor(Qt::OpenHandCursor);
	}*/
	if (dragging) {
		/*if (waveform) {
			seek_from_click(event->x());
		} else*/ if (event->buttons() & Qt::LeftButton) {
			if (gizmos == nullptr) {
				QDrag* drag = new QDrag(this);
				QMimeData* mimeData = new QMimeData;
				mimeData->setText("h"); // QMimeData will fail without some kind of data
				drag->setMimeData(mimeData);
				drag->exec();
				dragging = false;
			} else {
				move_gizmos(event, false);
			}
		}
	} else {
		EffectGizmo* g = get_gizmo_from_mouse(event->pos().x(), event->pos().y());
		if (g != nullptr) {
			if (g->get_cursor() > -1) {
				setCursor(static_cast<enum Qt::CursorShape>(g->get_cursor()));
			}
		}
	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) 
{
	if (dragging
			&& gizmos != nullptr
			&& event->button() == Qt::LeftButton
			/*&& (panel_timeline && panel_timeline->tool != TIMELINE_TOOL_HAND)*/) { //storm
		move_gizmos(event, true);
	}
	dragging = false;
}

void OpenGLWidget::close_window() {
}
#if 0
void OpenGLWidget::draw_waveform_func() {
}
#endif
void OpenGLWidget::draw_title_safe_area() {return;
	double halfWidth = 0.5;
	double halfHeight = 0.5;
	double viewportAr = (double) width() / (double) height();
	double halfAr = viewportAr*0.5;
#if 0
	if (config.use_custom_title_safe_ratio && config.custom_title_safe_ratio > 0) {
		if (config.custom_title_safe_ratio > viewportAr) {
			halfHeight = (config.custom_title_safe_ratio/viewportAr)*0.5;
		} else {
			halfWidth = (viewportAr/config.custom_title_safe_ratio)*0.5;
		}
	}
#endif
	glLoadIdentity();
	glOrtho(-halfWidth, halfWidth, halfHeight, -halfHeight, 0, 1);

	glColor4f(0.66f, 0.66f, 0.66f, 1.0f);
	glBegin(GL_LINES);

	// action safe rectangle
	glVertex2d(-0.45, -0.45);
	glVertex2d(0.45, -0.45);
	glVertex2d(0.45, -0.45);
	glVertex2d(0.45, 0.45);
	glVertex2d(0.45, 0.45);
	glVertex2d(-0.45, 0.45);
	glVertex2d(-0.45, 0.45);
	glVertex2d(-0.45, -0.45);

	// title safe rectangle
	glVertex2d(-0.4, -0.4);
	glVertex2d(0.4, -0.4);
	glVertex2d(0.4, -0.4);
	glVertex2d(0.4, 0.4);
	glVertex2d(0.4, 0.4);
	glVertex2d(-0.4, 0.4);
	glVertex2d(-0.4, 0.4);
	glVertex2d(-0.4, -0.4);

	// horizontal centers
	glVertex2d(-0.45, 0);
	glVertex2d(-0.375, 0);
	glVertex2d(0.45, 0);
	glVertex2d(0.375, 0);

	// vertical centers
	glVertex2d(0, -0.45);
	glVertex2d(0, -0.375);
	glVertex2d(0, 0.45);
	glVertex2d(0, 0.375);

	glEnd();

	// center cross
	glLoadIdentity();
	glOrtho(-halfAr, halfAr, 0.5, -0.5, -1, 1);

	glBegin(GL_LINES);

	glVertex2d(-0.05, 0);
	glVertex2d(0.05, 0);
	glVertex2d(0, -0.05);
	glVertex2d(0, 0.05);

	glEnd();
}

void OpenGLWidget::draw_gizmos() {
	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	float dot_size = GIZMO_DOT_SIZE / width() * glw;
	float target_size = GIZMO_TARGET_SIZE / width() * glw;

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, glw, 0, glh, -1, 10);
	float gizmo_z = 0.0f;
	for (int j=0;j<gizmos->gizmo_count();j++) {
		EffectGizmo* g = gizmos->gizmo(j);
		glColor4f(g->color.redF(), g->color.greenF(), g->color.blueF(), 1.0);
		switch (g->get_type()) {
		case GIZMO_TYPE_DOT: // draw dot
			glBegin(GL_QUADS);
			glVertex3f(g->screen_pos[0].x()-dot_size, g->screen_pos[0].y()-dot_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x()+dot_size, g->screen_pos[0].y()-dot_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x()+dot_size, g->screen_pos[0].y()+dot_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x()-dot_size, g->screen_pos[0].y()+dot_size, gizmo_z);
			glEnd();
			break;
		case GIZMO_TYPE_POLY: // draw lines
			glBegin(GL_LINES);
			for (int k=1;k<g->get_point_count();k++) {
				glVertex3f(g->screen_pos[k-1].x(), g->screen_pos[k-1].y(), gizmo_z);
				glVertex3f(g->screen_pos[k].x(), g->screen_pos[k].y(), gizmo_z);
			}
			glVertex3f(g->screen_pos[g->get_point_count()-1].x(), g->screen_pos[g->get_point_count()-1].y(), gizmo_z);
			glVertex3f(g->screen_pos[0].x(), g->screen_pos[0].y(), gizmo_z);
			glEnd();
			break;
		case GIZMO_TYPE_TARGET: // draw target
			glBegin(GL_LINES);
			glVertex3f(g->screen_pos[0].x()-target_size, g->screen_pos[0].y()-target_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x()+target_size, g->screen_pos[0].y()-target_size, gizmo_z);

			glVertex3f(g->screen_pos[0].x()+target_size, g->screen_pos[0].y()-target_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x()+target_size, g->screen_pos[0].y()+target_size, gizmo_z);

			glVertex3f(g->screen_pos[0].x()+target_size, g->screen_pos[0].y()+target_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x()-target_size, g->screen_pos[0].y()+target_size, gizmo_z);

			glVertex3f(g->screen_pos[0].x()-target_size, g->screen_pos[0].y()+target_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x()-target_size, g->screen_pos[0].y()-target_size, gizmo_z);

			glVertex3f(g->screen_pos[0].x()-target_size, g->screen_pos[0].y(), gizmo_z);
			glVertex3f(g->screen_pos[0].x()+target_size, g->screen_pos[0].y(), gizmo_z);

			glVertex3f(g->screen_pos[0].x(), g->screen_pos[0].y()-target_size, gizmo_z);
			glVertex3f(g->screen_pos[0].x(), g->screen_pos[0].y()+target_size, gizmo_z);
			glEnd();
			break;
		}
	}
	glPopMatrix();

	glColor4f(color[0], color[1], color[2], color[3]);
}

void OpenGLWidget::paintGL() {
	/*if (waveform) {
		draw_waveform_func();
	} else */{
		renderer->mutex.lock();
        qDebug()<<"OpenGLWidget::paintGL renderer->texColorBuffer: "<<renderer->texColorBuffer;

		makeCurrent();

		// clear to solid black

        qDebug()<<"OpenGLWidget::paintGL width: "<<width()<<" height: "<<height();
#if 0
		if(!renderer->fboOverlay && !renderer->texColorBuffer) 
        {
            glClearColor(1.0, 1.0, 1.0, 1.0);
            glViewport(0, 0, glw, glh);
            goto END_paint;
        }
        else
            glClearColor(0.0, 0.0, 0.0, 0.0); //第四个参数alpha必须为0,否则显示不出图像
#endif
        glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, glw, glh);

		// set color multipler to straight white
		///glColor4f(0.0, 0.0, 0.0, 0.0);

		glEnable(GL_TEXTURE_2D);

		// set screen coords to widget size
        glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, -1, 1);

		// draw texture from render thread

		if(renderer->fboOverlay) 
        {
            glBindTexture(GL_TEXTURE_2D, renderer->fboOverlay->texture());
#if 0
            QImage imgOverlay=renderer->fboOverlay->toImage();
            imgOverlay.save(QString("openglwidget.png"));
#endif
            qDebug()<<"OpenGLWidget fboOverlay->texture: "<<renderer->fboOverlay->texture();
        }
        else if(renderer->texColorBuffer)
        {
            glBindTexture(GL_TEXTURE_2D, renderer->texColorBuffer);
        }

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
        ////saveOpenglBuffer(); //storm
		glBindTexture(GL_TEXTURE_2D, 0);
#if 0
		// draw title/action safe area
		if (config.show_title_safe_area) {
			draw_title_safe_area();
		}
#endif
		gizmos = renderer->gizmos;
		if (gizmos != nullptr) {
			draw_gizmos();
		}

		glDisable(GL_TEXTURE_2D);
        glPopMatrix();
END_paint:
		renderer->mutex.unlock();

		if (renderer->did_texture_fail()) {
			doneCurrent();
			renderer->start_render(context(), nullptr);
		}
	}
}
void OpenGLWidget::create_effect_ui(Clip* c)
{
    if(c)
    {
        qDebug()<<"OpenGLWidget::create_effect_ui clip: "<<c;
#if 0
        Effect* e = create_effect(c, get_internal_meta(EFFECT_INTERNAL_TRANSFORM, EFFECT_TYPE_EFFECT));
        e->set_enabled(true);
        c->effects.append(e);
#else
        c->createEffect();
#endif
    }
    else
    {
        qInfo()<<"OpenGLWidget::create_effect_ui error no clip";
    }
}
