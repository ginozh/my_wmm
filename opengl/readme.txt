http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/

ok:
1, opengl_qt
2, otheropengl/framebufferobject
3, opengl_qt_multex: qt对象、支持多个frag
4, opengl_qtobject_multex_widget_thread
5, qt_qglwidget_thread_fro_multex_forlib
解决问题点：
    1), qopenglwidget闪屏问题(异步更新)
    2), nvdia机器不显示图片或者偶尔显示图片

尝试：1，qglwidget、thread、share fbo texture program、update    
        在子线程中update widget

结论:
1,使用QGLWiget,显示的更新widget(QOpenGLWidget隐式swapbuffer)
2,hiddenwidget的context与gldisplaywidget共享之前，移到子线程中去，这样可以共享opengl资源,否则某些机器不能共享数据
3,hiddenwidget、gldisplaywidget有各自的fbo数组，他们之间交换数据之前需要将各自fbo转化为texture
4,闪屏问题，可能是操作opengl时没有锁或者opengl资源耗尽？
    子线程fbo的操作还未被gpu执行，gui线程使用此fbo对应的texture才会有黑屏现象

工作步骤：
1、确定gldisplaywidget、hiddenwidget函数调用顺序，来决定在哪些函数中初始化glfunction、glcontext、glresource、fbo
    1), 纯粹的gldisplaywidget、hiddenwidget. 确定: 调用顺序;函数互相影响关系.
            QGLFormat::setDoubleBuffer(true);
    resizeEvent;//如果没有调用QGLWidget::resizeEvent，则不会有initializeGL
    setContext;
    initializeGL(); //setContext()之后;onece; paintGL() or resizeGL()之前
    resizeGL(); 
    showEvent();
    resizeEvent();
    resizeGL();
    paintGL();
    2), widgetbase基类, 包含opengl操作
2、gui线程操作gldisplaywidget。只包含2个操作:  
    1), initialOpengl: allEffects; m_mapEffectProgram
    addeffect(allEffects, ParseConfCreateProgram); -- m_mapEffectProgram(effectname, STFragmentInfo ), 
    paintGL(fragRenderForOtherThreadAgain); -- 不需要fbo
3、后台线程操作hiddenwidget.包含：
    1), 初始化: 创建fbo数组(vFboInfo): idxFrameBuf, maxFrameBuf,
    2), initialOpengl: 创建fbo数组(vFboInfo):createBindFramebufferTexture
    

4、gui、后台线程共享数据包括:
    allEffects
    m_mapEffectProgram(effectname, program, shader);
    arrTexturesVar --mSample


注意事项
showevent时可能QOpenGLContext::currentContext()的值还为0，此时所有的makeCurrent都无效
