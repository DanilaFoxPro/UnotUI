#ifndef __CALLBACKS_H_
#define __CALLBACKS_H_

#include <unotui\utility\deps\opengl_includes.h>

namespace unotui {

void CallbackKey(GLFWwindow* WindowReference, int Key, int Scancode, int Action, int Modifiers );
void CallbackCharacter(GLFWwindow* WindowReference, unsigned int Codepoint);
void CallbackScroll( GLFWwindow* WindowReference, double X, double Y );

void CallbackExit();

} // namespace unotui

#endif
