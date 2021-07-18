#include <unotui/utility/debug.h>

#include <stdio.h>
#include <stdexcept>

namespace unotui {
        
        void FancyAssert(
                const bool Condition,
                const std::size_t Line,
                const std::string File,
                const std::string ConditionString,
                const std::string Message
        )
        {
                if( !Condition ) {
                        
                        const std::size_t StringSize = 512;
                        char* String = (char*)malloc( sizeof(*String)*StringSize );
                        
                        snprintf(
                                String,
                                StringSize,
                                "Assertion failed.\n"
                                "Where:     At line %i in file '%s'.\n"
                                "Message:   '%s'\n"
                                "Assertion: '%s'",
                                (int)Line,
                                File.c_str(),
                                Message.c_str(),
                                ConditionString.c_str()
                        );
                        
                        throw std::logic_error(
                                std::string( String )
                        );
                }
        }
        
} // namespace unotui
