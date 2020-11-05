#ifndef __WINDOW_WORKER_H_
#define __WINDOW_WORKER_H_

#include <string>

#include <unotui\coordinates.h>
#include <unotui\entities\ent_window.h>
#include <unotui\entities\widgets\w_tab.h>

void CreatePendingWindow();
void CreateWindow();
void UpdateWindows();
void CheckMouseInput();
void CheckMouseInput( const std::size_t window );

bool AnyWindowVisible();

point MousePosition( const ent_window& Window = TheWindowManager.Cur() );

#endif
