#include <unotui/workers/widget_worker.h>

#include <unotui/entities/ent_tab.h>
#include <unotui/entities/ent_opengl.h>

#include <cassert>
#include <stdio.h>//TODO: DEBUG.

#include <unotui/entities/ent_window.h>
#include <unotui/workers/window_worker.h>
#include <unotui/utility/shortcuts.h>
#include <unotui/utility/widget.h>

namespace unotui {

//:: Interface actions.

void ProcessWidgetInput()
{

        if( !TheWindowManager.SetCurrent( TheWindowManager.Focused() ) ) {
                return;
        }
        
        ent_window& TheWindow = TheWindowManager.Cur();
        
        TheWindow.PendingInput.Swap();
        TheWindow.PendingKeys.Swap();

        std::string& Input = TheWindow.PendingInput.ReadBuffer();
        std::vector<key_item>& KeyInput = TheWindow.PendingKeys.ReadBuffer();

        std::weak_ptr<widget> WeakWidget = GetKeyboardFocus( TheWindow );
        if( !WeakWidget.expired() ) {
                std::shared_ptr<widget> Widget = WeakWidget.lock();
                if( Input.size() ) {
                        Widget->OnCharacterInput( Input );
                }
                if( KeyInput.size() ) {
                        for( key_item& Key : KeyInput )
                        {
                                if( Key.Key == GLFW_KEY_ESCAPE && Widget->boLoseFocusOnESC )
                                {
                                        ResetKeyboardFocus();
                                        break;
                                }
                                Widget->OnKeyInput( Key.Action, Key.Key, Key.Modifier );
                        }
                }
        }

        Input.clear();
        KeyInput.clear();
        
}

void UpdateWidgetHints()
{
        for( std::size_t i = 0; i < TheWindowManager.Windows.size(); i++ ) {
                UpdateWidgetHint( TheWindowManager[i] );
        }
}


void UpdateWidgetHint( ent_window& TheWindow )
{
        ent_tab& TheTab = TheWindow.Tab;
        point MousePos = MousePosition(TheWindow);
        const std::shared_ptr<widget> HintedWidget = HintCollisionTrace( MousePos, TheTab.Widgets );
        
        const bool Changed = HintedWidget != TheTab.LastHint;
        
        if( Changed && TheTab.LastHint ) {
                TheTab.LastHint->bsHintFocused = false;
                TheTab.LastHint->OnHintLeave();
        }
        
        TheTab.LastHint = HintedWidget;
        
        if( Changed && HintedWidget ) {
                HintedWidget->bsHintFocused = true;
                HintedWidget->OnHintEnter();
        }
        
}


void TickWidgets()
{
        for( std::size_t i = 0; i < TheWindowManager.Windows.size(); i++ ) {
                ent_tab& TheTab = TheWindowManager[i].Tab;
                TickWidgets( TheTab.Widgets );
        }
}


/**
* @brief Validates all widgets in all windows.
* 
* @note Sorts children if any of them were invalid.
*       Same with widgets contained in currently active tab.
*/
void RefreshWidgets()
{
        for( std::size_t i = 0; i < TheWindowManager.Windows.size(); i++ ) {
                TheWindowManager.SetCurrent(i);
                ent_tab& the_interface = TheWindowManager.Cur().Tab;
                RefreshWidgets( the_interface.Widgets );
        }
}

/**
* @brief Make all widgets update geometry on next frame.
* @param Window Window to invalidate widgets in.
* 
* @note Used when window is resized, for example.
*/
void InvalidateAllWidgets( const std::size_t Window, const ValidityState_t Reason )
{
        
        ent_tab const& the_tab = TheWindowManager[Window].Tab;
        return InvalidateWidgets( the_tab.Widgets, Reason );
        
}

void SwitchTabs()
{
        for( std::size_t i = 0; i < TheWindowManager.Windows.size(); i++ ) {
                ent_tab& TheTab = TheWindowManager[i].Tab;
                
                // Switch to pending tab.
                TheTab.SwitchTabToPending();
                
        }
}

void DrawWidgets()
{
        for( std::size_t i = 0; i < TheWindowManager.Windows.size(); i++ )
        {
                ent_window& Window = TheWindowManager[i];
                const bool bWindowVisible = !glfwGetWindowAttrib(Window.Reference, GLFW_ICONIFIED);
                if( bWindowVisible ){
                        DrawWidgets( i );
                        Window.CurrentFrame++;
                }// If visible.
                
        }// For each window.
}

void DrawWidgets( const std::size_t& WindowIndex )
{
        
        TheWindowManager.SetCurrent(WindowIndex);
        
        ent_window& TheWindow = TheWindowManager.Cur();
        ent_tab& TheInterface = TheWindow.Tab;
        
        w_tab* TheTab = nullptr;
        if( TheInterface.Widgets.size() > 0
                && ( TheTab = dynamic_cast<w_tab*>( TheInterface[0].get() ) ) ) {
                glClearColor(
                        TheTab->Background.r,
                        TheTab->Background.g,
                        TheTab->Background.b,
                        TheTab->Background.a
                );
        } else {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        DrawWidgets( TheInterface.Widgets );
        
        glfwSwapBuffers( TheWindow.Reference );
        
}

void SetKeyboardFocus( std::weak_ptr<widget> WeakWidget, ent_window& TheWindow )
{
        std::shared_ptr<widget> Widget = WeakWidget.lock();
        std::shared_ptr<widget> CurrentFocus = TheWindow.KeyboardFocus.lock();
        
        if( Widget == CurrentFocus )
        {
                return;
        }
        
        ResetKeyboardFocus( TheWindow );
        if( Widget )
        {
                
                if( !Widget->boKeyboardFocusable )
                {
                        return;
                }
                
                Widget->bsKeyboardFocused = true;
                Widget->OnKeyboardFocused();
                if( Widget->boInvalidateOnKeyboardFocus )
                {
                        Widget->Invalidate();
                }
                TheWindow.KeyboardFocus = Widget;
        }
}

void ResetKeyboardFocus( ent_window& TheWindow )
{
        if( !TheWindow.KeyboardFocus.expired() )
        {
                std::shared_ptr<widget> Widget = TheWindow.KeyboardFocus.lock();
                
                Widget->bsKeyboardFocused = false;
                Widget->OnLostKeyboardFocus();
                if( Widget->boInvalidateOnKeyboardFocus )
                {
                        Widget->Invalidate();
                }
                TheWindow.KeyboardFocus.reset();
        }
}

std::weak_ptr<widget> GetKeyboardFocus( ent_window& TheWindow )
{
        return TheWindow.KeyboardFocus;
}

//:: Search.

/**
*  @brief  Search for widget pointer in the current interface.
*  
*  Recursively.
* 
* */
std::weak_ptr<widget> WidgetByPointer( widget* const Widget )
{
        ent_tab& the_interface = TheWindowManager.Cur().Tab;
        
        return WidgetByPointer( the_interface.Widgets, Widget );
}

//:: Collision and input.

std::vector< std::weak_ptr<widget> > CollisionTrace( point Position )
{
        
        const ent_tab& the_interface = TheWindowManager.Cur().Tab;
        return CollisionTrace( Position, the_interface.Widgets );
        
}


void mouseMoveTrace( point Position, const std::vector< std::weak_ptr<widget> >& Colliding )
{
        
        const ent_tab& TheInterface = TheWindowManager.Cur().Tab;
        
        UpdateMouseOverState( Position, TheInterface.Widgets, Colliding );
}

void mouseButtonTrace( int Button, const std::vector< std::weak_ptr<widget> >& Colliding )
{
        ent_tab& the_interface = TheWindowManager.Cur().Tab;
        
        for( std::weak_ptr<widget> current : Colliding )
        {
                if( current.expired() ) {
                        continue;
                }
                
                const std::shared_ptr<widget> Locked = current.lock();
                
                Locked->OnMousePressed( Button );
                
                const bool bShouldFocus = (
                           Locked->boKeyboardFocusable
                        && !Locked->bsKeyboardFocused
                        && Button == GLFW_MOUSE_BUTTON_1
                );
                the_interface.MouseReleaseListeners.push_back( {current, Button, bShouldFocus} );
                
                if( bShouldFocus )
                {
                        SetKeyboardFocus( Locked );
                }
        }
        
}

void mouseButtonRelease( int Button )
{
        ent_tab& the_interface = TheWindowManager.Cur().Tab;
        
        auto& listeners = the_interface.MouseReleaseListeners;
        
        for( std::size_t i = listeners.size()-1; i != std::size_t(-1); i-- )
        {
                release_listener& Current = listeners[i];
                
                if( Current.Button == Button )
                {
                        if( !Current.Listener.expired() ) {
                                Current.Listener.lock()->OnMouseReleased( Button, Current.bFocusingPress );
                        }
                        
                        listeners.erase( listeners.begin()+i );
                }
        }
        
}

void ScrollTrace( const std::vector< std::weak_ptr<widget> >& Colliding )
{
        ent_window& TheWindow = TheWindowManager.Cur();
        auto& ScrollBuffer = TheWindow.PendingScroll;
        
        ScrollBuffer.Swap();
        double& Offset = ScrollBuffer.ReadBuffer();
        
        if( Offset == 0.0 ) {
                return;
        }
        
        for( std::size_t i = Colliding.size()-1; i != std::size_t(-1); i-- ) {
                
                auto Current = Colliding[i];
                std::shared_ptr<widget> Locked;
                
                if( (Locked = Current.lock()) && Locked->boAcceptExternalScroll ) {
                        const bool bControlPressed = (
                                glfwGetKey( TheWindow.Reference, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS
                                || glfwGetKey( TheWindow.Reference, GLFW_KEY_RIGHT_CONTROL ) == GLFW_PRESS
                        );
                        if( bControlPressed ) {
                                Locked->HandleEvent( std::make_shared<unotui::we_scrollpages>(Offset) );
                        } else {
                                Locked->HandleEvent( std::make_shared<unotui::we_scrolllines>(Offset) );
                        }
                        Offset = 0.0;
                        return;
                }
        }
}

} // namespace unotui
