#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

typedef struct {
    //const AVClass *class;
    GLuint        program;
    GLuint        frame_tex;
    GLFWwindow    *window;
    GLuint        pos_buf;
    int        w;
    int        h;
} GenericShaderContext;

#define PIXEL_FORMAT GL_RGB

static const float position[12] = {
    -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};

static const GLchar *v_shader_source =
"attribute vec2 position;\n"
"varying vec2 texCoord;\n"
"void main(void) {\n"
"  gl_Position = vec4(position, 0, 1);\n"
"  texCoord = position;\n"
"}\n";

static const GLchar *f_shader_source =
"vec4 INPUT(vec2 tc);\n"
"\n"
"vec4 rgb_to_xyz(vec4 color) {\n"
"float var_R = (color.r); //R from 0.0 to 255.0\n"
"float var_G = (color.g); //G from 0.0 to 255.0\n"
"float var_B = (color.b); //B from 0.0 to 255.0\n"
"\n"
"if (var_R > 0.04045) {\n"
"var_R = pow(((var_R + 0.055) / 1.055), 2.4);\n"
"} else {\n"
"var_R = var_R / 12.92;\n"
"}\n"
"if (var_G > 0.04045) {\n"
"var_G = pow(((var_G + 0.055) / 1.055), 2.4);\n"
"} else {\n"
"var_G = var_G / 12.92;\n"
"}\n"
"\n"
"if (var_B > 0.04045) {\n"
"var_B = pow(((var_B + 0.055) / 1.055), 2.4);\n"
"} else {\n"
"var_B = var_B / 12.92;\n"
"}\n"
"\n"
"var_R = var_R * 100.0;\n"
"var_G = var_G * 100.0;\n"
"var_B = var_B * 100.0;\n"
"\n"
"//Observer. = 2.0°, Illuminant = D65\n"
"float X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;\n"
"float Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;\n"
"float Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;\n"
"\n"
"return vec4(X, Y, Z, color.a);\n"
"}\n"
"\n"
"vec4 xyz_to_lab(vec4 color) {\n"
"\n"
"float ref_X = 95.047; //Observer= 2.0°, Illuminant= D65\n"
"float ref_Y = 100.000;\n"
"float ref_Z = 108.883;\n"
"\n"
"float var_X = color.r / ref_X;\n"
"float var_Y = color.g / ref_Y;\n"
"float var_Z = color.b / ref_Z;\n"
"\n"
"if (var_X > 0.008856) {\n"
"var_X = pow(var_X, (1.0 / 3.0));\n"
"} else {\n"
"var_X = (7.787 * var_X) + (16.0 / 116.0);\n"
"}\n"
"if (var_Y > 0.008856) {\n"
"var_Y = pow(var_Y, (1.0 / 3.0));\n"
"} else {\n"
"var_Y = (7.787 * var_Y) + (16.0 / 116.0);\n"
"}\n"
"if (var_Z > 0.008856) {\n"
"var_Z = pow(var_Z, (1.0 / 3.0));\n"
"} else {\n"
"var_Z = (7.787 * var_Z) + (16.0 / 116.0);\n"
"}\n"
"\n"
"float L = (116.0 * var_Y) - 16.0;\n"
"float a = 500.0 * (var_X - var_Y);\n"
"float b = 200.0 * (var_Y - var_Z);\n"
"\n"
"return vec4(L, a, b, color.a);\n"
"}\n"
"\n"
"vec4 lab_to_lch(vec4 color) {\n"
"\n"
"const float MPI = 3.14159265359;\n"
"\n"
"float var_H = atan(color.b, color.g); //in GLSL this is atan2\n"
"\n"
"if (var_H > 0.0) {\n"
"var_H = (var_H / MPI) * 180.0;\n"
"} else {\n"
"var_H = 360.0 - (abs(var_H) / MPI) * 180.0;\n"
"}\n"
"\n"
"float C = sqrt(pow(color.g, 2.0) + pow(color.b, 2.0));\n"
"float H = var_H;\n"
"\n"
"return vec4(color.r, C, H, color.a);\n"
"}\n"
"// ---------------------------\n"
"vec4 lch_to_lab(vec4 color) {\n"
"float a = cos(radians(color.b)) * color.g;\n"
"float b = sin(radians(color.b)) * color.g;\n"
"\n"
"return vec4(color.r, a, b, color.a);\n"
"}\n"
"\n"
"vec4 lab_to_xyz(vec4 color) {\n"
"float var_Y = (color.r + 16.0) / 116.0;\n"
"float var_X = color.g / 500.0 + var_Y;\n"
"float var_Z = var_Y - color.b / 200.0;\n"
"\n"
"if (pow(var_Y, 3.0) > 0.008856) {\n"
"var_Y = pow(var_Y, 3.0);\n"
"} else {\n"
"var_Y = (var_Y - 16.0 / 116.0) / 7.787;\n"
"}\n"
"if (pow(var_X, 3.0) > 0.008856) {\n"
"var_X = pow(var_X, 3.0);\n"
"} else {\n"
"var_X = (var_X - 16.0 / 116.0) / 7.787;\n"
"}\n"
"if (pow(var_Z, 3.0) > 0.008856) {\n"
"var_Z = pow(var_Z, 3.0);\n"
"} else {\n"
"var_Z = (var_Z - 16.0 / 116.0) / 7.787;\n"
"}\n"
"\n"
"float ref_X = 95.047; //Observer= 2.0 degrees, Illuminant= D65\n"
"float ref_Y = 100.000;\n"
"float ref_Z = 108.883;\n"
"\n"
"float X = ref_X * var_X;\n"
"float Y = ref_Y * var_Y;\n"
"float Z = ref_Z * var_Z;\n"
"\n"
"return vec4(X, Y, Z, color.a);\n"
"}\n"
"\n"
"vec4 xyz_to_rgb(vec4 color) {\n"
"float var_X = color.r / 100.0; //X from 0.0 to  95.047      (Observer = 2.0 degrees, Illuminant = D65);\n"
"float var_Y = color.g / 100.0; //Y from 0.0 to 100.000;\n"
"float var_Z = color.b / 100.0; //Z from 0.0 to 108.883;\n"
"\n"
"float var_R = var_X * 3.2406 + var_Y * -1.5372 + var_Z * -0.4986;\n"
"float var_G = var_X * -0.9689 + var_Y * 1.8758 + var_Z * 0.0415;\n"
"float var_B = var_X * 0.0557 + var_Y * -0.2040 + var_Z * 1.0570;\n"
"\n"
"if (var_R > 0.0031308) {\n"
"var_R = 1.055 * pow(var_R, (1.0 / 2.4)) - 0.055;\n"
"} else {\n"
"var_R = 12.92 * var_R;\n"
"}\n"
"if (var_G > 0.0031308) {\n"
"var_G = 1.055 * pow(var_G, (1.0 / 2.4)) - 0.055;\n"
"} else {\n"
"var_G = 12.92 * var_G;\n"
"}\n"
"if (var_B > 0.0031308) {\n"
"var_B = 1.055 * pow(var_B, (1.0 / 2.4)) - 0.055;\n"
"} else {\n"
"var_B = 12.92 * var_B;\n"
"}\n"
"\n"
"float R = var_R;\n"
"float G = var_G;\n"
"float B = var_B;\n"
"\n"
"return vec4(R, G, B, color.a);\n"
"}\n"
"\n"
"\n"
"vec4 rgb_to_lch(vec4 color) {\n"
"vec4 xyz = rgb_to_xyz(color);\n"
"vec4 lab = xyz_to_lab(xyz);\n"
"vec4 lch = lab_to_lch(lab);\n"
"return lch;\n"
"}\n"
"\n"
"\n"
"vec4 lch_to_rgb(vec4 color) {\n"
"vec4 lab = lch_to_lab(color);\n"
"vec4 xyz = lab_to_xyz(lab);\n"
"vec4 rgb = xyz_to_rgb(xyz);\n"
"return rgb;\n"
"}\n"
"\n"
"vec4 rgb_to_lab(vec4 color) {\n"
"vec4 xyz = rgb_to_xyz(color);\n"
"vec4 lab = xyz_to_lab(xyz);\n"
"return lab;\n"
"}\n"
"\n"
"vec4 lab_to_rgb(vec4 color) {\n"
"vec4 xyz = lab_to_xyz(color);\n"
"vec4 rgb = xyz_to_rgb(xyz);\n"
"return rgb;\n"
"}\n"
"\n"
"float thrdB(float l)\n"
"{\n"
"if (l < 10.)\n"
"return 0.;\n"
"float valIn = (l - 15.) / 85.;\n"
"float valOut = 1. - pow(1.-valIn,1.36);\n"
"return valOut * 100.;\n"
"}\n"
"\n"
"\n"
"vec4 FUNCNAME(vec2 tc)\n"
"{\n"
"vec4 col = INPUT(tc);\n"
"vec4 lab = rgb_to_lab(col);\n"
"lab.r = thrdB(lab.r);\n"
"return lab_to_rgb(lab);\n"
"}\n"
"\n"
"uniform sampler2D tex;\n"
"varying vec2 texCoord;\n"
"vec4 INPUT(vec2 tc)\n"
"{\n"
"//return texture2D(tex, texCoord * 0.5 + 0.5);\n"
"return texture2D(tex, tc);\n"
"}\n"
"void main() {\n"
"//gl_FragColor = texture2D(tex, texCoord * 0.5 + 0.5);\n"
"gl_FragColor = FUNCNAME(texCoord * 0.5 + 0.5);\n"
"//gl_FragColor = FUNCNAME(texCoord);\n"
"}\n";

