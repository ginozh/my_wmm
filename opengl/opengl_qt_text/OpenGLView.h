#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include <QVector3D>
#include <QGraphicsView>
#include <QOpenGLFunctions>
#include <QGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
//#include "Camera.h"
//#include "Format3DS.h"
#include "GraphicsScene.h"
#include "GLDisplayWidget.h"

class OpenGLView: public QGraphicsView,
        //protected QOpenGLFunctions
        protected QGLFunctions
{
    Q_OBJECT
public:
    OpenGLView( QWidget* pParent = 0 );
    virtual ~OpenGLView( void );
    void setScene( GraphicsScene* pScene );
    void setGlWidget(GLDisplayWidget*);
public slots:
    bool SwitchShader( const QString& shaderFileName );
    void SetLightPos( const QVector3D& lightPos = QVector3D( ) );
protected:
    void resizeEvent( QResizeEvent* pEvent );
    void mousePressEvent( QMouseEvent* pEvent );
    void mouseReleaseEvent( QMouseEvent* pEvent );
    void mouseMoveEvent( QMouseEvent* pEvent );
    void wheelEvent( QWheelEvent* pEvent );
    void drawBackground( QPainter* pPainter, const QRectF& rect );
private:
    void InitGL( void );
    void ResizeGL( int width, int height );
    void PaintGL( void );

    void DrawAxis( void );

    bool SetupShaders( void );

    //Camera          m_Camera;
    //Format3DS       m_3DS;

    // 着色器
    QOpenGLShader*  m_pVertexShader;
    QOpenGLShaderProgram* m_pShaderProgram;

    GLDisplayWidget* m_displayWidget=NULL;
};

#endif // OPENGLVIEW_H
