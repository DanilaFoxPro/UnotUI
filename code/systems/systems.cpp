#include <systems\systems.h>

//::STD.
#include <thread>
#include <stdio.h>
#include <system_error> // For throwing exceptions around.
#include <cmath> // SUPER HOT
#include <cstdlib> // std::atexit

//::OpenGL.
#include <utility\deps\opengl_includes.h>

//::Mine.
#include <entities\ent_application.h>
#include <entities\ent_window.h>
#include <entities\ent_opengl.h>

#include <systems\callbacks.h>

#include <workers\widget_worker.h>
#include <workers\window_worker.h>

#include <utility\general.h>
#include <utility\text.h>
#include <utility\packed_data.h>

#include <coordinates.h> // For coordinates. Come on. (Unit, point.)
#include <entities\ent_tab.h>

namespace systems
{
        void entry();
        void setup(); // Not going to call it initialization. Keeping it simple. :D
                void setup_glfw();
                void setup_glew();
                void setup_loadtextures();
                void setup_shaders();
                void setup_variables();
                void setup_callbacks();
                void setup_window();
        void cycle();
                void cycle_window();
                void cycle_input();
                void cycle_interface();
                void cycle_render();
        void terminate();
        
        
        
        void entry()
        {
                
                printf( ":: -- Running setup... -- ::\n" );
                
                setup();
                
                printf( ":: -- Beginning main cycle... -- ::\n" );
                
                for(;;) { //Fine because 'terminate()' calls 'exit(0)'.
                        cycle();
                }
                
        }
        void setup()
        {
                setup_glfw();
                setup_glew();
                setup_loadtextures();
                setup_shaders();
                setup_variables();
                setup_callbacks();
                setup_window();
        
        }
        
        void setup_glfw()
        {
                // ==//==========================================\\==
                
                printf( "Initializing GLFW\n{" );
                
                // Initialize GLFW.
                glfwInit();
                
                // Version.
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                
                printf("}\n");
        }
        
        void setup_glew()
        {
                printf("Initializing GLEW\n{");
                
                printf( "  Creating ghost window...\n" );
                
                glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
                TheWindowManager.GhostWindow = glfwCreateWindow( 1, 1, "", nullptr, nullptr );
                glfwMakeContextCurrent( TheWindowManager.GhostWindow );
                
                if( TheWindowManager.GhostWindow == nullptr ) {
                        printf( "  [!] Creation of ghost window has failed.\n" );
                }
                
                glewExperimental = GL_TRUE;
                
                if( GLEW_OK != glewInit() ) {
                        glfwTerminate();
                        throw std::system_error(EPERM, std::generic_category(), "Failed to initilize GLEW");
                }
                
                printf("}\n");
        }
        
        void setup_loadtextures()
        {
                printf("Loading textures\n{\n");
                
                try {
                        the_opengl.FontTexture = loadTexture( DATA_CONSOLAS_FONT, DATA_CONSOLAS_FONT_SIZE, GL_RED, GL_RED );
                } catch( std::runtime_error &Exception ) {
                        printf( "  Error while loading textures: %s\n", Exception.what() );
                        printf( "  This is fatal, terminating..\n" );
                        terminate();
                }
                
                printf("}\n");
        }
        
