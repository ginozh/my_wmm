#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTime>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStringList>
#include <QDesktopWidget>
#include <QTimer>
#include <QFontComboBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include "mainwindow.h"
#include "MMComm.h"
#include "OpenGLView.h"
#include "GraphicsScene.h"
#if 0
extern "C" {
#include "texture-atlas.h"
}
using namespace ftgl;
texture_atlas_t *atlas;
vertex_buffer_t *buffer;
#endif
extern "C" {
#include "ass_types.h"
#include "ass_library.h"
#include "ass_fontselect.h"
#include "ass_font.h"
#include "ass_cache.h"
}


#include "PlayerPrivate.h"
PlayerPrivate* playerPrivate=NULL;

void msg_callback(int level, const char *fmt, va_list va, void *data)
{
    if (level > 6)
        return;
    printf("libass: ");
    vprintf(fmt, va);
    printf("\n");
}

//#define SCALEFLAGS SWS_FAST_BILINEAR
#define SCALEFLAGS SWS_BILINEAR
//#define EXPORT
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Movie Maker"));
	QGLFormat qglFormat;
	qglFormat.setProfile(QGLFormat::CoreProfile);
	qglFormat.setSampleBuffers(true);
	qglFormat.setSamples(4);
    qglFormat.setStencil(true);
    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* lbl=NULL;
    //media player
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            hbox->addLayout(vbox);
            {
                lbl = new QLabel(tr("openglPlayer: "));
                vbox->addWidget(lbl);

                {
                    m_hiddenWidget=new GLHiddenWidget(qglFormat, this);
                    //playerWidget = new PlayerWidget(iw, ih, this);
                    playerWidget = new PlayerWidget(qglFormat, m_hiddenWidget, this, false);
                    qDebug()<<"playerWidget: "<<playerWidget<<" higlwidget: "<<playerWidget->m_shareWidget;
                    /////vbox->addWidget(playerWidget);
                    playerWidget->setFixedSize(iw,ih);
#if 0
                    QLineEdit* lineedit=new QLineEdit(playerWidget);
                    lineedit->setAttribute(Qt::WA_TranslucentBackground);
                    lineedit->setWindowOpacity(0.5);
#endif
#if 1
                    playerWidget->makeCurrent( );
                    //QGLWidget* pWidget = new QGLWidget( QGLFormat( QGL::SampleBuffers ), this );
                    //pWidget->makeCurrent( );
                    GraphicsScene* pScene = new GraphicsScene( this );
                    //QGraphicsView* pView = new QGraphicsView(this);
                    OpenGLView* pView = new OpenGLView(this);
                    pView->setViewport( playerWidget );
                    //pView->setViewport( pWidget );
                    pView->setGlWidget(playerWidget);
                    pView->setFixedSize(iw,ih);
                    pView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
                    pView->setScene( pScene );
                    playerWidget->doneCurrent( );
                    vbox->addWidget(pView);
#endif

                    //playerWidget->resize(iw,ih);
                    //playerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#if 0
                    m_playState=Idle;
                    player = new Player();
                    connect(player, SIGNAL(sigPlayStateChanged(PlayState)), 
                            this, SLOT(playStateChanged(PlayState)));
                    connect(player, SIGNAL(sigPositionChanged(qint64)), this, SLOT(positionChanged(qint64)));
                    connect(player, SIGNAL(sigDurationChanged(qint64)), this, SLOT(durationChanged(qint64)));
#endif
                }
            }
        }
    }
    //timeline fps
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            tmpfcb = new QFontComboBox();
            hbox->addWidget(tmpfcb);
            connect(tmpfcb, &QFontComboBox::currentFontChanged,
                    [=](const QFont &font)
                    {
                    QFontInfo fontInfo(font);
                    qDebug()<<"QFontComboBox::currentFontChanged font: "<<font.toString()
                    <<" family: "<<font.family()
                    <<" rawName: "<<font.rawName()
                    <<" defaultFamily: "<<font.defaultFamily()
                    <<" key: "<<font.key()
                    <<" lastResortFamily: "<<font.lastResortFamily()
                    <<" styleName: "<<font.styleName()
                    <<" substitutions: "<<font.substitutions().size()
                    <<" substitutes: "<<QFont::substitutes("KaiTi").size()
                    <<" family: "<<fontInfo.family()
                    <<" styleName: "<<fontInfo.styleName()
                    ;
#if 1
                    QFontDatabase database;
                    foreach (const QString &family, database.families()) {
                        //qDebug()<<"afamily: "<<family;
                    }
                    QFont f=database.font(font.family(), "", 9);
                    qDebug()<<"f family: "<<f.family();
#endif

                    //2,根据字体中文名字获取families(例如Arial) --libass 可能中文就可以了
                    //再确定系统families,本机(Microsoft JhengHei)
                    // 遍历所有文字信息,根据得分值获取最佳family
                    //clip, textreader
                    //textreader.init();
                    //1, init
                    ASS_Library *ass_library;
                    ass_library = ass_library_init();
                    if (!ass_library) {
                        printf("ass_library_init failed!\n");
                        exit(1);
                    }
                    ass_set_message_cb(ass_library, msg_callback, NULL);
                    FT_Library ft;
                    {
                        int error;
                        int vmajor, vminor, vpatch;
                        error = FT_Init_FreeType(&ft);
                        if (error) {
                            ///ass_msg(library, MSGL_FATAL, "%s failed", "FT_Init_FreeType");
                        }
                        FT_Library_Version(ft, &vmajor, &vminor, &vpatch);
                        ////ass_msg(library, MSGL_V, "Raster: FreeType %d.%d.%d", vmajor, vminor, vpatch);
                    }
                    ASS_FontSelector *fontselect;
                    //ass_set_fonts(ass_renderer, NULL, "sans-serif",    ///directwrite
                    //        ASS_FONTPROVIDER_AUTODETECT, NULL, 1);
                    const char *default_font=NULL;
                    //const char *default_family="sans-serif";
                    const char* default_family=font.family().toUtf8().data();
                    int dfp=ASS_FONTPROVIDER_AUTODETECT;
                    const char *config=NULL;
                    fontselect = ass_fontselect_init(ass_library, ft,
                            default_family, default_font, config, (ASS_DefaultFontProvider)dfp);

                    //2, font
                    //ass_parse.c:update_font
                    Cache *font_cache=ass_font_cache_create();;
                    //char family[]="Brush Script MT";
                    //char family[]="STXihei";
                    //char family[]="华文细黑";//symbol:  27979  face_index:  0  glyph_index:  10563
                    //iFace:  0  family_name:  Yu Gothic; iFace:  1  family_name:  Microsoft JhengHei UI
                    //char family[]="游ゴシック";//symbol:  27979  face_index:  1  glyph_index:  15369
                    char* family=font.family().toUtf8().data();
                    qDebug()<<"orignal family: "<<family;
                    ASS_Font *ass_font=NULL;
                    double font_size=20.0;
                    {
                        unsigned val;
                        ASS_FontDesc desc;
                        desc.vertical = 0;
                        desc.family = strdup(family);
                        val = 0;
                        // 0 = normal, 1 = bold, >1 = exact weight
                        if (val == 1 || val == -1)
                            val = 700;               // bold
                        else if (val <= 0)
                            val = 400;               // normal
                        desc.bold = val;

                        val = 0;
                        if (val == 1)
                            val = 100;              // italic
                        else if (val <= 0)
                            val = 0;                // normal
                        desc.italic = val;
                        ass_cache_dec_ref(ass_font);
                        ass_font = ass_font_new(font_cache, ass_library,
                                    ft, fontselect, &desc);
                    }
                    int face_index;
                    int glyph_index;

                    //ass_font_get_index(render_priv->fontselect, info->font,
                    //        info->symbol, &info->face_index, &info->glyph_index);//GlyphInfo *info
                    uint32_t symbol;FT_Glyph glyph;
                    symbol=27979;
                    face_index=0;
                    glyph_index=0;
                    ass_font_get_index(fontselect, ass_font, symbol, &face_index, &glyph_index);
                    //ass_face_set_size(info->font->faces[info->face_index], info->font_size);
                    //render_priv->text_info->glyphs[0]->font->faces[0]->size->metrics何时赋值? 
                    //  ass_render.c:ass_shaper_shape->shape_harfbuzz->get_hb_font
                    //
                    ass_face_set_size(ass_font->faces[face_index], 20.0);
                    //ass_font_set_transform(info->font, info->scale_x,
                    //        info->scale_y, NULL);
                    glyph = ass_font_get_glyph(ass_font,
                                symbol, face_index, glyph_index,
                                ASS_HINTING_NONE, 0);
                    //1, 15369
                    qDebug()<<"symbol: "<<symbol<<" face_index: "<<face_index
                        <<" glyph_index: "<<glyph_index<<" glyph_advance x: "<<glyph->advance.x
                        <<" y: "<<glyph->advance.y;
                    symbol=35797;
                    face_index=0;
                    glyph_index=0;
                    ass_font_get_index(fontselect, ass_font, symbol, &face_index, &glyph_index);
                    ass_face_set_size(ass_font->faces[face_index], 256.0);
                    glyph = ass_font_get_glyph(ass_font,
                                symbol, face_index, glyph_index,
                                ASS_HINTING_NONE, 0);
                    //1, 23187
                    qDebug()<<"symbol: "<<symbol<<" face_index: "<<face_index
                        <<" glyph_index: "<<glyph_index<<" glyph_advance x: "<<glyph->advance.x
                        <<" y: "<<glyph->advance.y;
                    symbol=66;
                    face_index=0;
                    glyph_index=0;
                    ass_font_get_index(fontselect, ass_font, symbol, &face_index, &glyph_index);
                    ass_face_set_size(ass_font->faces[face_index], 256.0);
                    glyph = ass_font_get_glyph(ass_font,
                                symbol, face_index, glyph_index,
                                ASS_HINTING_NONE, 0);
                    //0, 37->1, 37
                    qDebug()<<"symbol: "<<symbol<<" face_index: "<<face_index
                        <<" glyph_index: "<<glyph_index<<" glyph_advance x: "<<glyph->advance.x
                        <<" y: "<<glyph->advance.y;
                    for(int iFace=0; iFace<ass_font->n_faces; iFace++)
                    {
                        qDebug()<<"iFace: "<<iFace<<" family_name: "<<ass_font->faces[iFace]->family_name;
                        family=ass_font->faces[iFace]->family_name;
                        //break;
                    }
                    ////FT_Glyph glyph = ass_font_get_glyph(info->font, info->symbol, info->face_index, info->glyph_index, priv->settings.hinting, info->flags);//GlyphInfo *info


                    playerWidget->makeCurrentOut();
                    m_hiddenWidget->init(family);
                    playerWidget->doneCurrentOut();
                    playerPrivate->ready=1;

                    //clean tmp?
                    //ass_render.c::ass_renderer_done
                    ass_cache_done(font_cache);
                    ass_fontselect_free(fontselect);
                    FT_Done_FreeType(ft);
                    //test.c::main
                    ass_library_done(ass_library);

                    });
        }
        {
            QWidget* nullWidget = new QWidget;
            nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
            hbox->addWidget(nullWidget);
        }
    }
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            QWidget* nullWidget = new QWidget;
            nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
            hbox->addWidget(nullWidget);
        }
    }
    centralWidget->setLayout(mainLayout);
}
void MainWindow::initialOpengl()
{
    qDebug()<<"initialOpengl.";
    if(!firstShow)
    {
        return;
    }
    firstShow=false;
    playerWidget->setGLSize(iw, ih);
    playerPrivate = new PlayerPrivate();
    playerPrivate->setGLWidget(playerWidget);
    playerPrivate->startPlayback();
#if 0
#ifndef EXPORT
    t_resource = new Timeline(playerWidget, iw, ih, Fraction(25,1), 44100, 2, LAYOUT_STEREO);

    //t = new Timeline(playerWidget, iw, ih, Fraction(30000,1001), 44100, 2, LAYOUT_STEREO);
    //t = new Timeline(player, iw, ih, Fraction(25,1), 44100, 2, LAYOUT_STEREO);
    //t = new Timeline(playerWidget, iw, ih, Fraction(25,1), 44100, 2, LAYOUT_STEREO);
    t = new Timeline(playerWidget, iw, ih, Fraction(20,1), 44100, 2, LAYOUT_STEREO);

    bool bexist=false;
    m_reader1 = QtImageReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader2 = QtImageReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader5 = SubtitleReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader8 = FFmpegReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader9 = FFmpegReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip1 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip2 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip5 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip8 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip9 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);

    player->setGLWidget(playerWidget);
    qDebug()<<"initialOpengl. open t";
    player->open(t);
    player->play();
    player->pause();
