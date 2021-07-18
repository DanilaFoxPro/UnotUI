#ifndef __UNOTUI_ENTITIES_ENT_OPENGL_H_
#define __UNOTUI_ENTITIES_ENT_OPENGL_H_

#include <unotui/utility/deps/opengl_includes.h> // GLuint.
#include <vector>
#include <chrono>

#include <unotui/utility/types.h>

namespace unotui {

namespace chrono = std::chrono;

struct ent_opengl
{
	// Shaders.
	GLuint ColorShader;
	GLuint TextureShader;
	GLuint TextShader;
	
	// Textures.
	unsigned int FontTexture;
        
        // Other.
	chrono::microseconds IdealFramePeriod = chrono::microseconds(16666);
        
};

extern ent_opengl the_opengl;

} // namespace unotui

#endif
