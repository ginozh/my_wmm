#ifndef _GLHiddenWidget_h
#define _GLHiddenWidget_h
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QGLFunctions>
#include <QOpenGLFunctions>
#include <QMap>
#include <QVector>
#include <QImage>
#include <memory>
#include <QVariant>
#include <QMutex>
#include <memory>
//#include "GLBaseWidget.h"
#include "MMComm.h"
class GLDisplayWidget;
enum GPU_TYPE{
    GPUT_NOINIT = 0,
    GPUT_UNKOWN,
    GPUT_INTEL,
    GPUT_NVIDIA,
    GPUT_AMD,
};

//text start
#ifdef __cplusplus
extern "C" {
#endif
#include "vector.h"
#include "texture-atlas.h"
#include "mat4.h"
#include "texture-font.h"
#include "markup.h"

#ifdef __cplusplus
}
#endif
using namespace ftgl;
/**
 * Maximum number of attributes per vertex
 *
 * @private
 */
#define MAX_VERTEX_ATTRIBUTE 16


/**
 *  Generic vertex attribute.
 */
typedef struct vertex_attribute_t
{
    /**
     *  atribute name
     */
    GLchar * name;

    /**
     * index of the generic vertex attribute to be modified.
     */
    GLuint index;

    /**
     * Number of components per generic vertex attribute.
     *
     * Must be 1, 2, 3, or 4. The initial value is 4.
     */
    GLint size;

    /**
     *  data type of each component in the array.
     *
     *  Symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
     *  GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, or GL_DOUBLE are
     *  accepted. The initial value is GL_FLOAT.
     */
    GLenum type;

    /**
     *  whether fixed-point data values should be normalized (GL_TRUE) or
     *  converted directly as fixed-point values (GL_FALSE) when they are
     *  accessed.
     */
    GLboolean normalized;

    /**
     *  byte offset between consecutive generic vertex attributes.
     *
     *  If stride is 0, the generic vertex attributes are understood to be
     *  tightly packed in the array. The initial value is 0.
     */
    GLsizei stride;

    /**
     *  pointer to the first component of the first attribute element in the
     *  array.
     */
    GLvoid * pointer;

    /**
     * pointer to the function that enable this attribute.
     */
    void ( * enable )(void *);

} vertex_attribute_t;
/**
 * Generic vertex buffer.
 */
typedef struct vertex_buffer_t
{
    /** Format of the vertex buffer. */
    char * format;

    /** Vector of vertices. */
    vector_t * vertices;

#ifdef FREETYPE_GL_USE_VAO
    /** GL identity of the Vertex Array Object */
    GLuint VAO_id;
#endif

    /** GL identity of the vertices buffer. */
    GLuint vertices_id;

    /** Vector of indices. */
    vector_t * indices;

    /** GL identity of the indices buffer. */
    GLuint indices_id;

    /** Current size of the vertices buffer in GPU */
    size_t GPU_vsize;

    /** Current size of the indices buffer in GPU*/
    size_t GPU_isize;

    /** GL primitives to render. */
    GLenum mode;

    /** Whether the vertex buffer needs to be uploaded to GPU memory. */
    char state;

    /** Individual items */
    vector_t * items;

    /** Array of attributes. */
    vertex_attribute_t *attributes[MAX_VERTEX_ATTRIBUTE];
} vertex_buffer_t;
/**
 * Use LCD filtering
 */
#define LCD_FILTERING_ON    3

/**
 * Do not use LCD filtering
 */
#define LCD_FILTERING_OFF 1
/**
 * Text buffer structure
 */
typedef struct  text_buffer_t {
    /**
     * Vertex buffer
     */
    vertex_buffer_t *buffer;

    /**
     * Base color for text
     */
    vec4 base_color;


    /**
     * Pen origin
     */
    vec2 origin;

    /**
     * Last pen y location
     */
    float last_pen_y;

    /**
     * Total bounds
     */
    vec4 bounds;

    /**
     * Index (in the vertex buffer) of the current line start
     */
    size_t line_start;

    /**
     * Location of the start of the line
     */
    float line_left;

    /**
     * Vector of line information
     */
    vector_t * lines;

    /**
     * Current line ascender
     */
    float line_ascender;

    /**
     * Current line decender
     */
    float line_descender;
} text_buffer_t;


/**
 * Glyph vertex structure
 */
typedef struct glyph_vertex_t {
    /**
     * Vertex x coordinates
     */
    float x;

    /**
     * Vertex y coordinates
     */
    float y;

    /**
     * Vertex z coordinates
     */
    float z;

    /**
     * Texture first coordinate
     */
    float u;

    /**
     * Texture second coordinate
     */
    float v;

    /**
     * Color red component
     */
    float r;

    /**
     * Color green component
     */
    float g;

    /**
     * Color blue component
     */
    float b;

    /**
     * Color alpha component
     */
    float a;

    /**
     * Shift along x
     */
    float shift;

    /**
     * Color gamma correction
     */
    float gamma;

} glyph_vertex_t;


