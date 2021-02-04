#ifndef __UNOTUI_ENTITIES_INTERFACE_ENT_INTERFACE_H_
#define __UNOTUI_ENTITIES_INTERFACE_ENT_INTERFACE_H_

#include <unotui\entities\ent_window.h>

namespace unotui {

/** @brief Public interface of the library, with functions to be called when certain events occur.
 *  
 *  Users are expected to create a derived class and override the functions, then register the interface via
 *  `RegisterInterface()`.
 *  @see RegisterInterface()
 */
struct ent_interface
{
        virtual ~ent_interface() = default;
        
        //
        
//      virtual void PostSetup();
//      
//      virtual void OnCycle();
//      virtual void OnCycleWindow();
//      virtual void OnCycleInput();
//      virtual void OnCycleInterface();
//      virtual void OnCycleRender();
//      
//      virtual void OnExit();
//
//      virtual void OnNewWindow( ent_window& Window );
        
        //
        
                /** @brief Called when Unotui setup is finished.
                 * 
                 *  Good for setting up your own things, considering both GLFW and GLEW
                 *  are initialized at this point.
                 */
        virtual void PostSetup() {};
        
                /** @brief Called at each cycle start.
                 * 
                 *  Since Unotui is not currently multithreaded, this is both
                 *  a beginning of a logic tick, and a frame start.
                 */
        virtual void OnCycle() {};
                /** Called at window cycle stage. */
        virtual void OnCycleWindow() {};
                /** Called at input cycle stage. */
        virtual void OnCycleInput() {};
                /** @brief Called at interface cycle stage.
                 *  @note  Right before widgets are refreshed.
                 */
        virtual void OnCycleInterface() {};
                /** Called at render cycle stage. */
        virtual void OnCycleRender() {};
                /** Called when terminating UnotUI. */
        virtual void OnExit() {};
        
                /** @brief Called for each new window.
                 *  A good opportunity to switch tab.
                 */
        virtual void OnNewWindow( ent_window& Window, void* Context ) {};
        
};

} // namespace unotui

#endif
