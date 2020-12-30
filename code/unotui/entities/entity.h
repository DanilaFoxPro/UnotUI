#ifndef __UNOTUI_CODE_ENTITIES_ENTITY_H_
#define __UNOTUI_CODE_ENTITIES_ENTITY_H_

#include <cstdint>

#define UNOTUI_ENTITY(x) x : public unotui::entity<x>

namespace unotui
{

typedef std::uintptr_t entity_id;

template < class DERIVED >
struct entity
{
        static entity_id GetEntityIdentifier()
        {
                return reinterpret_cast<entity_id>(&GetEntityIdentifier);
        }
};

} // namespace unotui

#endif
