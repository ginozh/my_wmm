#include <QtMath>
#include "Clip.h"
#include "OpenGLWidget.h"
#include "Effects/internal/transformeffect.h"
#include <QXmlStreamReader>
Clip::Clip()
{
}
const EffectMeta* get_meta_from_name(const QString& input) 
{
	int split_index = input.indexOf('/');
	QString category;
	if (split_index > -1) {
		category = input.left(split_index);
	}
	QString name = input.mid(split_index + 1);

	for (int j=0;j<effects.size();j++) {
		if (effects.at(j).name == name
				&& (effects.at(j).category == category
					|| category.isEmpty())) {
			return &effects.at(j);
		}
	}
	return nullptr;
}

void Clip::createEffect()
{
    const EffectMeta* meta = nullptr;

    meta = get_meta_from_name("Distort/Transform");
    //Effect* e = create_effect(this, meta);
    Effect* e = new TransformEffect(this, meta);
    e->set_enabled(true);
    e->setValue("posx", QString::number(m_glwidget->glw/2));
    e->setValue("posy", QString::number(m_glwidget->glh/2));
    //e->setValue("scalex", QString::number(53.2813));
    e->setValue("scalex", QString::number(50));
    e->setValue("scaley", QString::number(80));
    int uniformscale=0;//1: true 0: false
    e->setValue("uniformscale", QString::number(uniformscale));
    ((TransformEffect*)e)->toggle_uniform_scale(uniformscale);
    e->setValue("rotation", QString::number(28.9063));
    //e->setValue("rotation", QString::number(0));
    e->setValue("anchorx", QString::number(0));
    e->setValue("anchory", QString::number(0));
    e->setValue("opacity", QString::number(100));
    e->setValue("blendmode", QString::number(0));
    //e->load(stream);

    this->effects.append(e);
}
#if 0
void Clip::createEffect()
{
    //QString xmlcontent="<effect name=\"Distort/Transform\" enabled=\"1\"> <row> <field id=\"posx\" value=\"849\"/> <field id=\"posy\" value=\"551\"/> </row> <row> <field id=\"scalex\" value=\"100\"/> <field id=\"scaley\" value=\"100\"/> </row> <row> <field id=\"uniformscale\" value=\"1\"/> </row> <row> <field id=\"rotation\" value=\"15.625\"/> </row> <row> <field id=\"anchorx\" value=\"0\"/> <field id=\"anchory\" value=\"0\"/> </row> <row> <field id=\"opacity\" value=\"100\"/> </row> <row> <field id=\"blendmode\" value=\"0\"/> </row> </effect>";
    QString xmlcontent="<effect name=\"Distort/Transform\" enabled=\"1\"> <row> <field id=\"posx\" value=\"640\"/> <field id=\"posy\" value=\"360\"/> </row> <row> <field id=\"scalex\" value=\"53.2813\"/> <field id=\"scaley\" value=\"100\"/> </row> <row> <field id=\"uniformscale\" value=\"1\"/> </row> <row> <field id=\"rotation\" value=\"28.9063\"/> </row> <row> <field id=\"anchorx\" value=\"0\"/> <field id=\"anchory\" value=\"0\"/> </row> <row> <field id=\"opacity\" value=\"100\"/> </row> <row> <field id=\"blendmode\" value=\"0\"/> </row> </effect>";
    QXmlStreamReader stream(xmlcontent);
    bool cancelled=false;
    while (!stream.atEnd() && !cancelled) {
        stream.readNextStartElement();
        if (stream.isStartElement() && (stream.name() == "effect" || stream.name() == "opening" || stream.name() == "closing")) {
            // "opening" and "closing" are backwards compatibility code
            //        load_effect(stream, c);
            int effect_id = -1;
            QString effect_name;
            bool effect_enabled = true;
            long effect_length = -1;
            for (int j=0;j<stream.attributes().size();j++) {
                const QXmlStreamAttribute& attr = stream.attributes().at(j);
                if (attr.name() == "id") {
                    effect_id = attr.value().toInt();
                } else if (attr.name() == "enabled") {
                    effect_enabled = (attr.value() == "1");
                } else if (attr.name() == "name") {
                    effect_name = attr.value().toString();
                } else if (attr.name() == "length") {
                    effect_length = attr.value().toLong();
                }
            }

            // wait for effects to be loaded
            ///panel_effect_controls->effects_loaded.lock();

            const EffectMeta* meta = nullptr;

            // find effect with this name
            if (!effect_name.isEmpty()) {
                meta = get_meta_from_name(effect_name);
            }

            ////panel_effect_controls->effects_loaded.unlock();

            QString tag = stream.name().toString();

            int type=0;
#if 0
            if (tag == "opening") {
                type = TA_OPENING_TRANSITION;
            } else if (tag == "closing") {
                type = TA_CLOSING_TRANSITION;
            } else {
                type = TA_NO_TRANSITION;
            }
#endif
            qDebug()<<"Clip::createEffect effect_name: "<<effect_name<<" meta: "<<meta;
            //create_effect_ui(&stream, c, type, &effect_name, meta, effect_length, effect_enabled);
			Effect* e = create_effect(this, meta);
			e->set_enabled(effect_enabled);
			e->load(stream);

			this->effects.append(e);
        }
    }
}
#endif
