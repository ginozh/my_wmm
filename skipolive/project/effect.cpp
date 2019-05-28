#include "effect.h"

#include "OpenGLWidget.h"
#include "Clip.h"

#include "effects/internal/transformeffect.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QOpenGLContext>
#include <QDir>
#include <QPainter>
#include <QtMath>
#include <QMenu>
#include <QApplication>
#include <QDebug>
bool shaders_are_enabled = true;
QVector<EffectMeta> effects;

Effect* create_effect(Clip* c, const EffectMeta* em) {
	if (em->internal >= 0 && em->internal < EFFECT_INTERNAL_COUNT) {
        qDebug()<<"effect.cpp::create_effect em->internal: "<<em->internal;
		// must be an internal effect
		switch (em->internal) {
		case EFFECT_INTERNAL_TRANSFORM: return new TransformEffect(c, em);
#if 0
		case EFFECT_INTERNAL_TEXT: return new TextEffect(c, em);
		case EFFECT_INTERNAL_TIMECODE: return new TimecodeEffect(c, em);
		case EFFECT_INTERNAL_SOLID: return new SolidEffect(c, em);
		case EFFECT_INTERNAL_NOISE: return new AudioNoiseEffect(c, em);
		case EFFECT_INTERNAL_VOLUME: return new VolumeEffect(c, em);
		case EFFECT_INTERNAL_PAN: return new PanEffect(c, em);
		case EFFECT_INTERNAL_TONE: return new ToneEffect(c, em);
		case EFFECT_INTERNAL_SHAKE: return new ShakeEffect(c, em);
		case EFFECT_INTERNAL_CORNERPIN: return new CornerPinEffect(c, em);
		case EFFECT_INTERNAL_FILLLEFTRIGHT: return new FillLeftRightEffect(c, em);
#ifndef NOVST
		case EFFECT_INTERNAL_VST: return new VSTHost(c, em);
#endif
#ifndef NOFREI0R
		case EFFECT_INTERNAL_FREI0R: return new Frei0rEffect(c, em);
#endif
#endif
		}
	} else if (!em->filename.isEmpty()) {
		// load effect from file
		return new Effect(c, em);
	} else {
		qCritical() << "Invalid effect data";
		/*QMessageBox::critical(mainWindow,
							  QCoreApplication::translate("Effect", "Invalid effect"),
							  QCoreApplication::translate("Effect", "No candidate for effect '%1'. This effect may be corrupt. Try reinstalling it or Olive.").arg(em->name));*/
	}
	return nullptr;
}

const EffectMeta* get_internal_meta(int internal_id, int type) {
	for (int i=0;i<effects.size();i++) {
		if (effects.at(i).internal == internal_id && effects.at(i).type == type) {
			return &effects.at(i);
		}
	}
	return nullptr;
}

Effect::Effect(Clip* c, const EffectMeta *em) :
	parent_clip(c),
	meta(em),
	enable_shader(false),
	enable_coords(false),
	enable_superimpose(false),
	enable_image(false),
	glslProgram(nullptr),
	texture(nullptr),
	enable_always_update(false),
	isOpen(false),
	bound(false)
{
}

Effect::~Effect() {
	if (isOpen) {
		close();
	}

	for (int i=0;i<rows.size();i++) {
		delete rows.at(i);
	}
	for (int i=0;i<gizmos.size();i++) {
		delete gizmos.at(i);
	}
}

void Effect::copy_field_keyframes(Effect* e) {
	for (int i=0;i<rows.size();i++) {
		EffectRow* row = rows.at(i);
		EffectRow* copy_row = e->rows.at(i);
		copy_row->setKeyframing(row->isKeyframing());
		for (int j=0;j<row->fieldCount();j++) {
			EffectField* field = row->field(j);
			EffectField* copy_field = copy_row->field(j);
			////copy_field->keyframes = field->keyframes;
			copy_field->set_current_data(field->get_current_data());
		}
	}
}

