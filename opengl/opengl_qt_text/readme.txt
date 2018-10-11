零、final
1, 初始化.
1.1, text.vert、text.frag
1.2, 

一、千里之行始于足下

1、抽出glwidget
2、把跟gpu相关的text代码全部加入到glwidget?

二、流程
1, 初始化glwidget+shader
追加PlayerPrivate

字体:.vert、.shader

2, 界面初始时
2.1)选择字体，字体属性。文字默认字体
2.2)根据字体family名称,查找、加载face
查看map中是否存在该字体(大小)，如果不存在则new atlas，
如果存在则refcount++；
原来的字体相应的refcount--, 如果refcount==0时删除
2.3)遍历每个文字以及字体属性，查找、加载合适的face

glyph在face中的大小--ok
不同字体的glyph的行内间距、上下行间距--no 取最大值?

三、难点问题
1, 所有相关字体的glyph图像数据都copy到atlas中去,中途是否可以追加和更新？
如果不行，可能每个字体(大小)都要有一个自己的atlas. --ok
为了不影响其它clip,可能一个clip需要一个atlas --no
每个clip都需要一个atlas, atlas根据字体大小、预计字数有一个初始值大小,一旦可能超过atlas则需要加倍atlas大小,将历史数据加载到atlas中去

2, 字符参数
2.1 示例text: Qjgab01
2.2 参数解释以及值
参数赋值: texture-font.c::texture_font_load_glyph
font->height(字体最高高度?):  31.43 <- texture-font.c::texture_font_init
                                metrics = face->size->metrics;self->height = (metrics.height >> 6) / 100.0;
pen->x,y(文字在左下为原点坐标的左上坐标点 - font->height): 5,468.57(预设值为5,500?)
glyph->offset_x,y(文字本身的开始着笔左上坐标?): Q(1,20)j(-1,20)g(1,15)a(1,15)b(2,20) <-face->glyph->bitmap_top
glyph->width,height(文字宽高):                  Q(20,24)j(7,27)g(15,22)a(15,16)b(15,21)<-face->bitmap.width+pad
glyph->advance(文字占位宽度):                   Q(21.25)j(7.5)g(17.1406)a(16.5469)b(17.1406)<-face->glyph->advance.x
kerning(字距调整): 0
x0,y0(左上开始落笔坐标): pen->x + glyph->offset_x, pen->y + glyph->offset_y
                                                Q(6,488)j(25,488)g(34,483)a(69,483)b(85,488)
s0,t0(texture左上坐标 x,y)            Q(0.0117188,0.00195313)j(0.0507813,0.00195313)g(0.0644531,0.00195313)
                                        <-texture-font.c::texture_font_load_glyph texture_atlas_get_region
s1,t1(texture右下坐标 x,y)            Q(0.0507813,0.0488281) j(0.0644531,0.0546875) g(0.09375  ,0.0449219)

hinting(字体微调): https://zh.wikipedia.org/wiki/字体微调 
2.3 不同的字体融合,例如选择是Brush Script MT字体，但是输入了汉字
非汉字选用Brush Script MT，如果是汉字则选用系统合适的
font->height选用各个字体中最大值, atlas需要销毁?,

#if 0
2, 选择字体, 初始化font
markup_t: 字体颜色(画笔颜色)、下划线、粗体、斜体
2.1) 一旦选择字体
if 没有此字体: QFont => getFontFile
texture_atlas_t => vertex_buffer_t => texture_font_t count
glTexImage2D atlas

2.2) add_text
根据画笔起始位置计算保存文件的vertex信息


2.3) display
一个一个字显示 -> BOA
特效:motion

2.4) 编辑转态不显示字体

三、待解决项
1, 字体属性: 字体颜色(画笔颜色)、下划线、粗体、斜体 --参考glfreetype
2, qfont中文名字 => 英文名? 无法用中文查找英文, --参考libass
3, 英文字体包含汉字,其中的汉字需要使用默认字体  --参考libass
3.1,加载选择的family字体，循环遍历输入的文字，找出不能改字体没有包含的文字(FT_Get_Char_Index);
3.2,选择合适的字体，参考face_idx = *face_index = add_face(fontsel, font, symbol);ass_font_select;libass
ass_directwrite.c 

