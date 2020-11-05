#ifndef __UNOTU_ENTITIES_ENT_APPLICATION_H_
#define __UNOTU_ENTITIES_ENT_APPLICATION_H_

#include <string>

#include <unotui\utility\classes\simple_timer.h>

namespace unotui {

struct ent_application
{
        const std::string Name = "Unotu";
          /** Timer that counts the time it takes to render a frame. */
        simple_timer FrameTimer;
};

extern ent_application TheApplication;

} // namespace unotui

#endif