EffectRow* Effect::add_row(const QString& name, bool savable, bool keyframable) {
	EffectRow* row = new EffectRow(this, savable, nullptr, name, rows.size(), keyframable);
	rows.append(row);
	return row;
}

EffectRow* Effect::row(int i) {
	return rows.at(i);
}

int Effect::row_count() {
	return rows.size();
}

EffectGizmo *Effect::add_gizmo(int type) {
	EffectGizmo* gizmo = new EffectGizmo(type);
	gizmos.append(gizmo);
	return gizmo;
}

EffectGizmo *Effect::gizmo(int i) {
	return gizmos.at(i);
}

int Effect::gizmo_count() {
	return gizmos.size();
}

void Effect::refresh() {}

void Effect::field_changed() {
	//sequence_viewer->viewer_widget->frame_update();
    if(parent_clip && parent_clip->m_glwidget)
    {
        parent_clip->m_glwidget->frame_update();
        parent_clip->m_glwidget->update();
    }
    else
    {
        qInfo()<<"Effect::field_changed error no parent_clip or m_glwidget";
    }
}

void Effect::show_context_menu(const QPoint& pos) 
{
}

void Effect::delete_self() 
{
}

void Effect::move_up() 
{
    if(parent_clip && parent_clip->m_glwidget)
    {
        parent_clip->m_glwidget->frame_update();
    }
    else
    {
        qInfo()<<"Effect::move_up error no parent_clip or m_glwidget";
    }
}

void Effect::move_down() 
{
    if(parent_clip && parent_clip->m_glwidget)
    {
        parent_clip->m_glwidget->frame_update();
    }
    else
    {
        qInfo()<<"Effect::move_down error no parent_clip or m_glwidget";
    }
}

int Effect::get_index_in_clip() 
{
	return -1;
}

bool Effect::is_enabled() {
    return true;
}

void Effect::set_enabled(bool b) {return;
}

QVariant load_data_from_string(int type, const QString& string) {
	switch (type) {
	case EFFECT_FIELD_DOUBLE: return string.toDouble();
	case EFFECT_FIELD_COLOR: return QColor(string);
	case EFFECT_FIELD_BOOL: return (string == "1");
	case EFFECT_FIELD_COMBO: return string.toInt();
	case EFFECT_FIELD_STRING:
	case EFFECT_FIELD_FONT:
	case EFFECT_FIELD_FILE:
		 return string;
	}
	return QVariant();
}

QString save_data_to_string(int type, const QVariant& data) {
	switch (type) {
	case EFFECT_FIELD_DOUBLE: return QString::number(data.toDouble());
	case EFFECT_FIELD_COLOR: return data.value<QColor>().name();
	case EFFECT_FIELD_BOOL: return QString::number(data.toBool());
	case EFFECT_FIELD_COMBO: return QString::number(data.toInt());
	case EFFECT_FIELD_STRING:
	case EFFECT_FIELD_FONT:
	case EFFECT_FIELD_FILE:
		return data.toString();
	}
	return QString();
}

