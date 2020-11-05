#ifndef __UNOTU_UTILITY_CLIPBOARD_H_
#define __UNOTU_UTILITY_CLIPBOARD_H_

#include <string>

namespace unotui {

std::string GetClipboard();
void SetClipboard( const std::string& );

} // namespace unotui

#endif
