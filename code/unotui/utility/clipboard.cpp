#include <unotui\utility\clipboard.h>

#include <unotui\utility\deps\opengl_includes.h>

namespace unotui {

std::string GetClipboard()
{
        const char* UTF8_String = glfwGetClipboardString(nullptr);
        
        static_assert( sizeof(char) == sizeof(uint8_t), "Char is 8 bits long." );
        
        // Since UTF-8 is backward compatible with ASCII, just strip characters with 8th bit set. Which ASCII shouldn't use.
        for( std::size_t i = 0; UTF8_String[i] != 0; i++ ) {
                uint8_t& Current = (uint8_t&)UTF8_String[i];
                if( Current & 128 ) {
                        // Perhaps a custom character similar to 'fsym::arrow_up' should be added to denote missing characters.
                        Current = ' ';
                }
        }
        return std::string(UTF8_String);
}

void SetClipboard( const std::string& String )
{
        glfwSetClipboardString( nullptr, String.data() );
}

} // namespace unotui