void Effect::load(QXmlStreamReader& stream) {
	int row_count = 0;

	QString tag = stream.name().toString();

	while (!stream.atEnd() && !(stream.name() == tag && stream.isEndElement())) {
		stream.readNext();
		if (stream.name() == "row" && stream.isStartElement()) {
			if (row_count < rows.size()) {
				EffectRow* row = rows.at(row_count);
				int field_count = 0;

				while (!stream.atEnd() && !(stream.name() == "row" && stream.isEndElement())) {
					stream.readNext();

					// read field
					if (stream.name() == "field" && stream.isStartElement()) {
						if (field_count < row->fieldCount()) {
							// match field using ID
							int field_number = field_count;
							for (int k=0;k<stream.attributes().size();k++) {
								const QXmlStreamAttribute& attr = stream.attributes().at(k);
								if (attr.name() == "id") {
									for (int l=0;l<row->fieldCount();l++) {
										if (row->field(l)->id == attr.value()) {
											field_number = l;
											qInfo() << "ID: "<<attr.value();
											break;
										}
									}
									break;
								}
							}

							EffectField* field = row->field(field_number);

							// get current field value
							for (int k=0;k<stream.attributes().size();k++) {
								const QXmlStreamAttribute& attr = stream.attributes().at(k);
								if (attr.name() == "value") {
                                    qInfo() << "value: "<<attr.value()<<" field->type: "<<field->type<<" field: "
                                        <<field;
									field->set_current_data(load_data_from_string(field->type, attr.value().toString()));
									break;
								}
							}

							while (!stream.atEnd() && !(stream.name() == "field" && stream.isEndElement())) {
								stream.readNext();
							}
						} else {
							qCritical() << "Too many fields for effect" << id << "row" << row_count << ". Project might be corrupt. (Got" << field_count << ", expected <" << row->fieldCount()-1 << ")";
						}
						field_count++;
					}
				}

			} else {
				qCritical() << "Too many rows for effect" << id << ". Project might be corrupt. (Got" << row_count << ", expected <" << rows.size()-1 << ")";
			}
			row_count++;
		} else if (stream.isStartElement()) {
			custom_load(stream);
		}
	}
}

void Effect::custom_load(QXmlStreamReader &) {}

void Effect::save(QXmlStreamWriter& stream) {
	stream.writeAttribute("name", meta->category + "/" + meta->name);
	stream.writeAttribute("enabled", QString::number(is_enabled()));

	for (int i=0;i<rows.size();i++) {
		EffectRow* row = rows.at(i);
		if (row->savable) {
			stream.writeStartElement("row"); // row
			for (int j=0;j<row->fieldCount();j++) {
				EffectField* field = row->field(j);
				stream.writeStartElement("field"); // field
				stream.writeAttribute("id", field->id);
				stream.writeAttribute("value", save_data_to_string(field->type, field->get_current_data()));
				stream.writeEndElement(); // field
			}
			stream.writeEndElement(); // row
		}
	}
}

bool Effect::is_open() {
	return isOpen;
}

void Effect::validate_meta_path() {
	if (!meta->path.isEmpty() || (vertPath.isEmpty() && fragPath.isEmpty())) return;
	QList<QString> effects_paths ;//= get_effects_paths();
	const QString& test_fn = vertPath.isEmpty() ? fragPath : vertPath;
	for (int i=0;i<effects_paths.size();i++) {
		if (QFileInfo::exists(effects_paths.at(i) + "/" + test_fn)) {
			for (int j=0;j<effects.size();j++) {
				if (&effects.at(j) == meta) {
					effects[j].path = effects_paths.at(i);
					return;
				}
			}
			return;
		}
	}
}

void Effect::open() {
	if (isOpen) {
		qWarning() << "Tried to open an effect that was already open";
		close();
	}
	if (shaders_are_enabled && enable_shader) {
		if (QOpenGLContext::currentContext() == nullptr) {
			qWarning() << "No current context to create a shader program for - will retry next repaint";
		} else {
			glslProgram = new QOpenGLShaderProgram();
			validate_meta_path();
			bool glsl_compiled = true;
			if (!vertPath.isEmpty()) {
				if (glslProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, meta->path + "/" + vertPath)) {
					qInfo() << "Vertex shader added successfully";
				} else {
					glsl_compiled = false;
					qWarning() << "Vertex shader could not be added";
				}
			}
			if (!fragPath.isEmpty()) {
				if (glslProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, meta->path + "/" + fragPath)) {
					qInfo() << "Fragment shader added successfully";
				} else {
					glsl_compiled = false;
					qWarning() << "Fragment shader could not be added";
				}
			}
			if (glsl_compiled) {
				if (glslProgram->link()) {
					qInfo() << "Shader program linked successfully";
				} else {
					qWarning() << "Shader program failed to link";
				}
			}
			isOpen = true;
		}
	} else {
		isOpen = true;
	}

	if (enable_superimpose) {
		texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	}
}

