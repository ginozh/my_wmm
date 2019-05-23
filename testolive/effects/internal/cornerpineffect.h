#ifndef CORNERPINEFFECT_H
#define CORNERPINEFFECT_H

#include "project/effect.h"

class CornerPinEffect : public Effect {
    Q_OBJECT
public:
    CornerPinEffect(Clip* c, const EffectMeta* em);
    void process_coords(double timecode, GLTextureCoords& coords, int data);
	void process_shader(double timecode, GLTextureCoords& coords);
    void gizmo_draw(double timecode, GLTextureCoords& coords);
private:
    EffectField* top_left_x;
    EffectField* top_left_y;
    EffectField* top_right_x;
    EffectField* top_right_y;
    EffectField* bottom_left_x;
    EffectField* bottom_left_y;
    EffectField* bottom_right_x;
    EffectField* bottom_right_y;
	EffectField* perspective;

    EffectGizmo* top_left_gizmo;
    EffectGizmo* top_right_gizmo;
    EffectGizmo* bottom_left_gizmo;
    EffectGizmo* bottom_right_gizmo;
};

#endif // CORNERPINEFFECT_H
