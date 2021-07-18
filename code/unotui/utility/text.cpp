#include <unotui/utility/text.h>

#include <cassert>
#include <cmath>
#include <cstring>

#include <unotui/entities/ent_window.h>

namespace unotui {

std::vector<split_line> SplitText( const std::string& String, std::size_t WidthLimit )
{
        
        if( WidthLimit == 0 ) {
                return {};
        }
        
        //
        
        std::vector<split_line> Output;
        int LastSpace = -1;
        int LastSplit = -1;
        
        //
        
        for( std::size_t i = 0; i < String.size(); i++ ) {
                
                switch( String[i] ) {
                        case ' ': {
                                LastSpace = i;
                                break;
                        }
                        case '\n': {
                                Output.push_back( split_line( LastSplit+1, i ) );
                                LastSpace = i;
                                LastSplit = i;
                                break;
                        }
                        // There may be more things that affect the split in the future.
                }
                
                //
                
                const std::size_t SinceLastSplit = i-LastSplit;
                if( SinceLastSplit == WidthLimit ) {
                        
                        if( ( LastSpace == -1 ) or ( i-LastSpace >= WidthLimit ) ) {
                                // Split on i.
                                Output.push_back( split_line( LastSplit+1, i ) );
                                LastSplit = i;
                        } else {
                                // Split on last_space.
                                Output.push_back( split_line( LastSplit+1, LastSpace ) );
                                LastSplit = LastSpace;
                        }
                }
                
        }
        
        const std::size_t SinceLastSplit = String.size()-LastSplit;
        
        if( SinceLastSplit != 0 ) {
                Output.push_back( split_line( LastSplit+1, String.size() ) );
        }
        
        return Output;
        
}


std::string CutText( const std::string& string, const int& limit, const int& offset )
{
        if( limit < 1 ) {
                return "";
        } else if ( offset < 0 ) {
                return "";
        }
	
	int current_line = 0;
	int begin = -1;
	int end = -1;
	
	//TODO: May be broken.
	
	for( std::size_t i = 0; i <= string.size(); i++ )
	{
		
		if( current_line == offset
                        && begin == -1 ) {
			begin = i;
		} else if( current_line == offset+limit-1 // -1 due to limit starting at 1.
                        && end == -1 ) {
			end = i;
		}
		
		
		switch( string[i] ) {
			case '\n': {
				current_line++;
				break;
			}
		}
		
	}
	
	// Never reached the beginning line.
	if( begin == -1 )
	{
		return "";
	}
	
	// Never reached the end line.
	if( end == -1 )
	{
		return string.substr( begin );
	}
	
	// Both begin and end defined.
	return string.substr( begin, end-begin-1 );
	
}

/** @brief Get a subset of lines using offset and limit. */
std::vector<split_line> CutLines( const std::vector<split_line>& Lines, const size_t Limit, const size_t Offset )
{
        if( Offset >= Lines.size() ) {
                return {};
        }
        
        std::vector<split_line> Output;
        
        const std::size_t AbsLimit = Offset+Limit;
        
        for( std::size_t i = Offset; i < AbsLimit && i < Lines.size(); i++ ) {
                Output.push_back( Lines[i] );
        }
        
        return Output;
        
}

/**
 * @brief Calculate top and bottom cut based on offset and space height.
 * @param SpaceHeight Height of text space, in characters.
 */
std::pair<float, float> TextCutsFromArea( const std::size_t LineCount, const float SpaceHeight, const double Offset )
{
        
        double Useless;
        const float TopCut    = modf( Offset, &Useless );
        const float BottomCut = clamp( (float)modf( LineCount-Offset-SpaceHeight, &Useless ), 0.0f, 1.0f );
        
        return { TopCut, BottomCut };
        
}


/** @brief Convert position in the source string to line coordinate. */
text_coord ToTextCoord( const std::size_t StrCoord, const std::vector<split_line>& Lines )
{
        
        // TODO: May not work.
        
        for( std::size_t i = 0; i < Lines.size(); i++ ) {
                const split_line Line = Lines[i];
                if( Line.Begin <= StrCoord && Line.Limit >= StrCoord ) {
                        return std::make_pair( i, StrCoord-Line.Begin );
                }
        }
        
        if( Lines.size() != 0 ) {
                const std::size_t LastIndex = Lines.size()-1;
                const split_line Last = Lines[ LastIndex ];
                return std::make_pair( LastIndex, Last.Length() );
        } else {
                return std::make_pair( 0, 0 );
        }
        
}

text_coord GetMaxTextCoord(const std::vector<split_line>& Lines)
{
        if( Lines.size() == 0 ) {
                return {0, 0};
        }
        
        const std::size_t Last = Lines.size()-1;
        const split_line Line = Lines[Last];
        
        return {Last, Line.Length()};
        
}

/** @brief Double line coordinate to 1d line coordinate. */
size_t ToSingleCoord( const std::pair<size_t, size_t>& Coord, const std::vector<split_line>& Lines )
{
        
        // TODO: May not work.
        
        std::size_t Offset = 0;
        for( std::size_t i = 0; i < Lines.size(); i++ ) {
                
                const split_line Line = Lines[i];
                
                if( Coord.first == i ) {
                        return Offset+Coord.second;
                }
                
                Offset += Line.Length();
        }
        return Offset;
}

/** @brief 1d line coordinate to double line coordinate. */
std::pair<size_t, size_t> ToDoubleCoord( const std::size_t Coord, const std::vector<split_line>& Lines )
{
        
        // TODO: May not work.
        
        std::size_t Offset = 0;
        for( std::size_t i = 0; i < Lines.size(); i++ ) {
                
                const split_line Line = Lines[i];
                
                if( Coord > Offset && Coord < Offset+Line.Length() ) {
                        return {i, Coord-Offset};
                }
                
                Offset += Line.Length();
        }
        return {0, 0};
}

size_t ToStringCoord( const std::pair<size_t, size_t>& LineCoord, const std::vector<split_line>& Lines )
{
        // TODO: May not work.
        
        if( LineCoord.first >= Lines.size() ) {
                return 0;
        }
        
        for( std::size_t i = 0; i < Lines.size(); i++ ) {
                const split_line Line = Lines[i];
                if( LineCoord.first == i ) {
                        return Line.Begin+LineCoord.second;
                }
        }
        
        // Never reaches that, but compiler complains.
        return 0;
        
}

line_coord GetLineBeginning( line_coord LineCoord, const std::string& Text )
{
        while( LineCoord != 0 && Text[LineCoord-1] != '\n' ) {
                LineCoord--;
        }
        
        return LineCoord;
}

line_coord GetLineEnd( line_coord LineCoord, const std::string& Text )
{
        while( LineCoord != Text.size() && Text[LineCoord] != '\n' ) {
                LineCoord++;
        }
        
        return LineCoord;
}

size_t GetMaxStringCoord( const std::vector<split_line>& Lines )
{
        // TODO: May not work.
        size_t Offset = 0;
        for( split_line Line : Lines ) {
                Offset += Line.Length();
        }
        return Offset;
}

line_coord YMoveLineCoord( line_coord LineCoord, const std::ptrdiff_t Offset, const std::string& Text )
{
        
        std::size_t Line = CountCharacterBefore( Text, '\n', LineCoord ) + 1;
        
        const std::size_t LastLine   = CountCharacter( Text, '\n' ) + 1;
        const std::size_t TargetLine = clamp(
                (std::ptrdiff_t)Line+Offset,
                (std::ptrdiff_t)0,
                (std::ptrdiff_t)LastLine
        );
        
        const std::size_t LineOffset = LineCoord-GetLineBeginning( LineCoord, Text );
        
        if( TargetLine < Line ) {
                while( TargetLine != Line ) {
                        LineCoord = GetLineBeginning( LineCoord, Text )-1;
                        Line--;
                }
        } else if( TargetLine > Line ) {
                while( TargetLine != Line ) {
                        LineCoord = GetLineEnd( LineCoord, Text )+1;
                        Line++;
                }
        }
        
        const std::size_t Limit          = GetLineEnd( LineCoord, Text );
        const std::size_t UnclampedCaret = GetLineBeginning( LineCoord, Text ) + LineOffset;
        
        LineCoord = clamp( UnclampedCaret, (size_t)0, Limit );
        
        // Just to be sure.
        LineCoord = clamp( LineCoord, (size_t)0, Text.size() );
        
        return LineCoord;
        
}


/**
 * @brief Vertically offsets text coordinate by offset specified.
 * @note Clamps the result, so it cannot underflow. Clamped output is always (0, 0).
 */
text_coord VerticallyOffsetTextCoord( const text_coord& TextCoord, const std::ptrdiff_t Offset )
{
        if( (std::ptrdiff_t)TextCoord.first < -Offset ) {
                return text_coord( 0, 0 );
        } else {
                return text_coord( TextCoord.first+Offset, TextCoord.second );
        }
}


std::string AssembleText( const std::string& Text, const std::vector<split_line>& Lines )
{
        std::string Output;
        
        for( std::size_t i = 0; i < Lines.size(); i++ ) {
                const split_line& Line = Lines[i];
                Output += Text.substr( Line.Begin, Line.Length() );
                if( i != Lines.size()-1 ) {
                        Output.push_back( '\n' );
                }
        }
        
        return Output;
        
}


std::pair< std::size_t, std::size_t > GetCharacterCoordinates( const std::string& String, std::size_t Position )
{
	
	std::size_t LineBeginning = 0;
	std::size_t NewlineCount = 0;
	
	for( std::size_t i = 1; i <= Position && i <= String.size(); i++ )
	{
		if( String[i-1]=='\n' )
		{
			LineBeginning = i;
			NewlineCount++;
		}
	}
	
    return std::make_pair( NewlineCount, Position-LineBeginning );
	
}

std::size_t GetCharacterPosition( const std::string& String, std::size_t Line, std::size_t Character )
{
    std::size_t LineBeginning = 0;
	std::size_t NewlineCount = 0;
    std::size_t Position = 0;
	
    while( Position <= String.size() )
    {
        if( String[Position-1]=='\n' )
		{
			LineBeginning = Position;
			NewlineCount++;
		}
		
		if( Line == NewlineCount && Character == Position-LineBeginning )
        {
            return Position;
        }
        else if ( Line < NewlineCount )
        {
            return Position-1;
        }
		
        Position++;
    }
    
    return Position;
    
}


rectangle_tc CharacterTextureCoordinates( const char& character )
{
        const int index = (unsigned char)character;

        const float atlas_width  = 32.0f; // In characters.
        const float atlas_height = 16.0f; // In characters.

        const int column = index%32;
        const int row = index/32;
        const int irow = 16-row; //Inverse row.

        const float x1 = (float)(column)*(1.0f/atlas_width);
        const float y1 = (float)(irow)*(1.0f/atlas_height);

        const float x2 = (float)(column+1)*(1.0f/atlas_width);
        const float y2 = (float)(irow-1)*(1.0f/atlas_height);

        return rectangle_tc { x1, y1, x2, y2 };

}

std::vector<textured_rectangle> StringToRectangles
(
	std::string Text,
	int FontSize,
	point Origin,
	float TopCut,
	float BottomCut
)
{
	
        TopCut    = clamp( TopCut, 0.0f, 1.0f );
        BottomCut = clamp( BottomCut, 0.0f, 1.0f );
        
        const float InvTopCut = 1.0f-TopCut;
        const float InvBottomCut = 1.0f-BottomCut;
        
        std::vector<textured_rectangle> RectanglesTextured;
        
        ent_window& TheWindow = TheWindowManager.Cur();
        
        const float OriginX = Origin.x.ratio( TheWindow.Width );
        float OriginY = Origin.y.ratio( TheWindow.Height );
        
        const float FontHeight = pixel(FontSize).ratio( TheWindow.Height );
        const float FontWidth  = pixel(FontSize).ratio( TheWindow.Width );
        
        const int LastLine = CountCharacter( Text, '\n' );
        
        int RightShift = 0;
        int Line = 0;
        for( std::size_t i = 0; i < Text.length(); i++ ) {
                
                if( Text[i] == '\n' ) {
                        
                        if( Line == 0 ) {
                                OriginY -= FontHeight*InvTopCut;
                        } else {
                                OriginY -= FontHeight;
                        }
                        
                        RightShift = 0;
                        Line++;
                        continue;
                }
                
                const float Shift = (float)(RightShift++);
                rectangle_tc TexCoord = CharacterTextureCoordinates( Text[i] );
                const float TexCoordCharacterHeight = 1.0f/16.0f;
                
                if( Line == 0 && TopCut != 0.0f ) {
                        TexCoord.y1 -= TexCoordCharacterHeight*TopCut;
                        
                        RectanglesTextured.push_back(
                                textured_rectangle(
                                        OriginX+(FontWidth/2.0f*Shift),
                                        OriginY,
                                                                        
                                        OriginX+(FontWidth/2.0f*(Shift+1.0f)),
                                        OriginY-FontHeight*InvTopCut,
                                                                        
                                        TexCoord
                                )
                        );
                } else if( Line == LastLine && BottomCut != 0.0f ) {
                        
                        TexCoord.y2 += TexCoordCharacterHeight*BottomCut;
                        
                        RectanglesTextured.push_back(
                                textured_rectangle(
                                        OriginX+(FontWidth/2.0f*Shift),
                                        OriginY,
                                                                
                                        OriginX+(FontWidth/2.0f*(Shift+1.0f)),
                                        OriginY-FontHeight*InvBottomCut,
                                                                
                                        TexCoord
                                )
                        );
                } else {
                        
                        RectanglesTextured.push_back(
                                textured_rectangle(
                                        OriginX+(FontWidth/2.0f*Shift),
                                        OriginY,
                                                                                
                                        OriginX+(FontWidth/2.0f*(Shift+1.0f)),
                                        OriginY-FontHeight,
                                                                                
                                        TexCoord
                                )
                        );
                }
                
        }
        
        return RectanglesTextured;
                
}

std::string ToLower( const std::string& String )
{
        std::string Output;
        Output.reserve( String.size() );
        for( char Char : String ) {
                Output.push_back( tolower( Char ) );
        }
        return Output;
}

std::string ToUpper( const std::string& String )
{
        std::string Output;
        Output.reserve( String.size() );
        for( char Char : String ) {
                Output.push_back( toupper( Char ) );
        }
        return Output;
}


std::size_t CountCharacter( const std::string& String, const char Character )
{
        std::size_t Count = 0;
        for( char c : String ) {
                if( c == Character ) {
                        Count++;
                }
        }
        return Count;
}

std::size_t CountCharacterBefore(const std::string& String, const char Character, const std::size_t Length)
{
        std::size_t Count = 0;
        for( std::size_t i = 0; i < String.length() && i < Length; i++ ) {
                const char& CurrentCharacter = String[i];
                if( CurrentCharacter == Character ) {
                        Count++;
                }
        }
        return Count;
}


char* ZeroTerminate( const char* String, const int Length )
{
	char* Output = new char[Length+1];
	memcpy( Output, String, Length );
	Output[Length] = '\0';
	return Output;
}

std::size_t LongestLine( const std::string& String )
{
        std::size_t MaxStreak = 0;
        std::size_t CurrentStreak = 0;
        
        for( std::size_t i = 0; i <= String.size(); i++, CurrentStreak++ ) {
                if( String[i] == '\n' || i == String.size() ) {
                        if( CurrentStreak > MaxStreak ) {
                                MaxStreak = CurrentStreak;
                        }
                        CurrentStreak = -1;
                }
        }
        
        return MaxStreak;
        
}

std::size_t LongestLine( const std::vector<split_line>& Lines )
{
        std::size_t Longest = 0;
        for( const auto& Line : Lines ) {
                if( Line.Length() > Longest ) {
                        Longest = Line.Length();
                }
        }
        return Longest;
}

} // namespace unotui
