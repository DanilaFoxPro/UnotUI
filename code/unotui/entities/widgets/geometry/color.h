#ifndef __UNOTU_ENTITIES_WIDGETS_GEOMETRY_COLOR_H_
#define __UNOTU_ENTITIES_WIDGETS_GEOMETRY_COLOR_H_

#include <unotui/entities/widgets/geometry/base.h>

#include <vector>

#include <unotui/utility/types.h>
#include <unotui/utility/shortcuts.h>
#include <unotui/utility/colors.h>

namespace unotui {

struct color_geometry : geometry
{
        std::vector<colored_vertex> Vertices;
        std::vector<unsigned int> Indices;
        
        color_geometry( GLenum usage_ = GL_STATIC_DRAW );
        
        virtual void Update();
        virtual void Draw();
        
        virtual void Clear();
        void AddLineSegment( const point Beginning, const point End, const uint Thinkness, const rgba Color = color::black );
        void AddArrow(
                const point Beginning,
                const point End,
                const uint Thickness,
                const rgba Color = color::black
        );
        void AddArrow(
                const point Beginning,
                const point End,
                const uint Thickness,
                const uint TipWidth,
                const uint TipHeight,
                const rgba Color = color::black
        );
        void AddRectangle( const colored_rectangle& );
        void AddOutline( const rectangle& Bounds, int Thickness, rgba Left, rgba Right, rgba Top, rgba Bottom );
        void AddOutline( const colored_rectangle&, int = 5);
};

} // namespace unotui

#endif