/**
 * Line structure
 */
typedef struct line_info_t {
    /**
     * Index (in the vertex buffer) where this line starts
     */
    size_t line_start;

    /**
     * bounds of this line
     */
    vec4 bounds;

} line_info_t;

/**
 * Align enumeration
 */
typedef enum Align
{
    /**
     * Align text to the left hand side
     */
    ALIGN_LEFT,

    /**
     * Align text to the center
     */
    ALIGN_CENTER,

    /**
     * Align text to the right hand side
     */
    ALIGN_RIGHT
} Align;
//text end

class GLHiddenWidget : public QGLWidget, protected QGLFunctions
//class GLHiddenWidget : public QOpenGLWidget, protected QOpenGLFunctions
//class GLHiddenWidget : public GLBaseWidget
{
	Q_OBJECT
public:
	GLHiddenWidget(QGLFormat format, QWidget *parent=0);
	~GLHiddenWidget();
protected:
	//virtual void glInit();
	//virtual void glDraw();

	virtual void initializeGL(); //setContext()之后;onece; paintGL() or resizeGL()之前; 
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

    //void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *);

public:
    void createBindFramebufferTexture();
    void initializeGLProgram();
    STFboInfo* getFboInfo(int idx);
    void resetFboUsed(int idx);
    GLuint GetTexture(int idx);
    //void clearFboBuffer();
    virtual int fragRenderForOtherThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2
        , bool useFbo/*=true*/, bool oneFrameFirstgl=false
        );
    virtual int fragRenderForOtherThreadAgain(const QString& effectname, const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2
        , bool useFbo, bool oneFrameFirstgl=false
        );
    void setContextThread(const QThread* thread){m_contextThread=thread;}
    const QThread* getContextThread(){return m_contextThread;}
    void setGLSize(int width, int height);
    void initialForBackThread();



    static int getGPUInfo();
    void initializeGLResource(int width, int height,bool bForce=false); //必须在主线程中初始化
    GLint createAndSetupTexture();
    virtual int baseFragRenderForAllThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2
        , STFboInfo* finalFboinfo /*, bool useFbo=true*/, bool oneFrameFirstgl=false
        );
    virtual int baseFragRenderForAllThreadAgain(const QString& effectname, const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2
        , STFboInfo* finalFboinfo /*,bool useFbo=true*/, bool oneFrameFirstgl=false
        );
    int LoadUniformAllTexture(const QString& effectname, const STFragmentInfo& fragInfo
        , GLuint texture , GLuint texture2);
    int UniformAllParament(const QString& effectname, const STFragmentInfo& fragInfo);
    int ParseConfCreateProgram(const QString pathpre, const QString& effectname);
    int RMEffectProgram(const QString& effectname);
    int RMEffectRef(const QString& effectname);
    int createProgram(QString effectname);
    void clearOpenglColor();
    void saveOpenglBuffer(int requested_frame=0);
    STFragmentInfo getFragmentInfo(const QString& effectname);
    void releaseResource();
    GLint load2DTextureInOut(int w, int h,const unsigned char *pixels);
    void deleteTextureOut(GLuint texture);
    virtual int makeCurrentOut();
    virtual int doneCurrentOut();
    void setGLDisplayWidget(GLDisplayWidget* glw);
private:
    ////int fragRenderForFbo(STFboInfo* finalFboinfo);
    STFboInfo* getNewFboInfo();
    STFboInfo* getRealNewFboInfo();
    void expandFramebuffer(int glw, int glh);
    GLuint blackTexture();


    GLuint buildShader(const GLchar *shader_source, GLenum type);
public:
    QMutex m_mutexRender;
    int glw=0;
    int glh=0;
private:
    const QThread* m_contextThread=NULL;
    QVector<STFboInfo*> vFboInfo; 
    int idxFrameBuf;
    int maxFrameBuf;
    bool m_bCreateFbo=false;


    std::shared_ptr<QImage> m_blackImage;
    int m_dst_w=10;
    int m_dst_h=10;
    GLuint m_blackTexture=0;



    QByteArray fragVertex;
    GLuint textureId[MAX_TEXTURES_CNT];
    int textureCnt=0;
    QByteArray arrTexturesVar[MAX_TEXTURES_CNT];

    GLDisplayWidget* m_glwidget=NULL;

    //QMutex m_mutexRender;
    QMutex m_mutexTexture;
    QVector<GLuint> vNeedDelTexture;
    static int m_gputype;

