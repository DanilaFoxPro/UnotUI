#include <unotui\entities\widgets\geometry\text.h>

#include <unotui\entities\ent_opengl.h>
#include <unotui\utility\shortcuts.h>
#include <unotui\entities\ent_window.h>
#include <unotui\utility\debug.h>

namespace unotui {

text_geometry::text_geometry( GLenum Usage )
{
        this->Usage = Usage;
}

void text_geometry::DrawChangeColor( const rgba Text, const rgba Background )
{
        glUniform4f(
                glGetUniformLocation( the_opengl.TextShader, "fontColor" ),
                Text.r,
                Text.g,
                Text.b,
                Text.a
        );
        glUniform4f(
                glGetUniformLocation( the_opengl.TextShader, "fontBackgroundColor" ),
                Background.r,
                Background.g,
                Background.b,
                Background.a
        );
}

void text_geometry::Draw()
{
        if( this->EnableBlend ) {
                glEnable( GL_BLEND );
        }
        
        glUseProgram( the_opengl.TextShader );
        glBindVertexArray( this->VAO );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, the_opengl.FontTexture );
        
        // While 'AddText()' already has default color on 0th index,
        // I want to make sure the possibility of first color change
        // not being on index 0 is covered. (Future-proof, perhaps.)
        this->DrawChangeColor( color::black, color::transparent );
        
        std::size_t IndexOffset = 0;
        for( size_t i = 0; i < this->ColorChanges.size(); i++ ) {
                
                // The idea is pretty simple, use color changes to delimit
                // drawing calls. Change color, then draw everything
                // between current color change and the next one (or the
                // end of indices). Repeat.
                // 
                
                const auto Change = this->ColorChanges[i];
                
                size_t NextChangeIndex = this->Indices.size();
                if( i != LastIndex(this->ColorChanges) ) {
                        NextChangeIndex = this->ColorChanges[i+1].Index;
                }
                
                if( Change.Index == NextChangeIndex ) {
                        continue;
                }
                
                assert(
                        NextChangeIndex > Change.Index,
                        "Changes vector should be sorted. This assert will fail if "
                        "a color change with lower 'index' occurs later in the "
                        "vector, thus constituting a retroactive color change, "
                        "which isn't supported."
                );
                
                this->DrawChangeColor( Change.Color, Change.BackgroundColor );
                
                // Number of indices to draw this time.
                const size_t DrawCount = NextChangeIndex-Change.Index;
                
                glDrawElements(
                        GL_TRIANGLES,
                        DrawCount,
                        GL_UNSIGNED_INT,
                        (void*)(IndexOffset*sizeof(unsigned int))
                );
                
                IndexOffset += DrawCount;
        }

        if( this->EnableBlend ) {
                glDisable( GL_BLEND );
        }
}

void text_geometry::Clear()
{
        texture_geometry::Clear();
        this->ColorChanges.clear();
}

void text_geometry::AddText(
        const std::string& Text,
        unit FontSize,
        fpoint Origin,
        rgba Color,
        float TopCut,
        float BottomCut
)
{
        return this->AddText(
                Text,
                SplitText( Text, std::size_t(-1) ),
                FontSize,
                Origin,
                Color,
                TopCut,
                BottomCut
        );
}

void text_geometry::AddText(
        const std::string& Text,
        std::vector<split_line> Lines,
        unit FontSize,
        fpoint Origin,
        rgba Color,
        float TopCut,
        float BottomCut
)
{
        return this->AddText(
                Text,
                Lines,
                FontSize,
                Origin,
                {{{0, 0}, Color}},
                TopCut,
                BottomCut
        );
}

/**
 * @brief Bloated function for adding colored text.
 * 
 * @param Text     The source text.
 * @param Lines    Lines to be drawn, as references to source text.
 * @param FontSize Size of the font in pixels.
 * @param Origin   Origin point at which to start drawing the text.
 * @param Colors   Set of positions at which to change the text color.
 *                 
 * @param TopCut    How much to cut off of the top of the top line. As ratio.
 * @param BottomCut How much to cut off of the bottom of the bottom line. As ratio.
 */
