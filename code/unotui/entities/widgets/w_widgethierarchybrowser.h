#ifndef __UNOTU_ENTITIES_WIDGETS_W_WIDGETHEIRARCHYBROWSER_H_
#define __UNOTU_ENTITIES_WIDGETS_W_WIDGETHEIRARCHYBROWSER_H_

#include <unotui\entities\widgets\widget.h>

namespace unotui {

struct w_widgethierarcybrowser : public widget
{
        w_widgethierarcybrowser();
        virtual ~w_widgethierarcybrowser();
        std::weak_ptr<widget> Root;
        std::weak_ptr<widget> Selected;
};

} // namespace unotui

#endif