#endif
#ifdef EXPORT
    exporter->setGLWidget(exportWidget);
    exporter->start(); //must
    const QRect geo = QApplication::desktop()->availableGeometry(this);
    qDebug()<<"MainWindow::showEvent width: "<<geo.width()<<" height: "<<geo.height();

    if(exportWidget)
    {
        exportWidget->move(geo.width()*2,geo.height()*2);
        //exportWidget->move(0,geo.height()/2);
    }
    //exportWidget->move(3*geo.width()/4,0);
#endif
#endif
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processFirstThing()));
    ////timer->setSingleShot(true);
    timer->start(2000);
}
void MainWindow::processFirstThing()
{
    ////return;
    qDebug()<<"MainWindow::processFirstThing";
    static int icnt=0;
    if(icnt++%2==0)
    {
        tmpfcb->setCurrentIndex(tmpfcb->count()-2);
    }
    else
    {
        tmpfcb->setCurrentIndex(tmpfcb->count()-3);
    }
#if 0
#ifndef EXPORT
    ///if(playerWidget) playerWidget->setFixedSize(iw,ih);
    //play media
    ////m_leMediaFile->setText("/c/shareproject/jpg/WeChatSight2266.mp4"); 
    m_leMediaFile->setText("/z/Desktop/upan/qqrtx/QQ/rotate_270.mp4"); 
    ////m_pbmedia->click();
    ////mediaplayButton->click();
    ////return;


#ifdef __UBUNTU__
    ////m_leClipFile1->setText("/z/Desktop/upan/qqrtx/QQ/white.jpg"); 
    m_leClipFile1->setText("/z/Desktop/upan/jpg/1_rotate_180.jpg"); 
#else
    m_leClipFile1->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\white.jpg"); 
    //m_leClipFile1->setText("c:\\shareproject\\Desktop\\upan\\jpg\\1_rotate.jpg"); 
#endif
    m_cbClipPicEffect1->setCurrentText("");
    m_cbClipEffect1->setCurrentText("");
    ////if(player) m_pbClipAddRm1->click();

#ifdef __UBUNTU__
    m_leClipFile2->setText("/z/Desktop/upan/qqrtx/QQ/black.jpg"); 
#else
    //m_leClipFile2->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\RGB_color_chart_by_ervis.jpg"); 
    m_leClipFile2->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\black.jpg"); 
    //m_leClipFile2->setText("2.jpg"); 
#endif
    m_cbClipPicEffect2->setCurrentText("");
    m_cbClipChromaTransparece2->setCurrentText("100");
    //m_cbClipPicEffect2->setCurrentText("Motion054 pandown");
    //m_cbClipEffect2->setCurrentText("");
    m_leClipOverlayPosX2->setText("340"); //w:300 h:200 (640, 360)
    m_leClipOverlayPosY2->setText("160"); 
    m_cbClipEffect2->setCurrentText("Motion055 pandownright");
    ////if(player) m_pbClipAddRm2->click();

#ifdef __UBUNTU__
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/SMDV-17.avi"); 
    m_leClipFile8->setText("/c/shareproject/jpg/Wildlife.wmv"); 
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/Spring_In_My_Step.mp3"); 
    //m_leClipFile8->setText("/c/shareproject/jpg/WeChatSight2266.mp4"); 
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/chaoduozishi.mp4"); 
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/2ndmingji.wmv"); 
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/Thousand.Planets.mp4"); 
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/rotate_270.mp4"); 
    ////m_leClipFile8->setText("/c/shareproject/ffmpeg-3.4.2/CityGroove.mp3"); 
    //m_leClipFile8->setText("/home/gino/.local/share/XMovieMaker/sample/video/Countdown1.mp4"); 
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/open_error.wmv"); 
#else
    ////m_leClipFile8->setText("Wildlife.wmv"); 
    //m_leClipFile8->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\Magic_Particles.mov"); 
    //m_leClipFile8->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\line.webm"); 
    //m_leClipFile8->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\Element_Love_1.webm"); 
    ///m_leClipFile8->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\0808-000\\1ef19d36d8a43c2e8b3bc3c4b76edaeb.mp4"); 
    m_leClipFile8->setText("c:\\shareproject\\Desktop\\upan\\jpg\\video\\2.mp4");
    //m_leClipFile8->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\Thousand.Planets.mp4");
#endif
    //m_cbClipDisableVideo8->setCheckState(Qt::Checked);
    //m_cbClipVolume8->setCurrentText("100");
    m_cbClipSpeed8->setCurrentText("1");
    m_leClipStart8->setText("100.0"); 
    //m_leFadeIn8->setText("2.0"); 
    ////m_leClipEnd8->setText("200.0"); 
    //m_leFadeOut8->setText("2.0"); 
    if(player) m_pbClipAddRm8->click();
    //return ;

#ifdef __UBUNTU__
    //m_leClipFile9->setText("/z/Desktop/upan/qqrtx/QQ/SMDV-17.avi"); 
    m_leClipFile9->setText("/c/shareproject/jpg/Wildlife.wmv"); 
#else
    //m_leClipFile9->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\Magic_Particles.mov"); 
    //m_leClipFile9->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\Magic_Particles5.mp4"); 
    //m_leClipFile9->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\5.webm"); 
    m_leClipFile9->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\line.webm"); 
    //m_leClipFile9->setText("c:\\shareproject\\Desktop\\upan\\qqrtx\\QQ\\0808-000\\Motohiko Hirami - Prelude (Motohiko Hirami & Yann).mp3"); 
    //m_cbClipDisableVideo9->setCheckState(Qt::Checked);
#endif
    //m_cbClipDisableVideo9->setCheckState(Qt::Checked);
    ///m_cbClipVolume9->setCurrentText("300");
    //m_leClipPos9->setText("2.0"); 
    m_leClipOverlayH9->setText("0"); 
    //m_reader9->setTransparence(30);
    ////m_cbClipChromaTransparece9->setCurrentText("100");
    ////m_cbClipChromaSimilarity9->setCurrentText("0.27");
    ////if(player) m_pbClipAddRm9->click();
#else
    m_leClipPos9->setText("2.0"); 
#endif
#ifdef EXPORT
    ///m_leClipEnd8->setText("2.0"); 
    //m_leClipFile8->setText("/c/shareproject/ffmpeg-3.4.2/CityGroove.mp3"); 
    //m_leClipFile8->setText("/z/Desktop/upan/qqrtx/QQ/Spring_In_My_Step.mp3"); 
    m_cbClipSpeed8->setCurrentText("1.3");
    m_pbExport->click();
    return;
#endif
    return;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processSecondThing()));
    timer->setSingleShot(true);
    timer->start(1000);
