http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/

ok:
1, opengl_qt
2, otheropengl/framebufferobject
3, opengl_qt_multex: qt对象、支持多个frag
4, opengl_qtobject_multex_widget_thread
5, qt_qglwidget_thread_fro_multex_win
解决问题点：
    1), qopenglwidget闪屏问题(异步更新)
    2), nvdia机器不显示图片或者偶尔显示图片

尝试：1，qglwidget、thread、share fbo texture program、update    
        在子线程中update widget

结论:
1,使用QGLWiget,显示的更新widget
2,hiddenwiget的context与glwidget共享之前，移到子线程中去，这样可以共享opengl资源,否则某些机器不能共享数据
3,hiddenwiget、glwidget有各自的fbo数组，他们之间交换数据之前需要将各自fbo转化为texture