libass
3.1）初始化idirectwrite接口. 加载所有字体文件信息, 特别加载指定的字体到ASS_Font->faces?
#0  ass_directwrite_add_provider (lib=0x2d2a890, selector=0x48b57f0,
    config=0x0) at ass_directwrite.c:755
add_font
ass_fontselect.c::ass_font_provider_add_font
#1  0x0000000000406e35 in ass_fontselect_init (library=0x2d2a890,
    ftlibrary=0x2d2a950, family=family@entry=0x4330d5 "sans-serif",
    path=path@entry=0x0, config=config@entry=0x0,
    dfp=dfp@entry=ASS_FONTPROVIDER_AUTODETECT) at ass_fontselect.c:1017
#2  0x000000000040e0a2 in ass_set_fonts (priv=0x4833d70,
    default_font=default_font@entry=0x0,
    default_family=default_family@entry=0x4330d5 "sans-serif",
    dfp=dfp@entry=1, config=config@entry=0x0, update=update@entry=1)
    at ass_render_api.c:155
#3  0x0000000000430372 in init (frame_w=1280, frame_h=720) at test.c:111
#4  main (argc=<optimized out>, argv=<optimized out>) at test.c:208
3.2), 显示文字-获取face_index, glyph_index
如果当前文字在faces中没有找到:FT_Get_Char_Index返回为0
则追加faces: add_face(fontsel, font, symbol);
#0  find_font (bold=bold@entry=700, italic=italic@entry=100,
    index=index@entry=0x79fa08,
    postscript_name=postscript_name@entry=0x79fa10, uid=uid@entry=0x79fa0c,
    stream=stream@entry=0x79fa20, code=code@entry=27979,
    name_match=name_match@entry=0x79f88f, library=0xfa890,
    priv=<optimized out>, priv=<optimized out>, meta=...)
    at ass_fontselect.c:541
根据bold、default_meta.fullnames()等打分找到最合适的字体
for (int i = 0; i < meta.n_fullname; i++) {
    const char *fullname = meta.fullnames[i];
    for (int x = 0; x < priv->n_font; x++) {
        ASS_FontInfo *font = &priv->font_infos[x];
        if (matches_family_name(font, fullname))
        if (!check_glyph(font, code))
#1  0x0000000000405f57 in select_font (priv=priv@entry=0x4a057f0,
    library=library@entry=0xfa890, family=<optimized out>,
    family@entry=0x4d36670 "Microsoft JhengHei UI", bold=bold@entry=700,
    italic=italic@entry=100, index=index@entry=0x79fa08,
    postscript_name=postscript_name@entry=0x79fa10, uid=uid@entry=0x79fa0c,
    stream=stream@entry=0x79fa20, code=code@entry=27979)
    at ass_fontselect.c:650

    result = find_font(priv, library, meta, bold, italic, index,
            postscript_name, uid, stream, code, &name_match);
#2  0x0000000000406af9 in ass_font_select (priv=priv@entry=0x4a057f0,
    library=0xfa890, font=font@entry=0x4d322c0, index=index@entry=0x79fa08,
    postscript_name=postscript_name@entry=0x79fa10, uid=uid@entry=0x79fa0c,
    data=data@entry=0x79fa20, code=code@entry=27979) at ass_fontselect.c:722
ASS_FontProvider *default_provider = priv->default_provider;
const char *search_family = family;
          if (!search_family || !*search_family)
              search_family = "Arial";
          char *fallback_family = default_provider->funcs.get_fallback(
                  default_provider->priv, search_family, code); //storm 得到Microsoft JhengHei UI
            get_fallback (priv=0x4b10290, base=0x4b44260 "Brush Script MT", codepoint=27979) at ass_directwrite.c:431
res = select_font(priv, library, fallback_family, bold, italic,
                      index, postscript_name, uid, data, code);
#3  0x0000000000426d3c in add_face (fontsel=fontsel@entry=0x4a057f0,
    font=font@entry=0x4d322c0, ch=ch@entry=27979) at ass_font.c:149

    path = ass_font_select(fontsel, font->library, font , &index,
             &postscript_name, &uid, &stream, ch); //获取到family
    //无文件路径时给定获取数据的函数：stream->func = provider->funcs.get_data
    error = FT_Open_Face(font->ftlibrary, &args, index, &face);
        read_stream_font(FT_Stream stream, unsigned long offset, unsigned char *buffer, unsigned long count)
            get_data (data=0x4849090, buf=0x0, offset=0, length=0) at ass_directwrite.c:307

