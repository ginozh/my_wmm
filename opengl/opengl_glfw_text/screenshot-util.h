/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#ifndef __UTIL_SCREENSHOT_H__
#define __UTIL_SCREENSHOT_H__

#include "opengl.h"
#include <GLFW/glfw3.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
namespace ftgl {
#endif
/**
 * Creates a screenshot from @window and saves it as a Targa file at @path.
 */
void screenshot( GLFWwindow* window, const char* path );
#ifdef __cplusplus
}
}
#endif

#endif /* __UTIL_SCREENSHOT_H__ */
