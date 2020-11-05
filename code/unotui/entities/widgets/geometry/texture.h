#ifndef __UNOTU_ENTITIES_WIDGETS_GEOMETRY_TEXTURE_H_
#define __UNOTU_ENTITIES_WIDGETS_GEOMETRY_TEXTURE_H_

#include <unotui\entities\widgets\geometry\base.h>

#include <vector>

#include <unotui\utility\types.h>

namespace unotui {

struct texture_geometry : geometry
{
        std::vector<textured_vertex> Vertices;
        std::vector<unsigned int> Indices;
        
        texture_geometry( GLenum usage_ = GL_STATIC_DRAW );
        virtual void Update(void);
        virtual void Draw(void);
        void Draw( GLuint );
        
        virtual void Clear(void);
        void AddRectangle( const textured_rectangle& );
};

} // namespace unotui

#endif
