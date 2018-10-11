#ifndef _GLHiddenWidgetText_cpp
#define _GLHiddenWidgetText_cpp
#include "GLHiddenWidget.h"
#include <assert.h>
#include "vec234.h"
#include "platform.h"
#include "utf8-utils.h"
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>
using namespace ftgl;
typedef struct {
    float x, y, z;    // position
    float s, t;       // texture
    float r, g, b, a; // color
} vertex_t;
#define SET_GLYPH_VERTEX(value,x0,y0,z0,s0,t0,r,g,b,a,sh,gm) { \
	glyph_vertex_t *gv=&value;                                 \
	gv->x=x0; gv->y=y0; gv->z=z0;                              \
	gv->u=s0; gv->v=t0;                                        \
	gv->r=r; gv->g=g; gv->b=b; gv->a=a;                        \
	gv->shift=sh; gv->gamma=gm;}

// ----------------------------------------------------------------------------
vertex_attribute_t *
GLHiddenWidget::vertex_attribute_new( GLchar * name,
                      GLint size,
                      GLenum type,
                      GLboolean normalized,
                      GLsizei stride,
                      GLvoid *pointer )
{
    vertex_attribute_t *attribute =
        (vertex_attribute_t *) malloc (sizeof(vertex_attribute_t));

    assert( size > 0 );

    attribute->name       = (GLchar *) strdup( name );
    attribute->index      = -1;
    attribute->size       = size;
    attribute->type       = type;
    attribute->normalized = normalized;
    attribute->stride     = stride;
    attribute->pointer    = pointer;
    return attribute;
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_attribute_delete( vertex_attribute_t * self )
{
    assert( self );

    free( self->name );
    free( self );
}



// ----------------------------------------------------------------------------
vertex_attribute_t *
GLHiddenWidget::vertex_attribute_parse( char *format )
{
    GLenum type = 0;
    int size;
    int normalized = 0;
    char ctype;
    char *name;
    vertex_attribute_t *attr;
    char *p = strchr(format, ':');
    if( p != NULL)
    {
        name = strndup(format, p-format);
        if( *(++p) == '\0' )
        {
            //fprintf( stderr, "No size specified for '%s' attribute\n", name );
            qDebug()<<"No size specified for "<<name<<" attribute";
            free( name );
            return 0;
        }
        size = *p - '0';

        if( *(++p) == '\0' )
        {
            //fprintf( stderr, "No format specified for '%s' attribute\n", name );
            qDebug()<<"No format specified for "<<name<<" attribute";
            free( name );
            return 0;
        }
        ctype = *p;

        if( *(++p) != '\0' )
        {
            if( *p == 'n' )
            {
                normalized = 1;
            }
        }

    }
    else
    {
        //fprintf(stderr, "Vertex attribute format not understood ('%s')\n", format );
        qDebug()<<"Vertex attribute format not understood "<<format;
        return 0;
    }

    switch( ctype )
    {
    case 'b': type = GL_BYTE;           break;
    case 'B': type = GL_UNSIGNED_BYTE;  break;
    case 's': type = GL_SHORT;          break;
    case 'S': type = GL_UNSIGNED_SHORT; break;
    case 'i': type = GL_INT;            break;
    case 'I': type = GL_UNSIGNED_INT;   break;
    case 'f': type = GL_FLOAT;          break;
    default:  type = 0;                 break;
    }


    attr = vertex_attribute_new( name, size, type, normalized, 0, 0 );
    free( name );
    return attr;
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_attribute_enable( vertex_attribute_t *attr )
{
    if( attr->index == -1 )
    {
        GLint program;
        glGetIntegerv( GL_CURRENT_PROGRAM, &program );
        if( program == 0)
        {
            return;
        }
        attr->index = glGetAttribLocation( program, attr->name );
        if( attr->index == -1 )
        {
            return;
        }
    }
    glEnableVertexAttribArray( attr->index );
    glVertexAttribPointer( attr->index, attr->size, attr->type,
                           attr->normalized, attr->stride, attr->pointer );
}
/**
 * Buffer status
 */
#define CLEAN  (0)
#define DIRTY  (1)
#define FROZEN (2)


// ----------------------------------------------------------------------------
vertex_buffer_t *
GLHiddenWidget::vertex_buffer_new( const char *format )
{
    size_t i, index = 0, stride = 0;
    const char *start = 0, *end = 0;
    GLchar *pointer = 0;

    vertex_buffer_t *self = (vertex_buffer_t *) malloc (sizeof(vertex_buffer_t));
    if( !self )
    {
        return NULL;
    }

    self->format = strdup( format );

    for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
    {
        self->attributes[i] = 0;
    }

    start = format;
    do
    {
        char *desc = 0;
        vertex_attribute_t *attribute;
        GLuint attribute_size = 0;
        end = (char *) (strchr(start+1, ','));

        if (end == NULL)
        {
            desc = strdup( start );
        }
        else
        {
            desc = strndup( start, end-start );
        }
        attribute = vertex_attribute_parse( desc );
        start = end+1;
        free(desc);
        attribute->pointer = pointer;

        switch( attribute->type )
        {
        case GL_BOOL:           attribute_size = sizeof(GLboolean); break;
        case GL_BYTE:           attribute_size = sizeof(GLbyte); break;
        case GL_UNSIGNED_BYTE:  attribute_size = sizeof(GLubyte); break;
        case GL_SHORT:          attribute_size = sizeof(GLshort); break;
        case GL_UNSIGNED_SHORT: attribute_size = sizeof(GLushort); break;
        case GL_INT:            attribute_size = sizeof(GLint); break;
        case GL_UNSIGNED_INT:   attribute_size = sizeof(GLuint); break;
        case GL_FLOAT:          attribute_size = sizeof(GLfloat); break;
        default:                attribute_size = 0;
        }
        stride  += attribute->size*attribute_size;
        pointer += attribute->size*attribute_size;
        self->attributes[index] = attribute;
        index++;
    } while ( end && (index < MAX_VERTEX_ATTRIBUTE) );

    for( i=0; i<index; ++i )
    {
        self->attributes[i]->stride = stride;
    }

#ifdef FREETYPE_GL_USE_VAO
    self->VAO_id = 0;
#endif

    self->vertices = vector_new( stride );
    self->vertices_id  = 0;
    self->GPU_vsize = 0;

    self->indices = vector_new( sizeof(GLuint) );
    self->indices_id  = 0;
    self->GPU_isize = 0;

    self->items = vector_new( sizeof(ivec4) );
    self->state = DIRTY;
    self->mode = GL_TRIANGLES;
    return self;
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_delete( vertex_buffer_t *self )
{
    size_t i;

    assert( self );

    for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
    {
        if( self->attributes[i] )
        {
            vertex_attribute_delete( self->attributes[i] );
        }
    }

#ifdef FREETYPE_GL_USE_VAO
    if( self->VAO_id )
    {
        glDeleteVertexArrays( 1, &self->VAO_id );
    }
    self->VAO_id = 0;
#endif

    vector_delete( self->vertices );
    self->vertices = 0;
    if( self->vertices_id )
    {
        glDeleteBuffers( 1, &self->vertices_id );
    }
    self->vertices_id = 0;

    vector_delete( self->indices );
    self->indices = 0;
    if( self->indices_id )
    {
        glDeleteBuffers( 1, &self->indices_id );
    }
    self->indices_id = 0;

    vector_delete( self->items );

    if( self->format )
    {
        free( self->format );
    }
    self->format = 0;
    self->state = 0;
    free( self );
}


// ----------------------------------------------------------------------------
const char *
GLHiddenWidget::vertex_buffer_format( const vertex_buffer_t *self )
{
    assert( self );

    return self->format;
}


// ----------------------------------------------------------------------------
size_t
GLHiddenWidget::vertex_buffer_size( const vertex_buffer_t *self )
{
    assert( self );

    return vector_size( self->items );
}


// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_print( vertex_buffer_t * self )
{
    int i = 0;
    static const char *gltypes[9] = {
        "GL_BOOL",
        "GL_BYTE",
        "GL_UNSIGNED_BYTE",
        "GL_SHORT",
        "GL_UNSIGNED_SHORT",
        "GL_INT",
        "GL_UNSIGNED_INT",
        "GL_FLOAT",
        "GL_VOID"
    };

    assert(self);

    //fprintf( stderr, "%ld vertices, %ld indices\n", vector_size( self->vertices ), vector_size( self->indices ) );
    qDebug()<<vector_size( self->vertices )<<" vertices, "<<vector_size( self->indices )<<" indices";
    while( self->attributes[i] )
    {
        int j = 8;
        switch( self->attributes[i]->type )
        {
        case GL_BOOL:           j=0; break;
        case GL_BYTE:           j=1; break;
        case GL_UNSIGNED_BYTE:  j=2; break;
        case GL_SHORT:          j=3; break;
        case GL_UNSIGNED_SHORT: j=4; break;
        case GL_INT:            j=5; break;
        case GL_UNSIGNED_INT:   j=6; break;
        case GL_FLOAT:          j=7; break;
        default:                j=8; break;
        }
        fprintf(stderr, "%s : %dx%s (+%p)\n",
                self->attributes[i]->name,
                self->attributes[i]->size,
                gltypes[j],
                self->attributes[i]->pointer);
        qDebug()<<self->attributes[i]->name<<" : "<<self->attributes[i]->size<<" x"<<gltypes[j]<<"(+"<<self->attributes[i]->pointer<<")";

        i += 1;
    }
}


// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_upload ( vertex_buffer_t *self )
{
    size_t vsize, isize;

    if( self->state == FROZEN )
    {
        return;
    }

    if( !self->vertices_id )
    {
        glGenBuffers( 1, &self->vertices_id );
    }
    if( !self->indices_id )
    {
        glGenBuffers( 1, &self->indices_id );
    }

    vsize = self->vertices->size*self->vertices->item_size;
    isize = self->indices->size*self->indices->item_size;


    // Always upload vertices first such that indices do not point to non
    // existing data (if we get interrupted in between for example).

    // Upload vertices
    glBindBuffer( GL_ARRAY_BUFFER, self->vertices_id );
    if( vsize != self->GPU_vsize )
    {
        glBufferData( GL_ARRAY_BUFFER,
                      vsize, self->vertices->items, GL_DYNAMIC_DRAW );
        self->GPU_vsize = vsize;
    }
    else
    {
        glBufferSubData( GL_ARRAY_BUFFER,
                         0, vsize, self->vertices->items );
    }
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // Upload indices
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, self->indices_id );
    if( isize != self->GPU_isize )
    {
        glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                      isize, self->indices->items, GL_DYNAMIC_DRAW );
        self->GPU_isize = isize;
    }
    else
    {
        glBufferSubData( GL_ELEMENT_ARRAY_BUFFER,
                         0, isize, self->indices->items );
    }
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_clear( vertex_buffer_t *self )
{
    assert( self );

    self->state = FROZEN;
    vector_clear( self->indices );
    vector_clear( self->vertices );
    vector_clear( self->items );
    self->state = DIRTY;
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_render_setup ( vertex_buffer_t *self, GLenum mode )
{
    size_t i;

#ifdef FREETYPE_GL_USE_VAO
    // Unbind so no existing VAO-state is overwritten,
    // (e.g. the GL_ELEMENT_ARRAY_BUFFER-binding).
    glBindVertexArray( 0 );
#endif

    if( self->state != CLEAN )
    {
        vertex_buffer_upload( self );
        self->state = CLEAN;
    }

#ifdef FREETYPE_GL_USE_VAO
    if( self->VAO_id == 0 )
    {
        // Generate and set up VAO

        glGenVertexArrays( 1, &self->VAO_id );
        glBindVertexArray( self->VAO_id );

        glBindBuffer( GL_ARRAY_BUFFER, self->vertices_id );

        for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
        {
            vertex_attribute_t *attribute = self->attributes[i];
            if( attribute == 0 )
            {
                continue;
            }
            else
            {
                vertex_attribute_enable( attribute );
            }
        }

        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        if( self->indices->size )
        {
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, self->indices_id );
        }
    }

    // Bind VAO for drawing
    glBindVertexArray( self->VAO_id );
#else

    glBindBuffer( GL_ARRAY_BUFFER, self->vertices_id );

    for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
    {
        vertex_attribute_t *attribute = self->attributes[i];
        if ( attribute == 0 )
        {
            continue;
        }
        else
        {
            vertex_attribute_enable( attribute );
        }
    }

    if( self->indices->size )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, self->indices_id );
    }
#endif

    self->mode = mode;
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_render_finish ( vertex_buffer_t *self )
{
#ifdef FREETYPE_GL_USE_VAO
    glBindVertexArray( 0 );
#else
    int i;

    for( i=0; i<MAX_VERTEX_ATTRIBUTE; ++i )
    {
        vertex_attribute_t *attribute = self->attributes[i];
        if( attribute == 0 )
        {
            continue;
        }
        else
        {
            glDisableVertexAttribArray( attribute->index );
        }
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
#endif
}


// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_render_item ( vertex_buffer_t *self,
                            size_t index )
{
    ivec4 * item = (ivec4 *) vector_get( self->items, index );
    assert( self );
    assert( index < vector_size( self->items ) );


    if( self->indices->size )
    {
        size_t start = item->istart;
        size_t count = item->icount;
        glDrawElements( self->mode, count, GL_UNSIGNED_INT, (void *)(start*sizeof(GLuint)) );
    }
    else if( self->vertices->size )
    {
        size_t start = item->vstart;
        size_t count = item->vcount;
        glDrawArrays( self->mode, start*self->vertices->item_size, count);
    }
}


// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_render ( vertex_buffer_t *self, GLenum mode )
{
    size_t vcount = self->vertices->size;
    size_t icount = self->indices->size;

    vertex_buffer_render_setup( self, mode );
    if( icount )
    {
        glDrawElements( mode, icount, GL_UNSIGNED_INT, 0 );
    }
    else
    {
        glDrawArrays( mode, 0, vcount );
    }
    vertex_buffer_render_finish( self );
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_push_back_indices ( vertex_buffer_t * self,
                                  const GLuint * indices,
                                  const size_t icount )
{
    assert( self );

    self->state |= DIRTY;
    vector_push_back_data( self->indices, indices, icount );
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_push_back_vertices ( vertex_buffer_t * self,
                                   const void * vertices,
                                   const size_t vcount )
{
    assert( self );

    self->state |= DIRTY;
    vector_push_back_data( self->vertices, vertices, vcount );
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_insert_indices ( vertex_buffer_t *self,
                               const size_t index,
                               const GLuint *indices,
                               const size_t count )
{
    assert( self );
    assert( self->indices );
    assert( index < self->indices->size+1 );

    self->state |= DIRTY;
    vector_insert_data( self->indices, index, indices, count );
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_insert_vertices( vertex_buffer_t *self,
                               const size_t index,
                               const void *vertices,
                               const size_t vcount )
{
    size_t i;
    assert( self );
    assert( self->vertices );
    assert( index < self->vertices->size+1 );

    self->state |= DIRTY;

     for( i=0; i<self->indices->size; ++i )
    {
        if( *(GLuint *)(vector_get( self->indices, i )) > index )
        {
            *(GLuint *)(vector_get( self->indices, i )) += index;
        }
    }

    vector_insert_data( self->vertices, index, vertices, vcount );
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_erase_indices( vertex_buffer_t *self,
                             const size_t first,
                             const size_t last )
{
    assert( self );
    assert( self->indices );
    assert( first < self->indices->size );
    assert( (last) <= self->indices->size );

    self->state |= DIRTY;
    vector_erase_range( self->indices, first, last );
}



// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_erase_vertices( vertex_buffer_t *self,
                              const size_t first,
                              const size_t last )
{
    size_t i;
    assert( self );
    assert( self->vertices );
    assert( first < self->vertices->size );
    assert( last <= self->vertices->size );
    assert( last > first );

    self->state |= DIRTY;
    for( i=0; i<self->indices->size; ++i )
    {
        if( *(GLuint *)(vector_get( self->indices, i )) > first )
        {
            *(GLuint *)(vector_get( self->indices, i )) -= (last-first);
        }
    }
    vector_erase_range( self->vertices, first, last );
}



// ----------------------------------------------------------------------------
size_t
GLHiddenWidget::vertex_buffer_push_back( vertex_buffer_t * self,
                         const void * vertices, const size_t vcount,
                         const GLuint * indices, const size_t icount )
{
    return vertex_buffer_insert( self, vector_size( self->items ),
                                 vertices, vcount, indices, icount );
}

// ----------------------------------------------------------------------------
size_t
GLHiddenWidget::vertex_buffer_insert( vertex_buffer_t * self, const size_t index,
                      const void * vertices, const size_t vcount,
                      const GLuint * indices, const size_t icount )
{
    size_t vstart, istart, i;
    ivec4 item;
    assert( self );
    assert( vertices );
    assert( indices );

    self->state = FROZEN;

    // Push back vertices
    vstart = vector_size( self->vertices );
    vertex_buffer_push_back_vertices( self, vertices, vcount );

    // Push back indices
    istart = vector_size( self->indices );
    vertex_buffer_push_back_indices( self, indices, icount );

    // Update indices within the vertex buffer
    for( i=0; i<icount; ++i )
    {
        *(GLuint *)(vector_get( self->indices, istart+i )) += vstart;
    }

    // Insert item
    item.x = vstart;
    item.y = vcount;
    item.z = istart;
    item.w = icount;
    vector_insert( self->items, index, &item );

    self->state = DIRTY;
    return index;
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::vertex_buffer_erase( vertex_buffer_t * self,
                     const size_t index )
{
    ivec4 * item;
    int vstart;
    size_t vcount, istart, icount, i;

    assert( self );
    assert( index < vector_size( self->items ) );

    item = (ivec4 *) vector_get( self->items, index );
    vstart = item->vstart;
    vcount = item->vcount;
    istart = item->istart;
    icount = item->icount;

    // Update items
    for( i=0; i<vector_size(self->items); ++i )
    {
        ivec4 * item = (ivec4 *) vector_get( self->items, i );
        if( item->vstart > vstart)
        {
            item->vstart -= vcount;
            item->istart -= icount;
        }
    }

    self->state = FROZEN;
    vertex_buffer_erase_indices( self, istart, istart+icount );
    vertex_buffer_erase_vertices( self, vstart, vstart+vcount );
    vector_erase( self->items, index );
    self->state = DIRTY;
}
void GLHiddenWidget::add_text( vertex_buffer_t * buffer, texture_font_t * font,
               const char * text, vec4 * color, vec2 * pen )
{
    size_t i;
    float r = color->red, g = color->green, b = color->blue, a = color->alpha;
    for( i = 0; i < strlen(text); ++i )
    {
        texture_glyph_t *glyph = texture_font_get_glyph( font, text + i );
        if( glyph != NULL )
        {
            float kerning =  0.0f;
            if( i > 0)
            {
                kerning = texture_glyph_get_kerning( glyph, text + i - 1 );
            }
            pen->x += kerning;
            int x0  = (int)( pen->x + glyph->offset_x );
            int y0  = (int)( pen->y + glyph->offset_y );
            int x1  = (int)( x0 + glyph->width );
            int y1  = (int)( y0 - glyph->height );
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;
            qDebug()<<"GLHiddenWidget::add_text text: "<<text[i]<<" kerning: "<<kerning<<" pen->x: "
                <<pen->x<<" offset_x: " <<glyph->offset_x<<" pen->y: "<<pen->y
                <<" offset_y: "<<glyph->offset_y<<" width: "<<glyph->width
                <<" height: "<<glyph->height<<" x0: "<<x0<<" y0: "<<y0<<" x1: "<<x1<<" y1: "<<y1
                <<" lefttop_s0: "<<s0<<" t0: "<<t0<<" s1: "<<s1<<" t1: "<<t1<<" advance_x: "<<glyph->advance_x;
            GLuint indices[6] = {0,1,2, 0,2,3};
            vertex_t vertices[4] = { { x0,y0,0,  s0,t0,  r,g,b,a },
                                     { x0,y1,0,  s0,t1,  r,g,b,a },
                                     { x1,y1,0,  s1,t1,  r,g,b,a },
                                     { x1,y0,0,  s1,t0,  r,g,b,a } };
            vertex_buffer_push_back( buffer, vertices, 4, indices, 6 );
            pen->x += glyph->advance_x;
        }
    }
}
GLuint
GLHiddenWidget::shader_load( const char * vert_filename,
              const char * frag_filename )
{
    GLuint handle = glCreateProgram( );
    GLint link_status;

    if( vert_filename && strlen( vert_filename ) )
    {
        char *vert_source = shader_read( vert_filename );
        GLuint vert_shader = shader_compile( vert_source, GL_VERTEX_SHADER);
        glAttachShader( handle, vert_shader);
        glDeleteShader( vert_shader );
        free( vert_source );
    }
    if( frag_filename && strlen( frag_filename ) )
    {
        char *frag_source = shader_read( frag_filename );
        GLuint frag_shader = shader_compile( frag_source, GL_FRAGMENT_SHADER);
        glAttachShader( handle, frag_shader);
        glDeleteShader( frag_shader );
        free( frag_source );
    }

    glLinkProgram( handle );

    glGetProgramiv( handle, GL_LINK_STATUS, &link_status );
    if (link_status == GL_FALSE)
    {
        GLchar messages[256];
        glGetProgramInfoLog( handle, sizeof(messages), 0, &messages[0] );
        //fprintf( stderr, "%s\n", messages );
        qDebug()<< messages ;
        exit(1);
    }
    return handle;
}
char *
GLHiddenWidget::shader_read( const char *filename )
{
    FILE * file;
    char * buffer;
	size_t size;

    file = fopen( filename, "rb" );
    if( !file )
    {
        //fprintf( stderr, "Unable to open file \"%s\".\n", filename );
        qDebug()<<"Unable to open file "<< filename ;
		return 0;
    }
	fseek( file, 0, SEEK_END );
	size = ftell( file );
	fseek(file, 0, SEEK_SET );
    buffer = (char *) malloc( (size+1) * sizeof( char *) );
	fread( buffer, sizeof(char), size, file );
    buffer[size] = 0;
    fclose( file );
    return buffer;
}
GLuint
GLHiddenWidget::shader_compile( const char* source,
                const GLenum type )
{
    GLint compile_status;
    GLuint handle = glCreateShader( type );
    glShaderSource( handle, 1, &source, 0 );
    glCompileShader( handle );

    glGetShaderiv( handle, GL_COMPILE_STATUS, &compile_status );
    if( compile_status == GL_FALSE )
    {
        GLchar messages[256];
        glGetShaderInfoLog( handle, sizeof(messages), 0, &messages[0] );
        //fprintf( stderr, "%s\n", messages );
        qDebug()<<messages;
        exit( EXIT_FAILURE );
    }
    return handle;
}
char *
GLHiddenWidget::match_description( char * description )
{

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
    fprintf( stderr, "\"font_manager_match_description\" "
                     "not implemented for windows.\n" );
    return 0;
#endif

    char *filename = 0;
    FcInit();
    FcPattern *pattern = FcNameParse((const FcChar8*)description);
    FcConfigSubstitute( 0, pattern, FcMatchPattern );
    FcDefaultSubstitute( pattern );
    FcResult result;
    FcPattern *match = FcFontMatch( 0, pattern, &result );
    FcPatternDestroy( pattern );

    if ( !match )
    {
        fprintf( stderr, "fontconfig error: could not match description '%s'", description );
        return 0;
    }
    else
    {
        FcValue value;
        FcResult result = FcPatternGet( match, FC_FILE, 0, &value );
        if ( result )
        {
            fprintf( stderr, "fontconfig error: could not match description '%s'", description );
        }
        else
        {
            filename = strdup( (char *)(value.u.s) );
        }
    }
    FcPatternDestroy( match );
    return filename;
}
void GLHiddenWidget::init(char* family)
{
    if(!text_shader)
        text_shader = shader_load( "text.vert",
                "text.frag" );

    if(!font_manager)
        font_manager = font_manager_new( 512, 512, LCD_FILTERING_ON );
    if(text_buffer)
    {
        text_buffer_delete(text_buffer);
    }
    text_buffer = text_buffer_new( );

    vec4 black  = {{0.0, 0.0, 0.0, 1.0}};
    vec4 white  = {{1.0, 1.0, 1.0, 1.0}};
    vec4 yellow = {{1.0, 1.0, 0.0, 1.0}};
    vec4 grey   = {{0.5, 0.5, 0.5, 1.0}};
    vec4 none   = {{1.0, 1.0, 1.0, 0.0}};

    //char *f_japanese = match_description("Droid Sans:size=18:lang=ja");
    //char *f_japanese = match_description("sans-serif:size=18:lang=zh");
    //char *f_japanese = match_description("sans\\-serif:size=18:lang=zh\\-CN");
    //char *f_japanese = match_description("FZLanTingHeiS\\-UL\\-GB:size=18:lang=zh\\-CN"); //ok
    //char *f_japanese = match_description("FZLanTingHeiS\\-UL\\-GB"); //ok
    //char *f_japanese = match_description("Yu Gothic UI Semibold"); //ok
    //char *f_japanese = match_description("jj:size=18:lang=zh\\-CN"); //ok
    //char *f_japanese = match_description("Brush Script MT:size=28:lang=zh\\-CN"); //ok
    //char *f_japanese = match_description("Microsoft JhengHei UI:size=28"); //ok
    char *f_japanese = match_description(family);
    //char *f_japanese = match_description("STHupo:size=28:lang=zh\\-CN"); //ok
    //char *f_japanese = match_description("华文隶书:size=28:lang=zh\\-CN"); //no
    printf("init. f_japanese: %s\n" , f_japanese);

    //exit(0);
    markup_t normal = {
        //.family  = f_normal,
        .family  = f_japanese,
        .size    = 20.0, .bold    = 0,   .italic  = 0,
        .spacing = 0.0,  .gamma   = 2.,
        .foreground_color    = white, .background_color    = none,
        .outline           = 0,     .outline_color     = white,
        .underline           = 0,     .underline_color     = white,
        .overline            = 0,     .overline_color      = white,
        .strikethrough       = 0,     .strikethrough_color = white,
        .font = 0,
    };
    markup_t japanese  = normal; japanese.family = f_japanese;
    //markup_t japanese  = normal; japanese.family = "C:/Windows/Fonts/simsun.ttc";
    //markup_t japanese  = normal; japanese.family = "c:/qtproject/opengl/freetype-gl/fonts/fireflysung.ttf";
                                 japanese.size = 25.0;
    japanese.font = font_manager_get_from_markup( font_manager, &japanese );
    qDebug()<<"GLHiddenWidgetText::init family: "<<family<<" f_japanese: "<<f_japanese<<" font: "<<(void*)japanese.font;
    free( f_japanese );

    vec2 pen = {{20, 200}};
#if 0
    text_buffer_printf( text_buffer, &pen,
                        &underline, "The",
                        &normal,    " Quick",
                        &big,       " brown ",
                        &reverse,   " fox \n",
                        &italic,    "jumps over ",
                        &bold,      "the lazy ",
                        &normal,    "dog.\n",
                        &small,     "Now is the time for all good men "
                                    "to come to the aid of the party.\n",
                        &italic,    "Ég get etið gler án þess að meiða mig.\n",
                        &japanese,  "aaa张私はガラスを食べられます。 それは私を傷つけません\n",
                        &math,      "ℕ ⊆ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ",
                        NULL );
#endif
    text_buffer_printf( text_buffer, &pen,
                        &japanese,  "Brush Script MT张私はガラスを食べられます。 それは私を傷つけません\n",
                        //&japanese,  "test font\n",
                        NULL );

    glGenTextures( 1, &font_manager->atlas->id );
    glBindTexture( GL_TEXTURE_2D, font_manager->atlas->id );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, font_manager->atlas->width,
                  font_manager->atlas->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                  font_manager->atlas->data );
    printf("init font_manager->atlas->width: %d font_manager->atlas->height: %d\n"
            , font_manager->atlas->width, font_manager->atlas->height);
    qDebug()<<"init font_manager->atlas->width:"<<font_manager->atlas->width
        <<"font_manager->atlas->height:"<<font_manager->atlas->height;

    text_buffer_align( text_buffer, &pen, ALIGN_CENTER );
#if 0
    vec4 bounds = text_buffer_get_bounds( text_buffer, &pen );
    float left = bounds.left;
    float right = bounds.left + bounds.width;
    float top = bounds.top;
    float bottom = bounds.top - bounds.height;

    bounds_shader = shader_load( "v3f-c4f.vert",
                                 "v3f-c4f.frag" );

    lines_buffer = vertex_buffer_new( "vertex:3f,color:4f" );
    vertex_t vertices[] = { { left - 10,         top, 0, 0,0,0,1}, // top
                            {right + 10,         top, 0, 0,0,0,1},

                            { left - 10,      bottom, 0, 0,0,0,1}, // bottom
                            {right + 10,      bottom, 0, 0,0,0,1},

                            {      left,    top + 10, 0, 0,0,0,1}, // left
                            {      left, bottom - 10, 0, 0,0,0,1},
                            {     right,    top + 10, 0, 0,0,0,1}, // right
                            {     right, bottom - 10, 0, 0,0,0,1} };
    GLuint indices[] = { 0,1,2,3,4,5,6,7 };
    vertex_buffer_push_back( lines_buffer, vertices, 8, indices, 8);
#endif
    mat4_set_identity( &projection );
    mat4_set_identity( &model );
    mat4_set_identity( &view );

    mat4_set_orthographic( &projection, 0, glw, 0, glh, -1, 1);

}
#if 0
void GLHiddenWidget::init( void )
{
    size_t i=100;//27;
    texture_font_t *font = 0;
    atlas = texture_atlas_new( 200, 200, 1 );
    const char * filename = "fonts/Vera.ttf";
    const char * text = "Qjgabb0";
    buffer = vertex_buffer_new( "vertex:3f,tex_coord:2f,color:4f" );
    vec2 pen = {{5,500}};
    vec4 black = {{0,0,0,1}};
    //for( i=7; i < 27; ++i)
    {
        font = texture_font_new_from_file( atlas, i, filename );
        pen.x = 5;
        pen.y -= font->height;
        //printf("pen x: %f y: %f font->height: %f\n", pen.x, pen.y, font->height);
        qDebug()<<"GLHiddenWidget::init pen x: "<<pen.x<<" y: "<<pen.y<<" height: "<<font->height;
        texture_font_load_glyphs( font, text );
        //const char * text = "klzxcvbnm";
        add_text( buffer, font, text, &black, &pen );
        texture_font_delete( font );
    }

    glGenTextures( 1, &atlas->id );
    glBindTexture( GL_TEXTURE_2D, atlas->id );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
                  0, GL_RED, GL_UNSIGNED_BYTE, atlas->data );

#if 1
    shader = shader_load("shaders/v3f-t2f-c4f.vert",
                         "shaders/v3f-t2f-c4f.frag");
    mat4_set_identity( &projection );
    mat4_set_identity( &model );
    mat4_set_identity( &view );
#endif
    mat4_set_orthographic( &projection, 0, glw, 0, glh, -1, 1);
}
#endif
void GLHiddenWidget::display( )
{
    qDebug()<<"GLHiddenWidget::display glw: "<<glw<<" glh: "<<glh;
    glClearColor(0.40,0.40,0.45,1.00);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glColor4f(1.00,1.00,1.00,1.00);
    glUseProgram( text_shader );
    {
        glUniformMatrix4fv( glGetUniformLocation( text_shader, "model" ),
                            1, 0, model.data);
        glUniformMatrix4fv( glGetUniformLocation( text_shader, "view" ),
                            1, 0, view.data);
        glUniformMatrix4fv( glGetUniformLocation( text_shader, "projection" ),
                            1, 0, projection.data);
        glUniform1i( glGetUniformLocation( text_shader, "tex" ), 0 );
        glUniform3f( glGetUniformLocation( text_shader, "pixel" ),
                     1.0f/font_manager->atlas->width,
                     1.0f/font_manager->atlas->height,
                     (float)font_manager->atlas->depth );

        glEnable( GL_BLEND );

        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, font_manager->atlas->id );

        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glBlendColor( 1, 1, 1, 1 );

        vertex_buffer_render( text_buffer->buffer, GL_TRIANGLES );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glBlendColor( 0, 0, 0, 0 );
        glUseProgram( 0 );
    }
#if 0
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    glBlendColor( 1.0, 1.0, 1.0, 1.0 );
    glUseProgram( bounds_shader );
    {
        glUniformMatrix4fv( glGetUniformLocation( bounds_shader, "model" ),
                            1, 0, model.data);
        glUniformMatrix4fv( glGetUniformLocation( bounds_shader, "view" ),
                            1, 0, view.data);
        glUniformMatrix4fv( glGetUniformLocation( bounds_shader, "projection" ),
                            1, 0, projection.data);
        vertex_buffer_render( lines_buffer, GL_LINES );
    }
#endif
    
    ////glfwSwapBuffers( window );
}
#if 0
void GLHiddenWidget::display( )
{
    qDebug()<<"GLHiddenWidget::display glw: "<<glw<<" glh: "<<glh;
    ////glClearColor( 1, 1, 1, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glUseProgram( shader );
    {
        glUniform1i( glGetUniformLocation( shader, "texture" ),
                     0 );
        glUniformMatrix4fv( glGetUniformLocation( shader, "model" ),
                            1, 0, model.data);
        glUniformMatrix4fv( glGetUniformLocation( shader, "view" ),
                            1, 0, view.data);
        glUniformMatrix4fv( glGetUniformLocation( shader, "projection" ),
                            1, 0, projection.data);
        vertex_buffer_render( buffer, GL_TRIANGLES );
    }
    //glFinish();

}
#endif
text_buffer_t *
GLHiddenWidget::text_buffer_new( )
{
    text_buffer_t *self = (text_buffer_t *) malloc (sizeof(text_buffer_t));
    self->buffer = vertex_buffer_new(
                                     "vertex:3f,tex_coord:2f,color:4f,ashift:1f,agamma:1f" );
    self->line_start = 0;
    self->line_ascender = 0;
    self->base_color.r = 0.0;
    self->base_color.g = 0.0;
    self->base_color.b = 0.0;
    self->base_color.a = 1.0;
    self->line_descender = 0;
    self->lines = vector_new( sizeof(line_info_t) );
    self->bounds.left   = 0.0;
    self->bounds.top    = 0.0;
    self->bounds.width  = 0.0;
    self->bounds.height = 0.0;
    return self;
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::text_buffer_delete( text_buffer_t * self )
{
    vector_delete( self->lines );
    vertex_buffer_delete( self->buffer );
    free( self );
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::text_buffer_clear( text_buffer_t * self )
{
    assert( self );

    vertex_buffer_clear( self->buffer );
    self->line_start = 0;
    self->line_ascender = 0;
    self->line_descender = 0;
    vector_clear( self->lines );
    self->bounds.left   = 0.0;
    self->bounds.top    = 0.0;
    self->bounds.width  = 0.0;
    self->bounds.height = 0.0;
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::text_buffer_printf( text_buffer_t * self, vec2 *pen, ... )
{
    markup_t *markup;
    char *text;
    va_list args;

    if( vertex_buffer_size( self->buffer ) == 0 )
    {
        self->origin = *pen;
    }

    va_start ( args, pen );
    do {
        markup = va_arg( args, markup_t * );
        if( markup == NULL )
        {
            return;
        }
        text = va_arg( args, char * );
        text_buffer_add_text( self, pen, markup, text, 0 );
    } while( markup != 0 );
    va_end ( args );
}

// ----------------------------------------------------------------------------
void
text_buffer_move_last_line( text_buffer_t * self, float dy )
{
    size_t i;
    int j;
    for( i=self->line_start; i < vector_size( self->buffer->items ); ++i )
    {
        ivec4 *item = (ivec4 *) vector_get( self->buffer->items, i);
        for( j=item->vstart; j<item->vstart+item->vcount; ++j)
        {
            glyph_vertex_t * vertex =
                (glyph_vertex_t *)  vector_get( self->buffer->vertices, j );
            vertex->y -= dy;
        }
    }
}


// ----------------------------------------------------------------------------
// text_buffer_finish_line (internal use only)
// 
//  Performs calculations needed at the end of each line of text 
//  and prepares for the next line if necessary
//
//  advancePen: if true, advance the pen to the next line
//
static void
text_buffer_finish_line( text_buffer_t * self, vec2 * pen, bool advancePen )
{
    float line_left = self->line_left;
    float line_right = pen->x;
    float line_width  = line_right - line_left;
    float line_top = pen->y + self->line_ascender;
    float line_height = self->line_ascender - self->line_descender;
    float line_bottom = line_top - line_height;

    line_info_t line_info;
    line_info.line_start = self->line_start;
    line_info.bounds.left = line_left;
    line_info.bounds.top = line_top;
    line_info.bounds.width = line_width;
    line_info.bounds.height = line_height;

    vector_push_back( self->lines,  &line_info);


    if (line_left < self->bounds.left)
    {
        self->bounds.left = line_left;
    }
    if (line_top > self->bounds.top)
    {
        self->bounds.top = line_top;
    }

    float self_right = self->bounds.left + self->bounds.width;
    float self_bottom = self->bounds.top - self->bounds.height;

    if (line_right > self_right)
    {
        self->bounds.width = line_right - self->bounds.left;
    }
    if (line_bottom < self_bottom)
    {
        self->bounds.height = self->bounds.top - line_bottom;
    }

    if ( advancePen )
    {
        pen->x = self->origin.x;
        pen->y += (int)(self->line_descender);
    }

    self->line_descender = 0;
    self->line_ascender = 0;
    self->line_start = vector_size( self->buffer->items );
    self->line_left = pen->x;
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::text_buffer_add_text( text_buffer_t * self,
                      vec2 * pen, markup_t * markup,
                      const char * text, size_t length )
{
    size_t i;
    const char * prev_character = NULL;

    if( markup == NULL )
    {
        return;
    }

    if( !markup->font )
    {
        fprintf( stderr, "Houston, we've got a problem !\n" );
        return;
    }

    if( length == 0 )
    {
        length = utf8_strlen(text);
    }
    if( vertex_buffer_size( self->buffer ) == 0 )
    {
        self->origin = *pen;
        self->line_left = pen->x;
        self->bounds.left = pen->x;
        self->bounds.top = pen->y;
    }
    else
    {
        if (pen->x < self->origin.x)
        {
            self->origin.x = pen->x;
        }
        if (pen->y != self->last_pen_y)
        {
            text_buffer_finish_line(self, pen, false);
        }
    }

    for( i = 0; length; i += utf8_surrogate_len( text + i ) )
    {
        text_buffer_add_char( self, pen, markup, text + i, prev_character );
        prev_character = text + i;
        length--;
    }

    self->last_pen_y = pen->y;
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::text_buffer_add_char( text_buffer_t * self,
                      vec2 * pen, markup_t * markup,
                      const char * current, const char * previous )
{
    size_t vcount = 0;
    size_t icount = 0;
    vertex_buffer_t * buffer = self->buffer;
    texture_font_t * font = markup->font;
    float gamma = markup->gamma;

    // Maximum number of vertices is 20 (= 5x2 triangles) per glyph:
    //  - 2 triangles for background
    //  - 2 triangles for overline
    //  - 2 triangles for underline
    //  - 2 triangles for strikethrough
    //  - 2 triangles for glyph
    glyph_vertex_t vertices[4*5];
    GLuint indices[6*5];
    texture_glyph_t *glyph;
    texture_glyph_t *black;
    float kerning = 0.0f;

    if( markup->font->ascender > self->line_ascender )
    {
        float y = pen->y;
        pen->y -= (markup->font->ascender - self->line_ascender);
        text_buffer_move_last_line( self, (float)(int)(y-pen->y) );
        self->line_ascender = markup->font->ascender;
    }
    if( markup->font->descender < self->line_descender )
    {
        self->line_descender = markup->font->descender;
    }

    if( *current == '\n' )
    {
        text_buffer_finish_line(self, pen, true);
        return;
    }

    glyph = texture_font_get_glyph( font, current );
    black = texture_font_get_glyph( font, NULL );

    if( glyph == NULL )
    {
        return;
    }

    if( previous && markup->font->kerning )
    {
        kerning = texture_glyph_get_kerning( glyph, previous );
    }
    pen->x += kerning;

    // Background
    if( markup->background_color.alpha > 0 )
    {
        float r = markup->background_color.r;
        float g = markup->background_color.g;
        float b = markup->background_color.b;
        float a = markup->background_color.a;
        float x0 = ( pen->x -kerning );
        float y0 = (float)(int)( pen->y + font->descender );
        float x1 = ( x0 + glyph->advance_x );
        float y1 = (float)(int)( y0 + font->height + font->linegap );
        float s0 = black->s0;
        float t0 = black->t0;
        float s1 = black->s1;
        float t1 = black->t1;

        SET_GLYPH_VERTEX(vertices[vcount+0],
                         (float)(int)x0,y0,0,  s0,t0,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+1],
                         (float)(int)x0,y1,0,  s0,t1,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+2],
                         (float)(int)x1,y1,0,  s1,t1,  r,g,b,a,  x1-((int)x1), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+3],
                         (float)(int)x1,y0,0,  s1,t0,  r,g,b,a,  x1-((int)x1), gamma );
        indices[icount + 0] = vcount+0;
        indices[icount + 1] = vcount+1;
        indices[icount + 2] = vcount+2;
        indices[icount + 3] = vcount+0;
        indices[icount + 4] = vcount+2;
        indices[icount + 5] = vcount+3;
        vcount += 4;
        icount += 6;
    }

    // Underline
    if( markup->underline )
    {
        float r = markup->underline_color.r;
        float g = markup->underline_color.g;
        float b = markup->underline_color.b;
        float a = markup->underline_color.a;
        float x0 = ( pen->x - kerning );
        float y0 = (float)(int)( pen->y + font->underline_position );
        float x1 = ( x0 + glyph->advance_x );
        float y1 = (float)(int)( y0 + font->underline_thickness );
        float s0 = black->s0;
        float t0 = black->t0;
        float s1 = black->s1;
        float t1 = black->t1;

        SET_GLYPH_VERTEX(vertices[vcount+0],
                         (float)(int)x0,y0,0,  s0,t0,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+1],
                         (float)(int)x0,y1,0,  s0,t1,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+2],
                         (float)(int)x1,y1,0,  s1,t1,  r,g,b,a,  x1-((int)x1), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+3],
                         (float)(int)x1,y0,0,  s1,t0,  r,g,b,a,  x1-((int)x1), gamma );
        indices[icount + 0] = vcount+0;
        indices[icount + 1] = vcount+1;
        indices[icount + 2] = vcount+2;
        indices[icount + 3] = vcount+0;
        indices[icount + 4] = vcount+2;
        indices[icount + 5] = vcount+3;
        vcount += 4;
        icount += 6;
    }

    // Overline
    if( markup->overline )
    {
        float r = markup->overline_color.r;
        float g = markup->overline_color.g;
        float b = markup->overline_color.b;
        float a = markup->overline_color.a;
        float x0 = ( pen->x -kerning );
        float y0 = (float)(int)( pen->y + (int)font->ascender );
        float x1 = ( x0 + glyph->advance_x );
        float y1 = (float)(int)( y0 + (int)font->underline_thickness );
        float s0 = black->s0;
        float t0 = black->t0;
        float s1 = black->s1;
        float t1 = black->t1;
        SET_GLYPH_VERTEX(vertices[vcount+0],
                         (float)(int)x0,y0,0,  s0,t0,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+1],
                         (float)(int)x0,y1,0,  s0,t1,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+2],
                         (float)(int)x1,y1,0,  s1,t1,  r,g,b,a,  x1-((int)x1), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+3],
                         (float)(int)x1,y0,0,  s1,t0,  r,g,b,a,  x1-((int)x1), gamma );
        indices[icount + 0] = vcount+0;
        indices[icount + 1] = vcount+1;
        indices[icount + 2] = vcount+2;
        indices[icount + 3] = vcount+0;
        indices[icount + 4] = vcount+2;
        indices[icount + 5] = vcount+3;
        vcount += 4;
        icount += 6;
    }

    /* Strikethrough */
    if( markup->strikethrough )
    {
        float r = markup->strikethrough_color.r;
        float g = markup->strikethrough_color.g;
        float b = markup->strikethrough_color.b;
        float a = markup->strikethrough_color.a;
        float x0  = ( pen->x -kerning );
        float y0  = (float)(int)( pen->y + (int)font->ascender*.33f);
        float x1  = ( x0 + glyph->advance_x );
        float y1  = (float)(int)( y0 + (int)font->underline_thickness );
        float s0 = black->s0;
        float t0 = black->t0;
        float s1 = black->s1;
        float t1 = black->t1;
        SET_GLYPH_VERTEX(vertices[vcount+0],
                         (float)(int)x0,y0,0,  s0,t0,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+1],
                         (float)(int)x0,y1,0,  s0,t1,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+2],
                         (float)(int)x1,y1,0,  s1,t1,  r,g,b,a,  x1-((int)x1), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+3],
                         (float)(int)x1,y0,0,  s1,t0,  r,g,b,a,  x1-((int)x1), gamma );
        indices[icount + 0] = vcount+0;
        indices[icount + 1] = vcount+1;
        indices[icount + 2] = vcount+2;
        indices[icount + 3] = vcount+0;
        indices[icount + 4] = vcount+2;
        indices[icount + 5] = vcount+3;
        vcount += 4;
        icount += 6;
    }
    {
        // Actual glyph
        float r = markup->foreground_color.red;
        float g = markup->foreground_color.green;
        float b = markup->foreground_color.blue;
        float a = markup->foreground_color.alpha;
        float x0 = ( pen->x + glyph->offset_x );
        float y0 = (float)(int)( pen->y + glyph->offset_y );
        float x1 = ( x0 + glyph->width );
        float y1 = (float)(int)( y0 - glyph->height );
        float s0 = glyph->s0;
        float t0 = glyph->t0;
        float s1 = glyph->s1;
        float t1 = glyph->t1;

        SET_GLYPH_VERTEX(vertices[vcount+0],
                         (float)(int)x0,y0,0,  s0,t0,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+1],
                         (float)(int)x0,y1,0,  s0,t1,  r,g,b,a,  x0-((int)x0), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+2],
                         (float)(int)x1,y1,0,  s1,t1,  r,g,b,a,  x1-((int)x1), gamma );
        SET_GLYPH_VERTEX(vertices[vcount+3],
                         (float)(int)x1,y0,0,  s1,t0,  r,g,b,a,  x1-((int)x1), gamma );
        indices[icount + 0] = vcount+0;
        indices[icount + 1] = vcount+1;
        indices[icount + 2] = vcount+2;
        indices[icount + 3] = vcount+0;
        indices[icount + 4] = vcount+2;
        indices[icount + 5] = vcount+3;
        vcount += 4;
        icount += 6;

        vertex_buffer_push_back( buffer, vertices, vcount, indices, icount );
        pen->x += glyph->advance_x * (1.0f + markup->spacing);
    }
}