static GLuint build_shader(const GLchar *shader_source, GLenum type) {
    GLuint shader = glCreateShader(type);
    if (!shader || !glIsShader(shader)) {
        return 0;
    }
    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		int length;
        char *log;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		//log =(char*) av_malloc(length);//new char[length];
		//glGetShaderInfoLog(shader, length, &length, log);
        //av_log(NULL, AV_LOG_ERROR, "build_shader error: %s\n", log);
        //av_free  (log);
	}
    return status == GL_TRUE ? shader : 0;
}

static void vbo_setup(GenericShaderContext *gs) {
    //http://blog.csdn.net/wangdingqiaoit/article/details/51457675
    glGenBuffers(1, &gs->pos_buf);
    glBindBuffer(GL_ARRAY_BUFFER, gs->pos_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    GLint loc = glGetAttribLocation(gs->program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

static void tex_setup(GenericShaderContext *gs) {

    glGenTextures(1, &gs->frame_tex);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, gs->frame_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gs->w, gs->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);

    glUniform1i(glGetUniformLocation(gs->program, "tex"), 0);
}
int opengl(unsigned char* in, unsigned char* out, int w, int h)
{
    GenericShaderContext* gs = new GenericShaderContext;
    gs->w=w;
    gs->h=h;
    glfwWindowHint(GLFW_VISIBLE, 0);
    gs->window = glfwCreateWindow(gs->w, gs->h, "", NULL, NULL);

    glfwMakeContextCurrent(gs->window);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        //av_log(NULL, AV_LOG_ERROR, "glewInit. error: %s\n", glewGetErrorString(err));
        //qDebug()<<"glewInit. error: "<<glewGetErrorString(err);
        return err;
    }
#endif

    glViewport(0, 0, gs->w, gs->h);

    GLuint v_shader, f_shader;
    v_shader = build_shader(v_shader_source, GL_VERTEX_SHADER);
    //av_log(NULL, AV_LOG_ERROR, "Fragment 2.1\n");
    f_shader = build_shader(f_shader_source, GL_FRAGMENT_SHADER);
    if (!(v_shader && f_shader )) {
        // av_log(NULL, AV_LOG_ERROR, "build_shader error\n");
        // return -1;
        // qDebug()<<"build_shader error";
        return -1;
    }
    gs->program = glCreateProgram();
    glAttachShader(gs->program, v_shader);
    glAttachShader(gs->program, f_shader);
    glLinkProgram(gs->program);

    GLint status;
    glGetProgramiv(gs->program, GL_LINK_STATUS, &status);
    //end build_program

    glUseProgram(gs->program);
    vbo_setup(gs);
    tex_setup(gs);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gs->w, gs->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, in);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glReadPixels(0, 0, gs->w, gs->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *)out);

    return 0;
}
#if 0
int main(int argc, char *argv[])
{
    //opengl();
    return 0;
}
#endif
