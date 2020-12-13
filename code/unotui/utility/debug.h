#ifndef __UNOTUI_CODE_UTILITY_DEBUG_H_
#define __UNOTUI_CODE_UTILITY_DEBUG_H_

#include <string>

#ifdef UNOTUI_INTERNAL
        #define UNOTUI_USE_UNOTUI_ASSERT
#endif

#ifdef UNOTUI_USE_UNOTUI_ASSERT
        #define assert( Condition, Message ) unotui::FancyAssert( Condition, __LINE__, __FILE__, #Condition, Message )
#endif

namespace unotui {
        
        void FancyAssert(
                const bool Condition,
                const std::size_t Line,
                const std::string File,
                const std::string ConditionString,
                const std::string Message = ""
        );
        
} // namespace unotui

#endif