// ----------------------------------------------------------------------------
void
GLHiddenWidget::text_buffer_align( text_buffer_t * self, vec2 * pen,
                   enum Align alignment )
{
    if (ALIGN_LEFT == alignment)
    {
        return;
    }

    size_t total_items = vector_size( self->buffer->items );
    if ( self->line_start != total_items )
    {
        text_buffer_finish_line( self, pen, false );
    }


    size_t i, j;
    int k;
    float self_left, self_right, self_center;
    float line_left, line_right, line_center;
    float dx;

    self_left = self->bounds.left;
    self_right = self->bounds.left + self->bounds.width;
    self_center = (self_left + self_right) / 2;

    line_info_t* line_info;
    size_t lines_count, line_end;

    lines_count = vector_size( self->lines );
    for ( i = 0; i < lines_count; ++i )
    {
        line_info = (line_info_t*)vector_get( self->lines, i );

        if ( i + 1 < lines_count )
        {
            line_end = ((line_info_t*)vector_get( self->lines, i + 1 ))->line_start;
        }
        else
        {
            line_end = vector_size( self->buffer->items );
        }

        line_right = line_info->bounds.left + line_info->bounds.width;

        if ( ALIGN_RIGHT == alignment )
        {
            dx = self_right - line_right;
        }
        else // ALIGN_CENTER
        {
            line_left = line_info->bounds.left;
            line_center = (line_left + line_right) / 2;
            dx = self_center - line_center;
        }

        dx = roundf( dx );

        for( j=line_info->line_start; j < line_end; ++j )
        {
            ivec4 *item = (ivec4 *) vector_get( self->buffer->items, j);
            for( k=item->vstart; k<item->vstart+item->vcount; ++k)
            {
                glyph_vertex_t * vertex =
                                   (glyph_vertex_t *)vector_get( self->buffer->vertices, k );
                vertex->x += dx;
            }
        }
        
    }
}

vec4
GLHiddenWidget::text_buffer_get_bounds( text_buffer_t * self, vec2 * pen )
{
    size_t total_items = vector_size( self->buffer->items );
    if ( self->line_start != total_items )
    {
        text_buffer_finish_line( self, pen, false );
    }

    return self->bounds;
}
#endif
