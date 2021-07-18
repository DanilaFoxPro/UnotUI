#ifndef __W_TABWIDGET_H_
#define __W_TABWIDGET_H_

#include <unotui/entities/widgets/widget.h>

#include <unotui/utility/colors.h>

namespace unotui {

struct w_tab : widget
{
	w_tab( const std::string& = "" );
        virtual ~w_tab() = default;
        
	std::string Title;
        rgba Background = color::white;
};

} // namespace unotui

#endif
