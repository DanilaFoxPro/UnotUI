#ifndef __WINDOW_WORKER_H_
#define __WINDOW_WORKER_H_

#include <string>

#include <unotui\utility\coordinates.h>
#include <unotui\entities\ent_window.h>
#include <unotui\entities\widgets\w_tab.h>

namespace unotui {

void CreateWindow( void* const Context = nullptr );
void CreatePendingWindows();
void CreateWindowImmediately( const pending_window& Pending );
void UpdateWindows();
void CheckMouseInput();
void CheckMouseInput( const std::size_t window );

bool AnyWindowVisible();

point MousePosition( const ent_window& Window = TheWindowManager.Cur() );
int KeyState( const int Key );
bool IsKeyPressed( const int Key );

} // namespace unotui

#endif