void Effect::close() {
	if (!isOpen) {
		qWarning() << "Tried to close an effect that was already closed";
	}
	delete_texture();
	if (glslProgram != nullptr) {
		delete glslProgram;
		glslProgram = nullptr;
	}
	isOpen = false;
}

bool Effect::is_glsl_linked() {
	return glslProgram != nullptr && glslProgram->isLinked();
}

void Effect::startEffect() {
	if (!isOpen) {
		open();
		qWarning() << "Tried to start a closed effect - opening";
	}
	if (shaders_are_enabled
			&& enable_shader
			&& glslProgram->isLinked()) {
		bound = glslProgram->bind();
	}
}

void Effect::endEffect() {
	if (bound) glslProgram->release();
	bound = false;
}

void Effect::process_image(double, uint8_t *, uint8_t *, int){}

Effect* Effect::copy(Clip* c) {
	Effect* copy = create_effect(c, meta);
	copy->set_enabled(is_enabled());
	copy_field_keyframes(copy);
	return copy;
}

void Effect::process_shader(double timecode, GLTextureCoords&) {
	if (parent_clip == nullptr || parent_clip->m_glwidget == nullptr) {
        qInfo()<<"Effect::process_shader error no parent_clip or m_glwidget";
        return;
    }
	glslProgram->setUniformValue("resolution", parent_clip->m_glwidget->glw, parent_clip->m_glwidget->glh);
	glslProgram->setUniformValue("time", GLfloat(timecode));

	for (int i=0;i<rows.size();i++) {
		EffectRow* row = rows.at(i);
		for (int j=0;j<row->fieldCount();j++) {
			EffectField* field = row->field(j);
			if (!field->id.isEmpty()) {
				switch (field->type) {
				case EFFECT_FIELD_DOUBLE:
					glslProgram->setUniformValue(field->id.toUtf8().constData(), GLfloat(field->get_double_value(timecode)));
					break;
				case EFFECT_FIELD_COLOR:
					glslProgram->setUniformValue(
								field->id.toUtf8().constData(),
								GLfloat(field->get_color_value(timecode).redF()),
								GLfloat(field->get_color_value(timecode).greenF()),
								GLfloat(field->get_color_value(timecode).blueF())
							);
					break;
				case EFFECT_FIELD_STRING: break; // can you even send a string to a uniform value?
				case EFFECT_FIELD_BOOL:
					glslProgram->setUniformValue(field->id.toUtf8().constData(), field->get_bool_value(timecode));
					break;
				case EFFECT_FIELD_COMBO:
					glslProgram->setUniformValue(field->id.toUtf8().constData(), field->get_combo_index(timecode));
					break;
				case EFFECT_FIELD_FONT: break; // can you even send a string to a uniform value?
				case EFFECT_FIELD_FILE: break; // can you even send a string to a uniform value?
				}
			}
		}
	}
}

void Effect::process_coords(double, GLTextureCoords&, int) {}
void Effect::gizmo_draw(double, GLTextureCoords &) {}

