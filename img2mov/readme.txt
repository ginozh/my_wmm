一、主线程
1、批量导入图片
1.1、显示在当前图片索引后面，同时保存图片像素大小、文件位置
1.2、connect视频生成结束信号
1.3、调用辅助线程。
    数据结构包括：
    buffer
        指针;
        大小;
        最大大小;
    图片数组
        图片路径buffer;
        原始图片内容buffer;
        小图片内容buffer;
        图片视频buffer;
    总的视频buffer;
    

2、选择动画
2.1、选择某个动画
2.2、调用辅助线程

二、辅助线程
1、对应主线程1.3： 接收参数为图片数组，总视频buffer
    1.1、ffmpeg转化为小图片(min(384*, 图片最小像素));
    1.2、ffmpeg生成各个图片视频
        N秒: ./ffmpeg -y -framerate 1/N -i 'jpg/img001.jpg'  jpg/mi.mp4
    1.3、ffmpeg合成总视频
        ./ffmpeg_g -y -f concat -i mylist.txt  -c copy jpg/mm.mp4
        大概耗费0.08s
        如果是多个图片合成
    1.4、信号通知主线程完毕
2、对应主线程2.2： 接收参数为图片数组，
    需要动画的开始图片索引、结束图片索引(可能2个图片合成或者单个图片合成)，生成的视频索引，动画名称
    2.1、ffmpeg生成视频
    2.2、ffmpeg合成总视频


前提要求：
1、ffmpeg可重入：不会发生内存泄漏; 
2、ffmpeg性能优化：可选
3、ffmpeg线程安全：支持多线程提升性能；多线程同时调用ffmpeg，此时需要消除全局变量

测试：
1、主线程、ffmpeg线程传递QString：测试主、工作线程QString传递是否有问题，即主线程调用工作线程后修改了QString、工作线程发送信号之后也修改了QString，看看是否传递后的QString是否会变化
其实只要看QString得赋值是否是指针赋值就ok了
肯定不是指针赋值，因为函数返回参数的类型可以是QString

三、文字属性同步
element.cpp创建
0、点击tabHome中的caption，类似于双击了linetext
1、双击linetext,转到tabText，同时videotext获取焦点
2、修改tabText，更新videotext，修改属性
3、修改vdiotext，更新linetext，修改linetext内容超出范围省略号显示