        void setup_shaders()
        {
                printf("Loading and compiling shaders\n{\n");
                
                //::Load.
                
                        //Color shader source.
                        GLchar* vertexShaderSource_color        = ZeroTerminate( (char*)DATA_COLORED_VERTICES_VS, DATA_COLORED_VERTICES_VS_SIZE );
                        GLchar* fragmentShaderSource_color      = ZeroTerminate( (char*)DATA_COLORED_VERTICES_FRAG, DATA_COLORED_VERTICES_FRAG_SIZE );
                        
                        //Texture shader source.
                        GLchar* vertexShaderSource_texture      = ZeroTerminate( (char*)DATA_TEXTURED_VERTICES_VS, DATA_TEXTURED_VERTICES_VS_SIZE );
                        GLchar* fragmentShaderSource_texture    = ZeroTerminate( (char*)DATA_TEXTURED_VERTICES_FRAG, DATA_TEXTURED_VERTICES_FRAG_SIZE );
                        
                        //Text shader source.
                        GLchar* vertexShaderSource_text         = ZeroTerminate( (char*)DATA_TEXT_VS, DATA_TEXT_VS_SIZE );
                        GLchar* fragmentShaderSource_text       = ZeroTerminate( (char*)DATA_TEXT_FRAG, DATA_TEXT_FRAG_SIZE );
                
                //::Compile and link.
                        
                the_opengl.ColorShader          = compileAndLinkShaders( vertexShaderSource_color, fragmentShaderSource_color, "Color shader" );
                the_opengl.TextureShader        = compileAndLinkShaders( vertexShaderSource_texture, fragmentShaderSource_texture, "Texture shader" );
                the_opengl.TextShader           = compileAndLinkShaders( vertexShaderSource_text, fragmentShaderSource_text, "Text shader" );
                
                //::Bind textures.
                
                glUseProgram( the_opengl.TextureShader );
                glUniform1i( glGetUniformLocation( the_opengl.TextureShader, "daTexture" ), GL_TEXTURE0 );
                glUseProgram( the_opengl.TextShader );
                glUniform1i( glGetUniformLocation( the_opengl.TextureShader, "fontTexture" ), GL_TEXTURE0 );
                
                //::Done.
                
                delete[] vertexShaderSource_color;
                delete[] fragmentShaderSource_color;
                
                delete[] vertexShaderSource_texture;
                delete[] fragmentShaderSource_texture;
                
                delete[] vertexShaderSource_text;
                delete[] fragmentShaderSource_text;
                
                printf("}\n");
        }
        
        void setup_variables()
        {
                printf("Setting up variables\n{\n  Setup ma' ass.\n");
                
                //:: Extensions check.
                printf( "  Checking supported extensions..\n" );
                
                const std::vector<const char*> Extensions = {
                        "WGL_EXT_swap_control_tear",
                        "GLX_EXT_swap_control_tear"
                };
                
                for( std::size_t i = 0; i < Extensions.size(); i++ )
                {
                        bool Supported = glfwExtensionSupported( Extensions[i] ) == GLFW_TRUE;
                        printf( "    %s - %s.\n", Extensions[i], Supported ? "supported" : "not supported" );
                }
                
                // Vertical synchronization.
                glfwSwapInterval(0);
                
                printf( "}\n" );
                        
        }
        
        void setup_callbacks()
        {
                // Window callbacks are setup within 'CreateWindow()'.
                std::atexit( CallbackExit );
        }
        
        void setup_window()
        {
                printf("Creating window\n{\n");
                
                //:: Window.
                
                CreatePendingWindow();
                
                printf("}\n");
        }
        
        void cycle()
        {
                simple_timer& Timer = TheApplication.FrameTimer;
                Timer.Start();
                
                cycle_window(); // Window events, handling resizing..
                cycle_input(); // Input polling.
                cycle_interface(); // Interface updates.
                cycle_render(); // OpenGL rendering. Nothing more.
                
                //printf( "%i: %i\n", (int)TheWindowManager.Cur().CurrentFrame, (int)Timer.ElapsedTime().count() );
                std::this_thread::sleep_for( the_opengl.IdealFramePeriod-Timer.ElapsedTime() );
                
        }
        
        void cycle_window()
        {
                
                //:: Pending windows.
                for( auto Pending : TheWindowManager.PendingWindows ){
                        CreateWindow();
                }
                TheWindowManager.PendingWindows.clear();
                
                //::Window events.
                
                glfwPollEvents();
                UpdateWindows();
                
                if( !TheWindowManager.Windows.size() )
                {
                        printf( "  Terminating everything..\n" );
                        terminate();
                }
                
        }
        
        void cycle_input()
        {
                CheckMouseInput();
                ProcessWidgetInput();
        }
        
        void cycle_interface()
        {
                
                //:: Normal stuff.
                
                UpdateWidgetHints();
                TickWidgets();
                SwitchTabs();
                RefreshWidgets();
        
        }
        
        void cycle_render()
        {
                //:: Draw stuff.
                
                if( AnyWindowVisible() ) {
                        DrawWidgets();
                } else {
                        // Freeze everything and wait for events.
                        glfwWaitEvents();
                }
                
        }
        
        void terminate()
        {
                exit(0);
        }
}// Namepsace system.