#4  0x0000000000427494 in ass_font_get_index (fontsel=0x4a057f0,
    font=0x4d322c0, symbol=27979, face_index=face_index@entry=0x4be02a0,
    glyph_index=glyph_index@entry=0x4be02a4) at ass_font.c:504
        face_idx = *face_index = add_face(fontsel, font, symbol);
        face = font->faces[face_idx];
        index = FT_Get_Char_Index(face, ass_font_index_magic(face, symbol));
        
        *face_index  = FFMAX(*face_index, 0);
        *glyph_index = index;
#5  0x00000000004122e2 in ass_shaper_find_runs (shaper=<optimized out>,
    render_priv=render_priv@entry=0x4983d70, glyphs=<optimized out>, len=15)
    at ass_shaper.c:751
#6  0x000000000040adeb in ass_render_event (event_images=<optimized out>,
    event=<optimized out>, render_priv=0x4983d70) at ass_render.c:2334
init_render_context(render_priv, event); //初始化render_priv->state.font
parse_events(render_priv, event) // 解析ass的events文本, 填充render_priv->text_info数组
                                 //数据元素GlyphInfo: font, symbol, face_index, glyph_index
TextInfo *text_info = &render_priv->text_info;
ass_shaper_find_runs(render_priv->shaper, render_priv, text_info->glyphs, text_info->length);
#7  ass_render_frame (priv=<optimized out>, track=<optimized out>,
    track@entry=0x4d13240, now=<optimized out>, now@entry=1000,
    detect_change=<optimized out>, detect_change@entry=0x0)
    at ass_render.c:2875
#8  0x00000000004303b6 in main (argc=<optimized out>, argv=<optimized out>)
    at test.c:215

3.3), 显示文字-获取glyph
#0  ass_font_get_glyph (font=0x4c42d90, ch=66, face_index=0, index=37,
    hinting=ASS_HINTING_NONE, deco=0) at ass_font.c:542
#1  0x000000000040bb51 in get_outline_glyph (info=0x4b10410, priv=0x2db3d70)
    at ass_render.c:1051
#2  retrieve_glyphs (render_priv=0x2db3d70) at ass_render.c:1836
#3  ass_render_event (event_images=<optimized out>, event=<optimized out>,
    render_priv=0x2db3d70) at ass_render.c:2342
#4  ass_render_frame (priv=<optimized out>, track=<optimized out>,
    track@entry=0x4c43330, now=<optimized out>, now@entry=1000,
    detect_change=<optimized out>, detect_change@entry=0x0)
    at ass_render.c:2875
#5  0x00000000004303f6 in main (argc=<optimized out>, argv=<optimized out>)
    at test.c:215


3.2), 取得font filepath
#0  add_face (fontsel=fontsel@entry=0x48b57f0, font=0x4bc2be0, ch=ch@entry=0)
    at ass_font.c:149
family: path = ass_font_select(fontsel, font->library, font , &index, &postscript_name, &uid, &stream, ch);
        res = select_font(priv, library, family, bold, italic, index,

#1  0x0000000000427122 in ass_font_new (font_cache=<optimized out>,
    library=0x2d2a890, ftlibrary=0x2d2a950, fontsel=0x48b57f0,
    desc=desc@entry=0x79fae0) at ass_font.c:255
#2  0x000000000040e4ca in update_font (
    render_priv=render_priv@entry=0x4833d70) at ass_parse.c:132
#3  0x000000000040a1b9 in reset_render_context (
    render_priv=render_priv@entry=0x4833d70, style=<optimized out>,
    style@entry=0x0) at ass_render.c:838
#4  0x000000000040a9c6 in init_render_context (event=0x4bc4190,
    render_priv=0x4833d70) at ass_render.c:890
#5  ass_render_event (event_images=0x4bc4310, event=0x4bc4190,
    render_priv=0x4833d70) at ass_render.c:2319
#6  ass_render_frame (priv=<optimized out>, track=<optimized out>,
    track@entry=0x4bc3090, now=<optimized out>, now@entry=1000,
    detect_change=<optimized out>, detect_change@entry=0x0)
    at ass_render.c:2875
#7  0x00000000004303b6 in main (argc=<optimized out>, argv=<optimized out>)
    at test.c:215

4, 统一使用glhiddenwidget绘制？还是分开？使用pmv概念 --分开,参考glfreetype

#endif
