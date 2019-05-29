#ifndef EFFECTGIZMO_H
#define EFFECTGIZMO_H

#define GIZMO_TYPE_DOT 0
#define GIZMO_TYPE_POLY 1
#define GIZMO_TYPE_TARGET 2

#define GIZMO_DOT_SIZE 2.5
#define GIZMO_TARGET_SIZE 5.0

#include <QString>
#include <QRect>
#include <QPoint>
#include <QVector>
#include <QColor>

class EffectField;

class EffectGizmo
{
public:
	EffectGizmo(int type);

	QVector<QPoint> world_pos;
	QVector<QPoint> screen_pos;

	EffectField* x_field1;
	double x_field_multi1;
	EffectField* y_field1;
	double y_field_multi1;
	EffectField* x_field2;
	double x_field_multi2;
	EffectField* y_field2;
	double y_field_multi2;

	void set_previous_value();

	QColor color;
	int get_point_count();

	int get_type();

	int get_cursor();
	void set_cursor(int c);
    void setUniformScale(bool u); 
    bool isUniformScale();
private:
	int type;
	bool uniformscale=false;
	int cursor;
};

#endif // EFFECTGIZMO_H
