#include <unotui\workers\interface_worker.h>

#include <unotui\entities\ent_application.h>

namespace unotui {

void RegisterInterface( std::unique_ptr<ent_interface> Interface )
{
        TheApplication.Interface.swap( Interface );
}

} // namespace unotui
