#include "effectfield.h"

#include "effectrow.h"
#include "effect.h"
#include "io/math.h"

#include <QDateTime>
#include <QtMath>

#define GLFRAMERATE 20

EffectField::EffectField(EffectRow *parent, int t, const QString &i) :
	parent_row(parent),
	type(t),
	id(i)
{
    qDebug()<<"EffectField::EffectField t: "<<t;
    ui_element = new Element();
}

QVariant EffectField::get_previous_data() {
	switch (type) {
	case EFFECT_FIELD_DOUBLE: return ui_element->getPreviousValue();
	///case EFFECT_FIELD_COLOR: return static_cast<ColorButton*>(ui_element)->getPreviousValue();
	///case EFFECT_FIELD_STRING: return static_cast<TextEditEx*>(ui_element)->getPreviousValue();
	case EFFECT_FIELD_BOOL: return !ui_element->isChecked();
	case EFFECT_FIELD_COMBO: return ui_element->getPreviousIndex();
	///case EFFECT_FIELD_FONT: return static_cast<FontCombobox*>(ui_element)->getPreviousValue();
	///case EFFECT_FIELD_FILE: return static_cast<EmbeddedFileChooser*>(ui_element)->getPreviousValue();
	}
	return QVariant();
}

QVariant EffectField::get_current_data() {
	switch (type) {
	case EFFECT_FIELD_DOUBLE: return ui_element->value();
	///case EFFECT_FIELD_COLOR: return static_cast<ColorButton*>(ui_element)->get_color();
	///case EFFECT_FIELD_STRING: return static_cast<TextEditEx*>(ui_element)->getPlainTextEx();
	case EFFECT_FIELD_BOOL: return ui_element->isChecked();
	case EFFECT_FIELD_COMBO: return ui_element->currentIndex();
	///case EFFECT_FIELD_FONT: return static_cast<FontCombobox*>(ui_element)->currentText();
	///case EFFECT_FIELD_FILE: return static_cast<EmbeddedFileChooser*>(ui_element)->getFilename();
	}
	return QVariant();
}

double EffectField::frameToTimecode(long frame) {
	return (double(frame) / GLFRAMERATE);
}

long EffectField::timecodeToFrame(double timecode) {
	return qRound(timecode * GLFRAMERATE);
}

void EffectField::set_current_data(const QVariant& data) {
	switch (type) {
	case EFFECT_FIELD_DOUBLE: return ui_element->set_value(data.toDouble(), false);
	//case EFFECT_FIELD_COLOR: return static_cast<ColorButton*>(ui_element)->set_color(data.value<QColor>());
	//case EFFECT_FIELD_STRING: return static_cast<TextEditEx*>(ui_element)->setPlainTextEx(data.toString());
	case EFFECT_FIELD_BOOL: return ui_element->setChecked(data.toBool());
	case EFFECT_FIELD_COMBO: return ui_element->setCurrentIndexEx(data.toInt());
	///case EFFECT_FIELD_FONT: return static_cast<FontCombobox*>(ui_element)->setCurrentTextEx(data.toString());
	///case EFFECT_FIELD_FILE: return static_cast<EmbeddedFileChooser*>(ui_element)->setFilename(data.toString());
	}
}

void EffectField::ui_element_change() {
	emit changed();
}
bool EffectField::is_enabled() {
	return ui_element->isEnabled();
}

void EffectField::set_enabled(bool e) {
	ui_element->setEnabled(e);
}

double EffectField::get_double_value(double timecode, bool async) {
	return (ui_element)->value();
}

void EffectField::set_double_value(double v) {
	(ui_element)->set_value(v, false);
}

void EffectField::set_double_default_value(double v) {
	(ui_element)->set_default_value(v);
}

void EffectField::set_double_minimum_value(double v) {
	(ui_element)->set_minimum_value(v);
}

void EffectField::set_double_maximum_value(double v) {
	(ui_element)->set_maximum_value(v);
}

void EffectField::add_combo_item(const QString& name, const QVariant& data) 
{
}

int EffectField::get_combo_index(double timecode, bool async) {
	return (ui_element)->currentIndex();
}

QVariant EffectField::get_combo_data(double timecode) {
	return "";
}

QString EffectField::get_combo_string(double timecode) {
    return "";
}

void EffectField::set_combo_index(int index) {
	(ui_element)->setCurrentIndexEx(index);
}

void EffectField::set_combo_string(const QString& s) {
}

bool EffectField::get_bool_value(double timecode, bool async) {
	return (ui_element)->isChecked();
}

void EffectField::set_bool_value(bool b) {
	return (ui_element)->setChecked(b);
}

QString EffectField::get_string_value(double timecode, bool async) {
    return "";
}

void EffectField::set_string_value(const QString& s) {
}

QString EffectField::get_font_name(double timecode, bool async) 
{
    return "";
}

void EffectField::set_font_name(const QString& s) {
}

QColor EffectField::get_color_value(double timecode, bool async) {
    return QColor();
}

void EffectField::set_color_value(QColor color) {
}

QString EffectField::get_filename(double timecode, bool async) {
    return "";
}

void EffectField::set_filename(const QString &s) {
}
Element::Element() 
{
}
int Element::getPreviousIndex() {
	return previousIndex;
}
int Element::currentIndex() {
	return index;
}
void Element::setCurrentIndexEx(int idx) {
	index=idx;
}
bool Element::isChecked()
{
    return ischecked;
}
void Element::setChecked(bool checked)
{
    ischecked=checked;
}
double Element::getPreviousValue() {
    return previous_value;
}
void Element::set_previous_value() {
    previous_value = internal_value;
}
double Element::value() {
    return internal_value;
}
void Element::set_value(double v, bool userSet) {
    set = true;
    if (v != internal_value) {
        if (min_enabled && v < min_value) {
            internal_value = min_value;
        } else if (max_enabled && v > max_value) {
            internal_value = max_value;
        } else {
            internal_value = v;
        }

		///setText(valueToString(internal_value));
        ///if (userSet) emit valueChanged();
    }
}
void Element::set_default_value(double v) {
    default_value = v;
	if (!set) {
		set_value(v, false);
		set = false;
	}
}
void Element::set_maximum_value(double v) {
    max_value = v;
    max_enabled = true;
}
bool Element::isEnabled()
{
    return isenabled;
}
void Element::setEnabled(bool e)
{
    isenabled=e;
}
void Element::set_minimum_value(double v) {
    min_value = v;
    min_enabled = true;
}
