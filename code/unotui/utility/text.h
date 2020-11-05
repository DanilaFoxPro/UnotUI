#ifndef CODE_UTILITY_TEXT_H_
#define CODE_UTILITY_TEXT_H_

#include <unotui\utility\coordinates.h>
#include <unotui\utility\types.h>

#include <string>
#include <vector>
#include <climits>

/**
 * @brief Text cursor flash period.
 * The time it takes to complete the cycle.
 * So sum of visible and invisible period.
 */
const double CaretPeriod = 1.5f;

/** @brief Used to store lines for the text split system.
 *  @note  Limit is last character +1. (So it can represent empty lines.)
 */
struct split_line
{
        split_line() = default;
        split_line( std::size_t Begin, std::size_t Limit ) {
                this->Begin = Begin;
                this->Limit = Limit;
        }
        
        std::size_t Begin;
        std::size_t Limit;
        
        std::size_t Length() const {
                return this->Limit - this->Begin;
        }
        std::size_t End() const {
                return this->Limit-1;
        }
        std::size_t IsEmpty() const {
                return this->Limit >= this->Begin;
        }
        std::size_t operator[]( std::size_t Index ) const {
                return this->Begin+Index;
        }
        
};

std::string SplitText( std::string, std::size_t );
std::vector<split_line> SplitTextNew( const std::string&, std::size_t );

std::string CutText( const std::string&, const int& = INT_MAX, const int& = 0 );
std::vector<split_line> CutLines(
        const std::vector<split_line>& Lines,
        const std::size_t Limit = INT_MAX,
        const std::size_t Offset = 0
);

std::pair< float, float > TextCutsFromArea( const std::size_t LineCount, const float SpaceHeight, const double Offset );

text_coord ToTextCoord( const std::size_t StringCoord, const std::vector<split_line>& Lines );
text_coord GetMaxTextCoord( const std::vector<split_line>& Lines );

size_t
ToSingleCoord(
        const std::pair< size_t, size_t >& Coord,
        const std::vector<split_line>& Lines
);
std::pair< size_t, size_t >
ToDoubleCoord(
        const std::size_t Coord,
        const std::vector<split_line>& Lines
);

size_t ToStringCoord( const std::pair< size_t, size_t >& LineCoord, const std::vector<split_line>& Lines );
size_t GetMaxStringCoord( const std::vector<split_line>& Lines );

line_coord GetLineBeginning( line_coord, const std::string& );
line_coord GetLineEnd( line_coord, const std::string& );

line_coord YMoveLineCoord( line_coord, const std::ptrdiff_t, const std::string& );

text_coord VerticallyOffsetTextCoord( const text_coord& TextCoord, const std::ptrdiff_t Offset );

std::string AssembleText( const std::string& Text, const std::vector<split_line>& Lines );
std::pair< std::size_t, std::size_t > GetCharacterCoordinates( const std::string&, std::size_t );
std::size_t GetCharacterPosition( const std::string&, std::size_t, std::size_t );

rectangle_tc CharacterTextureCoordinates( const char& character );
std::vector<textured_rectangle> StringToRectangles
(
	std::string Text,
	int FontSize,
	point Origin,
	float TopCut = 0.0f,
	float BottomCut = 0.0f
);

std::string ToLower( const std::string& );
std::string ToUpper( const std::string& );

std::size_t CountCharacter( const std::string& String, const char Character );
std::size_t CountCharacterBefore( const std::string& String, const char Character, const std::size_t Length );
char* ZeroTerminate( const char*, const int );
std::size_t LongestLine( const std::string& String );

#endif /* CODE_UTILITY_TEXT_H_ */