//text start
public:
/**
 * Create an attribute from the given parameters.
 *
 * @param size       number of component
 * @param type       data type
 * @param normalized Whether fixed-point data values should be normalized
                     (GL_TRUE) or converted directly as fixed-point values
                     (GL_FALSE) when they are  accessed.
 * @param stride     byte offset between consecutive attributes.
 * @param pointer    pointer to the first component of the first attribute
 *                   element in the array.
 * @return           a new initialized vertex attribute.
 *
 * @private
 */
vertex_attribute_t *
vertex_attribute_new( GLchar * name,
                      GLint size,
                      GLenum type,
                      GLboolean normalized,
                      GLsizei stride,
                      GLvoid *pointer );
/**
 * Delete a vertex attribute.
 *
 * @param  self a vertex attribute
 *
 */
void
vertex_attribute_delete( vertex_attribute_t * self );


/**
 * Create an attribute from the given description.
 *
 * @param  format Format string specifies the format of a vertex attribute.
 * @return        an initialized vertex attribute
 *
 * @private
 */
  vertex_attribute_t *
  vertex_attribute_parse( char *format );

/**
 * Enable a vertex attribute.
 *
 * @param attr  a vertex attribute
 *
 * @private
 */
  void
  vertex_attribute_enable( vertex_attribute_t *attr );

/**
 * Creates an empty vertex buffer.
 *
 * @param  format a string describing vertex format.
 * @return        an empty vertex buffer.
 */
  vertex_buffer_t *
  vertex_buffer_new( const char *format );


/**
 * Deletes vertex buffer and releases GPU memory.
 *
 * @param  self  a vertex buffer
 */
  void
  vertex_buffer_delete( vertex_buffer_t * self );


/**
 *  Returns the number of items in the vertex buffer
 *
 *  @param  self  a vertex buffer
 *  @return       number of items
 */
  size_t
  vertex_buffer_size( const vertex_buffer_t *self );


/**
 *  Returns vertex format
 *
 *  @param  self  a vertex buffer
 *  @return       vertex format
 */
  const char *
  vertex_buffer_format( const vertex_buffer_t *self );


/**
 * Print information about a vertex buffer
 *
 * @param  self  a vertex buffer
 */
  void
  vertex_buffer_print( vertex_buffer_t * self );


/**
 * Prepare vertex buffer for render.
 *
 * @param  self  a vertex buffer
 * @param  mode  render mode
 */
  void
  vertex_buffer_render_setup ( vertex_buffer_t *self,
                               GLenum mode );


/**
 * Finish rendering by setting back modified states
 *
 * @param  self  a vertex buffer
 */
  void
  vertex_buffer_render_finish ( vertex_buffer_t *self );


/**
 * Render vertex buffer.
 *
 * @param  self  a vertex buffer
 * @param  mode  render mode
 */
  void
  vertex_buffer_render ( vertex_buffer_t *self,
                         GLenum mode );


/**
 * Render a specified item from the vertex buffer.
 *
 * @param  self   a vertex buffer
 * @param  index index of the item to be rendered
 */
  void
  vertex_buffer_render_item ( vertex_buffer_t *self,
                              size_t index );


/**
 * Upload buffer to GPU memory.
 *
 * @param  self  a vertex buffer
 */
  void
  vertex_buffer_upload( vertex_buffer_t *self );


/**
 * Clear all items.
 *
 * @param  self  a vertex buffer
 */
  void
  vertex_buffer_clear( vertex_buffer_t *self );


/**
 * Appends indices at the end of the buffer.
 *
 * @param  self     a vertex buffer
 * @param  indices  indices to be appended
 * @param  icount   number of indices to be appended
 *
 * @private
 */
  void
  vertex_buffer_push_back_indices ( vertex_buffer_t *self,
                                    const GLuint * indices,
                                    const size_t icount );


/**
 * Appends vertices at the end of the buffer.
 *
 * @note Internal use
 *
 * @param  self     a vertex buffer
 * @param  vertices vertices to be appended
 * @param  vcount   number of vertices to be appended
 *
 * @private
 */
  void
  vertex_buffer_push_back_vertices ( vertex_buffer_t *self,
                                     const void * vertices,
                                     const size_t vcount );


/**
 * Insert indices in the buffer.
 *
 * @param  self    a vertex buffer
 * @param  index   location before which to insert indices
 * @param  indices indices to be appended
 * @param  icount  number of indices to be appended
 *
 * @private
 */
  void
  vertex_buffer_insert_indices ( vertex_buffer_t *self,
                                 const size_t index,
                                 const GLuint *indices,
                                 const size_t icount );