#endif
}

void MainWindow::processSecondThing()
{
#if 0
    qDebug()<<"MainWindow::processSecondThing";
    player->play();
#if 0
    this->close();
    QCoreApplication::exit();
#endif
#endif
}
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    initialOpengl(); //showevent时可能QOpenGLContext::currentContext()的值还为0，此时所有的makeCurrent都无效

#if 0
    if(!firstShow)
    {
        return;
    }
    firstShow=false;
    //MMGlobalContext::instance()->switchExportStatue(false);
    t = new Timeline(playerWidget, iw, ih, Fraction(25,1), 44100, 2, LAYOUT_STEREO);
#if 0
    bool bexist=false;
    m_reader1 = QtImageReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader2 = QtImageReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader5 = SubtitleReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader8 = FFmpegReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_reader9 = FFmpegReader::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip1 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip2 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip5 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip8 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
    m_clip9 = Clip::NewObject(ObjectKey(t, QString::number(id++)),bexist);
#endif
    player->setGLWidget(playerWidget);
    player->open(t);
    player->play();
    //player->pause();
#endif
#if 0
    exportWidget->makeCurrent();
    MMGlobalContext::instance()->initialOpengl(iw_export, ih_export);
    exportWidget->doneCurrent();
    MMGlobalContext::instance()->switchExportStatue(true);
#endif

#if 0
    MMGlobalContext::instance()->switchExportStatue(true);
    t_export = new Timeline(iw_export, ih_export, Fraction(25,1), 44100, 2, LAYOUT_STEREO);
    qDebug()<<"exportState: "<<m_exportState;
#if 1
    exporter->setGLWidget(exportWidget);
    //exporter->setDisplayWidget(exportPlayerWidget);
    exporter->start(); //must
#endif
    exporter->open(t_export);

#endif
#if 0
    exporter->setGLWidget(exportWidget);
    exporter->start(); //must
#endif
}
void MainWindow::closeEvent(QCloseEvent *event)
{
#if 0
    if(exporter)
        exporter->stopThread();
    if(player) player->stopThread();
    ////MMGlobalContext::instance()->releaseResource(); //uncomplete
#endif
    QMainWindow::closeEvent(event);
}
