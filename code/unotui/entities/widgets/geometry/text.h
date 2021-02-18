#ifndef __UNOTU_ENTITIES_WIDGETS_GEOMETRY_TEXT_H_
#define __UNOTU_ENTITIES_WIDGETS_GEOMETRY_TEXT_H_

#include <unotui\entities\widgets\geometry\texture.h>

#include <vector>

#include <unotui\utility\types.h>
#include <unotui\utility\colors.h>
#include <unotui\utility\text.h>

namespace unotui {

/**
 *  @brief Structure used to store color changes inside 'text_geometry'.
 */
struct tg_color_change
{
        tg_color_change() = default;
        tg_color_change( std::size_t Index, rgba Color )
        {
                this->Index = Index;
                this->Color = Color;
        }
        tg_color_change( std::size_t Index, rgba Color, rgba BackgroundColor )
        {
                this->Index = Index;
                this->Color = Color;
                this->BackgroundColor = BackgroundColor;
        }
        std::size_t Index = 0;
        
        rgba Color           = color::black;
        rgba BackgroundColor = color::transparent;
};

/**
 *  @brief Used to describe color changes when calling 'text_geometry::AddText()'.
 */
struct color_change
{
        color_change() = default;
        color_change( text_coord Position, rgba Color )
        {
                this->Position = Position;
                this->Color = Color;
        }
        color_change( text_coord Position, rgba Color, rgba BackgroundColor )
        {
                this->Position = Position;
                this->Color = Color;
                this->BackgroundColor = BackgroundColor;
        }
        text_coord Position = {0, 0};
        rgba Color           = color::black;
        rgba BackgroundColor = color::transparent;
};

struct text_geometry : public texture_geometry
{
        std::vector< tg_color_change > ColorChanges;
        
        text_geometry( GLenum Usage = GL_STATIC_DRAW );
        virtual void Draw(void);
        virtual void Clear(void);
        
        void AddText(
                const std::string& Text,
                unit FontSize,
                fpoint Origin,
                rgba Color = color::black,
                float TopCut = 0.0f,
                float BottomCut = 0.0f
        );
        
        void AddText(
                const std::string& Text,
                std::vector<split_line> Lines,
                unit FontSize,
                fpoint Origin,
                rgba Color = color::black,
                float TopCut = 0.0f,
                float BottomCut = 0.0f
        );
        
        void AddText(
                const std::string& Text,
                std::vector<split_line> Lines,
                unit FontSize,
                fpoint Origin,
                std::vector<color_change> Colors = {{{0, 0}, color::black}},
                float TopCut = 0.0f,
                float BottomCut = 0.0f
        );
        
        //:: Internal helper functions.
        
        void DrawChangeColor( const rgba Text, const rgba Background );
        
};

} // namespace unotui

#endif
