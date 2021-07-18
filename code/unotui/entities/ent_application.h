#ifndef __UNOTUI_ENTITIES_ENT_APPLICATION_H_
#define __UNOTUI_ENTITIES_ENT_APPLICATION_H_

#include <string>
#include <memory>

#include <unotui/utility/classes/simple_timer.h>
#include <unotui/entities/interface/ent_interface.h>

namespace unotui {

struct ent_application
{
                /** Timer that counts the time it takes to render a frame. */
        simple_timer FrameTimer;
                /** Bound interface, contains functions that should be called when certain events occur. */
        std::unique_ptr<ent_interface> Interface;
};

extern ent_application TheApplication;

} // namespace unotui

#endif
