#ifndef __UNOTUI_WORKERS_INTERFACE_WORKER_H_
#define __UNOTUI_WORKERS_INTERFACE_WORKER_H_

#include <memory>

#include <unotui\entities\interface\ent_interface.h>

namespace unotui {

/** @brief Registers an interface with event callbacks.
 *  @note  Overrides previously registered interface.
 */
void RegisterInterface( std::unique_ptr<ent_interface> Interface );

void RunUnotUI();

} // namespace unotui

#endif
