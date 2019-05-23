#ifndef TEXTEFFECT_H
#define TEXTEFFECT_H

#include "project/effect.h"

#include <QFont>
#include <QImage>
class QOpenGLTexture;

class TextEffect : public Effect {
	Q_OBJECT
public:
	TextEffect(Clip* c, const EffectMeta *em);
	void redraw(double timecode);

	EffectField* text_val;
	EffectField* size_val;
	EffectField* set_color_button;
	EffectField* set_font_combobox;
	EffectField* halign_field;
	EffectField* valign_field;
	EffectField* word_wrap_field;

	EffectField* outline_bool;
	EffectField* outline_width;
	EffectField* outline_color;

	EffectField* shadow_bool;
	EffectField* shadow_distance;
	EffectField* shadow_color;
	EffectField* shadow_softness;
	EffectField* shadow_opacity;
private slots:
	void outline_enable(bool);
	void shadow_enable(bool);
	void text_edit_menu();
	void open_text_edit();
private:
	QFont font;
};

#endif // TEXTEFFECT_H