void text_geometry::AddText(
        const std::string& Text,
        std::vector<split_line> Lines,
        unit FontSize,
        fpoint Origin,
        std::vector<color_change> Colors,
        float TopCut,
        float BottomCut
)
{
        
        /* TODO: Looks like a mess, make more readable.
         *       Perhaps split into multiple functions.
         */
        
        // Make sure to change text color to default (black). Otherwise
        // text color would remain the same as for the previously
        // rendered text, leading to possible inconsistency.
        // Clamping cuts, to be safe.
        // OriginY is decremented with each text line, and keeps track
        // of where the text should start vertically. OriginX
        // (constant) and RightShift (integer) are fulfilling the same
        // task for horizontal offset.
        // Text coloring is done by grouping into 'colored_patch'es,
        // see 'text_geometry::Draw()'.
        //
        
        if( Colors.size() == 0
            || Colors[0].Position != text_coord{0, 0} ) {
                Colors.insert( Colors.begin(), {{{0, 0}, color::black}} );
        }
        
        const float FontWidth  = FontSize.xratio();
        const float FontHeight = FontSize.yratio();
        
        TopCut    = clamp( TopCut, 0.0f, 1.0f );
        BottomCut = clamp( BottomCut, 0.0f, 1.0f );
        
        const float InvTopCut = 1.0f-TopCut;
        const float InvBottomCut = 1.0f-BottomCut;
        
        const float& OriginX = Origin.x;
              float& OriginY = Origin.y;
        
        const std::size_t LastLine = Lines.size()-1;
        std::size_t TotalCharactersAdded = 0;
        
        std::size_t CurColorChange = 0;
        std::size_t LastColorChangeIndex = this->ColorChanges.size() != 0 ? this->ColorChanges[this->ColorChanges.size()-1].Index : -1;
        
        for( std::size_t Line = 0; Line < Lines.size(); Line++ )
        {
                const split_line& SplitLine = Lines[Line];
                
                int RightShift = 0;
                for( std::size_t i = 0; i <= SplitLine.Length(); i++, TotalCharactersAdded++ ) {
                        
                        // Using a while loop because multiple color changes could be stacked
                        // on top of each other. In which case, only the last one should
                        // persist.
                        while( Colors[CurColorChange].Position == text_coord{ Line, i } ) {
                                
                                const color_change ColorChange  = Colors[CurColorChange];
                                const size_t       CurrentIndex = this->Indices.size();
                                
                                if( LastColorChangeIndex == CurrentIndex ) {
                                        this->ColorChanges[this->ColorChanges.size()-1].Color           = ColorChange.Color;
                                        this->ColorChanges[this->ColorChanges.size()-1].BackgroundColor = ColorChange.BackgroundColor;
                                } else if( this->ColorChanges.size() != 0
                                                && this->ColorChanges[CurColorChange-1].Color           == ColorChange.Color
                                                && this->ColorChanges[CurColorChange-1].BackgroundColor == ColorChange.BackgroundColor
                                ) {
                                        // Don't do anything, this color change isn't actually a change.
                                } else {
                                        this->ColorChanges.push_back(
                                                tg_colored_change(
                                                        CurrentIndex,
                                                        ColorChange.Color,
                                                        ColorChange.BackgroundColor
                                                )
                                        );
                                        LastColorChangeIndex = CurrentIndex;
                                }
                                
                                CurColorChange++;
                                
                        }
                        
                        const char& CurrentChar = Text[SplitLine[i]];
                        
                        if( CurrentChar == '\n' ) {
                                TotalCharactersAdded--;
                                continue;
                        }
                        
                        const float Shift = (float)(RightShift++);
                        rectangle_tc TexCoord = CharacterTextureCoordinates( CurrentChar );
                        const float TCCharHeight = 1.0f/16.0f;
                        float Y2 = OriginY-FontHeight;
                        
                        // Handle top and bottom cuts.
                        if( Line == 0 && TopCut != 0.0f ) {
                                TexCoord.y1 -= TCCharHeight*TopCut;
                                Y2 = OriginY-FontHeight*InvTopCut;
                        } else if( Line == LastLine && BottomCut != 0.0f ) {
                                TexCoord.y2 += TCCharHeight*BottomCut;
                                Y2 = OriginY-FontHeight*InvBottomCut;
                        }
                        
                        // Character rectangle.
                        const textured_rectangle Rect = textured_rectangle(
                                OriginX+(FontWidth/2.0f*Shift),
                                OriginY,
                                                                        
                                OriginX+(FontWidth/2.0f*(Shift+1.0f)),
                                Y2,
                                                                        
                                TexCoord
                        );
                        
                        this->AddRectangle( Rect );
                        
                }
                
                if( Line == 0 ) {
                        OriginY -= FontHeight*InvTopCut;
                } else {
                        OriginY -= FontHeight;
                }
        }
        
}

} // namespace unotui
