一, analyse code 
1, initial
//main.cpp
int main(int argc, char *argv[])
    qmlRegisterSingletonType<FileProcessor>("VideoHW", 0, 1, "FileProcessor", FileProcessorInstance);
    // fileprocessor.h
    Q_PROPERTY(VideoSource* source READ getSource WRITE setSource NOTIFY sourceChanged)
    // fileprocessor.cpp
    static QObject *FileProcessorInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
        return new FileProcessor();
        =>FileProcessor::FileProcessor(QObject *parent)
            m_source = new VideoSource(this);
            m_decoder = HWDecoderFactory::createDecoder(this);
            connect(m_decoder, &HWDecoder::frameDecoded, m_source, &VideoSource::setFrame);

// main.cpp
qmlRegisterType<VideoFBORenderer>("VideoHW", 0, 1, "VideoRenderer");
class VideoFBORenderer : public QQuickFramebufferObject
// main.qml
VideoRenderer {
    source: FileProcessor.source
}
void VideoFBORenderer::setSource(VideoSource *source)
    connect(m_source, &VideoSource::frameReady, this, &VideoFBORenderer::onFrameReady);

2, start
// main.qml . click "Start"
FileProcessor.processMedia(openDialog.fileUrl);
// fileprocessor.cpp
=>void FileProcessor::processMedia(const QUrl &input)
    //Call processFile in another thread
    QtConcurrent::run(this, &FileProcessor::processFile, input.toLocalFile());
    =>void FileProcessor::processFile(const QString & input)
        decodeFile(input);
        =>void FileProcessor::decodeFile(const QString &input)
        /* open the input file */
        avformat_open_input(&formatCtx, input.toStdString().c_str(), NULL, NULL);
        /* find the video stream information */
        ret = av_find_best_stream(inputCtx.data(), AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
        m_decoder->init(codecParameters));
        m_decoder->open();
        //Decoding loop
        while (processing() && ret >= 0) 
        if ((ret = av_read_frame(inputCtx.data(), &packet)) < 0)
        ret = m_decoder->decode(&packet);
        // hwdecoder.cpp
        =>int HWDecoder::decode(AVPacket *packet)
            int ret = avcodec_send_packet(m_decoderCtx, packet);
            ret = avcodec_receive_frame(m_decoderCtx, frame.data());
            VideoFrame* videoFrame;
            videoFrame = createHWVideoFrame(frame.data());
            sendFrame(videoFrame);
            => void HWDecoder::sendFrame(VideoFrame *frame)
                // connect(m_decoder, &HWDecoder::frameDecoded, m_source, &VideoSource::setFrame);
                Q_EMIT frameDecoded(sharedFrame);
                => void VideoSource::setFrame(VideoFramePtr frame)
                    Q_EMIT frameReady(frame);
                    =>connect(m_source, &VideoSource::frameReady, this, &VideoFBORenderer::onFrameReady);
                    // videorenderer.cpp
                    => void VideoFBORenderer::onFrameReady(VideoFramePtr frame)
                        m_frame = frame;
                        update();
3, render    
QQuickFramebufferObject::Renderer *VideoFBORenderer::createRenderer() const
    return new VideoRenderer();
    =>class VideoRenderer : public QQuickFramebufferObject::Renderer
    VideoRenderer()
        m_frameRenderer.initialize();
    virtual void synchronize(QQuickFramebufferObject * renderer) override 
        VideoFBORenderer* fboRenderer = (VideoFBORenderer*)renderer;
        m_frameRenderer.setFrame(fboRenderer->frame());
            =>VideoFramePtr VideoFBORenderer::frame() const
                return m_frame;
    void render() 
        m_frameRenderer.render();
        // framerenderer.cpp
        => void FrameRenderer::render() 
            glDepthMask(GL_TRUE);

            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_frame->map(m_texture);
            m_shaderProgram.setUniformValue("frameTexture", m_texture);
            m_frame->unmap();
            glFlush();
