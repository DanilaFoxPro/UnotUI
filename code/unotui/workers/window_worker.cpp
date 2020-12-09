#include <unotui\workers\window_worker.h>

#include <unotui\entities\ent_application.h>
#include <unotui\entities\ent_window.h>
#include <unotui\systems\systems.h>
#include <unotui\systems\callbacks.h>
#include <unotui\workers\widget_worker.h>
#include <unotui\utility\packed_data.h>
#include <unotui\utility\general.h>

#include <stdio.h> //TODO: DEBUG.
#include <system_error> // For throwing exeptions around.

namespace unotui {

/** Doesn't create the window right away, but schedules its
*  creation to an appropriate time.
*/
void CreateWindow()
{
        pending_window PendingWindow;
        TheWindowManager.PendingWindows.push_back( PendingWindow );
}

void CreatePendingWindows()
{
        //:: Pending windows.
        for( auto Pending : TheWindowManager.PendingWindows ){
                CreateWindowImmediately();
        }
        TheWindowManager.PendingWindows.clear();
}

/** Shouldn't be called directly, since it creates the window right away.
*  Use CreateWindow() instead.
* 
*  @note Sets the newly created window as current.
*/
void CreateWindowImmediately()
{
        
        TheWindowManager.Windows.push_back( ent_window() );
        TheWindowManager.SetCurrent( &TheWindowManager.Last() );
        ent_window& TheWindow = TheWindowManager.Cur();
        
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint( GLFW_VISIBLE, GLFW_TRUE );
        
        //                                   Width Height Title                    Monitor  Context to share
        TheWindow.Reference = glfwCreateWindow(800, 600, "[Default window title]", nullptr, TheWindowManager.GhostWindow);
        
        if( TheWindow.Reference == nullptr ) {
                throw std::system_error(EPERM, std::generic_category(), "Couldn't create a window.");
        }
        
        const auto& Layers = TheWindowManager.IconLayers;
        if( Layers.size() ) {
                glfwSetWindowIcon( TheWindow.Reference, Layers.size(), Layers.data() );
        }
        
        // This makes sure the OpenGL context is set. Since the context didn't exist
        // when 'SetCurrent()' was first called here.
        TheWindowManager.SetCurrent( &TheWindow );
        
        //:: Fill my wrapper.

        TheWindow.Width = TheWindow.Height = 0;
        TheWindow.MouseButtonsState = std::vector<bool>( TheWindowManager.ButtonsToCheck.size(), false );
        
        //:: Callbacks.
        
        glfwSetCharCallback( TheWindow.Reference, CallbackCharacter );
        glfwSetKeyCallback( TheWindow.Reference, CallbackKey );
        glfwSetScrollCallback( TheWindow.Reference, CallbackScroll );
        
        //:: OpenGL stuff.
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //Wireframe mode.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        //:: Interface callback.
        
        if( TheApplication.Interface ) TheApplication.Interface->OnNewWindow( TheWindow );
        
}

void UpdateWindows()
{
        if( TheWindowManager.Windows.size() == 0 )
        {
                // I'm not sure if it was causing a crash.
                return;
        }
        
        for( std::size_t i = TheWindowManager.Windows.size()-1; i != std::size_t(-1); i-- )
        {
                TheWindowManager.SetCurrent(i);
                ent_window& TheWindow = TheWindowManager.Cur();
                
                //:: Close check.
                if( glfwWindowShouldClose(TheWindow.Reference) )
                {
                        printf(
                                "    Destroying window index '%i' (of %i)..",
                               (int)i, (int)TheWindowManager.Windows.size()-1
                        );
                        glfwDestroyWindow( TheWindow.Reference );
                        printf( " Removing from window manager.." );
                        TheWindowManager.Windows.erase( TheWindowManager.Windows.begin()+i );
                        printf( " Continue..\n" );
                        continue;
                }
                
                //:: Make sure it's visible.
                const bool bWindowVisible = !glfwGetWindowAttrib(TheWindow.Reference, GLFW_ICONIFIED);
                if( !bWindowVisible )
                {
                        continue;
                }
                
                //::Resize check and handling.
                {
                        int Width, Height;
                        
                        glfwGetFramebufferSize( TheWindow.Reference, &Width, &Height );
                        
                        if( ( Width != TheWindow.Width ) or ( Height != TheWindow.Height ) )
                        {
                                glViewport(0, 0, Width, Height);
                
                                TheWindow.Width = Width;
                                TheWindow.Height = Height;
                                TheWindow.ScaleReference = ( Width + Height ) / 2;
                                TheWindow.PixelSize = point( pixel(1) );
                                
                                printf( "Window[%i] buffer resized to: %i %i\n", (int)i, TheWindow.x, TheWindow.y );
                                
                                // Widget geometry is no longer valid.
                                const int window_index = i;
                                InvalidateAllWidgets( window_index, ValidityState::Resized );
                        }
                }
                
        }//for each window.
                
}

void CheckMouseInput()
{
        for( std::size_t i = 0; i < TheWindowManager.Windows.size(); i++ )
        {
                if( !TheWindowManager[i].Visible() ) {
                        continue;
                }
                CheckMouseInput( i );
        }
}
void CheckMouseInput( const std::size_t Window )
{
        TheWindowManager.SetCurrent(Window);
        point MousePos = MousePosition();
        
        auto Colliding = CollisionTrace( MousePos );
        
        //:: Tell the widgets if the mouse is over them.
        mouseMoveTrace( MousePos, Colliding );

        //:: Scroll.
        ScrollTrace( Colliding );
        
        //:: Check mouse buttons.
        for( std::size_t i = 0; i < TheWindowManager.ButtonsToCheck.size(); i++ ) {
                
                ent_window& the_window = TheWindowManager.Cur();
                auto& Button = TheWindowManager.ButtonsToCheck[i];
                bool bPressed = glfwGetMouseButton(the_window.Reference, Button) == GLFW_PRESS;
                
                if( bPressed != the_window.MouseButtonsState[i] ) {
                        if(bPressed){
                                mouseButtonTrace( Button, Colliding );
                        } else {
                                mouseButtonRelease( Button );
                        }
                }
                
                the_window.MouseButtonsState[i] = bPressed;
        }
}

bool AnyWindowVisible()
{

        for( ent_window& Window : TheWindowManager.Windows ) {
                if( Window.Visible() ) {
                        return true;
                }
        }

        return false;

}

point MousePosition( const ent_window& Window )
{
        
        std::pair<double, double> Position;
        glfwGetCursorPos(Window.Reference, &Position.first, &Position.second);
        
        return point( ratio( Position.first / (double)Window.x ), ratio(1.0f)-ratio( Position.second / (double)Window.y ) );
}

int KeyState( const int Key )
{
        return glfwGetKey( TheWindowManager.Cur().Reference, Key );
}

bool IsKeyPressed( const int Key )
{
        return KeyState( Key ) == GLFW_PRESS;
}

} // namespace unotui
