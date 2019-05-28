#include "effectgizmo.h"

#include "effectfield.h"
#include <QDebug>

EffectGizmo::EffectGizmo(int type) :
    x_field1(nullptr),
    x_field_multi1(1.0),
    y_field1(nullptr),
    y_field_multi1(1.0),
    x_field2(nullptr),
    x_field_multi2(1.0),
    y_field2(nullptr),
    y_field_multi2(1.0),
    type(type),
    cursor(-1)
{
    int point_count = (type == GIZMO_TYPE_POLY) ? 4 : 1;
    world_pos.resize(point_count);
    screen_pos.resize(point_count);

    color = Qt::white;
}

void EffectGizmo::set_previous_value() {
    if (x_field1 != nullptr) (x_field1->ui_element)->set_previous_value();
    if (y_field1 != nullptr) (y_field1->ui_element)->set_previous_value();
    if (x_field2 != nullptr) (x_field2->ui_element)->set_previous_value();
    if (y_field2 != nullptr) (y_field2->ui_element)->set_previous_value();
}

int EffectGizmo::get_point_count() {
    return world_pos.size();
}

int EffectGizmo::get_type() {
    return type;
}

int EffectGizmo::get_cursor() {
    if(cursor!=-1)
    {
        qDebug()<<"EffectGizmo::get_cursor type: "<<type<<" cursor: "<<cursor;
    }
    return cursor;
}

void EffectGizmo::set_cursor(int c) {
    qDebug()<<"EffectGizmo::set_cursor type: "<<type<<" cursor: "<<c;
    cursor = c;
}