void Effect::gizmo_move(EffectGizmo* gizmo, int x_movement, int y_movement, double timecode, bool done) {
	for (int i=0;i<gizmos.size();i++) {
		if (gizmos.at(i) == gizmo) {
			//ComboAction* ca = nullptr;
			//if (done) ca = new ComboAction();
			if (gizmo->x_field1 != nullptr) {
				gizmo->x_field1->set_double_value(gizmo->x_field1->get_double_value(timecode) + x_movement*gizmo->x_field_multi1);
				//gizmo->x_field1->make_key_from_change(ca);
			}
			if (gizmo->y_field1 != nullptr) {
				gizmo->y_field1->set_double_value(gizmo->y_field1->get_double_value(timecode) + y_movement*gizmo->y_field_multi1);
				//gizmo->y_field1->make_key_from_change(ca);
			}
			if (gizmo->x_field2 != nullptr) {
				gizmo->x_field2->set_double_value(gizmo->x_field2->get_double_value(timecode) + x_movement*gizmo->x_field_multi2);
				//gizmo->x_field2->make_key_from_change(ca);
			}
			if (gizmo->y_field2 != nullptr) {
				gizmo->y_field2->set_double_value(gizmo->y_field2->get_double_value(timecode) + y_movement*gizmo->y_field_multi2);
				//gizmo->y_field2->make_key_from_change(ca);
			}
			//if (done) undo_stack.push(ca);
			break;
		}
	}
}

void Effect::gizmo_world_to_screen() {
	if (parent_clip == nullptr || parent_clip->m_glwidget == nullptr) {
        qInfo()<<"Effect::gizmo_world_to_screen error no parent_clip or m_glwidget";
        return;
    }
	GLfloat view_val[16];
	GLfloat projection_val[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, view_val);
	glGetFloatv(GL_PROJECTION_MATRIX, projection_val);

	QMatrix4x4 view_matrix(view_val);
	QMatrix4x4 projection_matrix(projection_val);

	for (int i=0;i<gizmos.size();i++) {
		EffectGizmo* g = gizmos.at(i);

		for (int j=0;j<g->get_point_count();j++) {
			QVector4D screen_pos = QVector4D(g->world_pos[j].x(), g->world_pos[j].y(), 0, 1.0) * (view_matrix * projection_matrix);

			int adjusted_sx1 = qRound(((screen_pos.x()*0.5f)+0.5f)*parent_clip->m_glwidget->glw);
			int adjusted_sy1 = qRound((1.0f-((screen_pos.y()*0.5f)+0.5f))*parent_clip->m_glwidget->glh);
            qDebug()<<"Effect::gizmo_world_to_screen i: "<<i<<" j: "<<j<<" adjusted_sx1: "<<adjusted_sx1<<" adjusted_sy1: "<<adjusted_sy1<<" world_posx: "<<g->world_pos[j].x()<<" world_posy: "<<g->world_pos[j].y();//storm
			g->screen_pos[j] = QPoint(adjusted_sx1, adjusted_sy1);
		}
	}
}

bool Effect::are_gizmos_enabled() {
	return (gizmos.size() > 0);
}

void Effect::redraw(double) 
{
}

bool Effect::valueHasChanged(double timecode) {
	if (cachedValues.size() == 0) {
		for (int i=0;i<row_count();i++) {
			EffectRow* crow = row(i);
			for (int j=0;j<crow->fieldCount();j++) {
				cachedValues.append(crow->field(j)->get_current_data());
			}
		}
		return true;
	} else {
		bool changed = false;
		int index = 0;
		for (int i=0;i<row_count();i++) {
			EffectRow* crow = row(i);
			for (int j=0;j<crow->fieldCount();j++) {
				EffectField* field = crow->field(j);
				////field->validate_keyframe_data(timecode);
				if (cachedValues.at(index) != field->get_current_data()) {
					changed = true;
				}
				cachedValues[index] = field->get_current_data();
				index++;
			}
		}
		return changed;
	}
}

void Effect::delete_texture() {
	if (texture != nullptr) {
		delete texture;
		texture = nullptr;
	}
}

qint16 mix_audio_sample(qint16 a, qint16 b) {
	qint32 mixed_sample = static_cast<qint32>(a) + static_cast<qint32>(b);
	mixed_sample = qMax(qMin(mixed_sample, static_cast<qint32>(INT16_MAX)), static_cast<qint32>(INT16_MIN));
	return static_cast<qint16>(mixed_sample);
}