/**
 * Insert vertices in the buffer.
 *
 * @param  self     a vertex buffer
 * @param  index    location before which to insert vertices
 * @param  vertices vertices to be appended
 * @param  vcount   number of vertices to be appended
 *
 * @private
 */
  void
  vertex_buffer_insert_vertices ( vertex_buffer_t *self,
                                  const size_t index,
                                  const void *vertices,
                                  const size_t vcount );

/**
 * Erase indices in the buffer.
 *
 * @param  self   a vertex buffer
 * @param  first  the index of the first index to be erased
 * @param  last   the index of the last index to be erased
 *
 * @private
 */
  void
  vertex_buffer_erase_indices ( vertex_buffer_t *self,
                                const size_t first,
                                const size_t last );

/**
 * Erase vertices in the buffer.
 *
 * @param  self   a vertex buffer
 * @param  first  the index of the first vertex to be erased
 * @param  last   the index of the last vertex to be erased
 *
 * @private
 */
  void
  vertex_buffer_erase_vertices ( vertex_buffer_t *self,
                                 const size_t first,
                                 const size_t last );


/**
 * Append a new item to the collection.
 *
 * @param  self   a vertex buffer
 * @param  vcount   number of vertices
 * @param  vertices raw vertices data
 * @param  icount   number of indices
 * @param  indices  raw indices data
 */
  size_t
  vertex_buffer_push_back( vertex_buffer_t * self,
                           const void * vertices, const size_t vcount,
                           const GLuint * indices, const size_t icount );


/**
 * Insert a new item into the vertex buffer.
 *
 * @param  self      a vertex buffer
 * @param  index     location before which to insert item
 * @param  vertices  raw vertices data
 * @param  vcount    number of vertices
 * @param  indices   raw indices data
 * @param  icount    number of indices
 */
  size_t
  vertex_buffer_insert( vertex_buffer_t * self,
                        const size_t index,
                        const void * vertices, const size_t vcount,
                        const GLuint * indices, const size_t icount );

/**
 * Erase an item from the vertex buffer.
 *
 * @param  self     a vertex buffer
 * @param  index    index of the item to be deleted
 */
  void
  vertex_buffer_erase( vertex_buffer_t * self,
                       const size_t index );
  //textreader

  GLuint shader;
  texture_atlas_t *atlas=NULL;
  vertex_buffer_t *buffer=NULL;
  mat4   model, view, projection;
  void init( void );
  void add_text( vertex_buffer_t * buffer, texture_font_t * font,
          const char * text, vec4 * color, vec2 * pen );
  GLuint
      shader_load( const char * vert_filename,
              const char * frag_filename );
  char *
      shader_read( const char *filename );
  GLuint
      shader_compile( const char* source,
              const GLenum type );
  void display( );
  /**
   * Creates a new empty text buffer.
   *
   * @return  a new empty text buffer.
   *
   */
  text_buffer_t *
      text_buffer_new( );
  /**
   * Deletes texture buffer and its associated vertex buffer.
   *
   * @param  self  texture buffer to delete
   *
   */
  void
      text_buffer_delete( text_buffer_t * self );


  /**
   * Print some text to the text buffer
   *
   * @param self a text buffer
   * @param pen  position of text start
   * @param ...  a series of markup_t *, char * ended by NULL
   *
   */
  void
      text_buffer_printf( text_buffer_t * self, vec2 * pen, ... );


  /**
   * Add some text to the text buffer
   *
   * @param self   a text buffer
   * @param pen    position of text start
   * @param markup Markup to be used to add text
   * @param text   Text to be added
   * @param length Length of text to be added
   */
  void
      text_buffer_add_text( text_buffer_t * self,
              vec2 * pen, markup_t * markup,
              const char * text, size_t length );

  /**
   * Add a char to the text buffer
   *
   * @param self     a text buffer
   * @param pen      position of text start
   * @param markup   markup to be used to add text
   * @param current  charactr to be added
   * @param previous previous character (if any)
   */
  void
      text_buffer_add_char( text_buffer_t * self,
              vec2 * pen, markup_t * markup,
              const char * current, const char * previous );

  /**
   * Align all the lines of text already added to the buffer
   * This alignment will be relative to the overall bounds of the
   * text which can be queried by text_buffer_get_bounds
   *
   * @param self      a text buffer
   * @param pen       pen used in last call (must be unmodified)
   * @param alignment desired alignment of text
   */
  void
      text_buffer_align( text_buffer_t * self, vec2 * pen,
              enum Align alignment );

  /**
   * Get the rectangle surrounding the text
   *
   * @param self      a text buffer
   * @param pen       pen used in last call (must be unmodified)
   */
  vec4
      text_buffer_get_bounds( text_buffer_t * self, vec2 * pen );

  /**
   * Clear text buffer
   *
   * @param self a text buffer
   */
  void
      text_buffer_clear( text_buffer_t * self );
//text end
};

#endif // _GLHiddenWidget_h
