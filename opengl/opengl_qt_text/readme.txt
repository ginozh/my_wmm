千里之行始于足下

1、抽出glwidget
2、把跟gpu相关的text代码全部加入到glwidget?

流程
1, 初始化glwidget+shader
追加PlayerPrivate

字体:.vert、.shader
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
