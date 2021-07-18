#ifndef __UNOTU_ENTITIES_WIDGETS_W_OVERLAY_H_
#define __UNOTU_ENTITIES_WIDGETS_W_OVERLAY_H_

#include <chrono>

#include <unotui/entities/widgets/widget.h>
#include <unotui/utility/classes/simple_timer.h>

namespace unotui {

struct w_overlay : widget
{
        
//:: Settings.
        bool bDisplayFPS = true;
        bool bDisplayCurrentWindow = false;
        bool bDisplayFrameCount = false;
        bool bDisplayMouse = true;
        
        enum {
                FPSAverageStep = 10,
                DefaultFontSize = 32,
        };
        
//:: Constructor.
        w_overlay()
        {
                this->Position = point( 0.0f, 1.0f );
                this->Size     = point( 1.0f, 1.0f );
                
                this->boSolid = false;
                this->Layer  = LayerReference::Frontmost;
                
                this->boInvalidateOnMouseOver = true;
                
                this->AverageFPS = 1.0/0.0;
                
        }
        
//:: Data.
        double AverageFPS;
        simple_timer FrameTimer;
        
//:: Geometry.
        color_geometry  gColor  = color_geometry(GL_STREAM_DRAW);
        text_geometry   gText   = text_geometry(GL_STREAM_DRAW);
        
//:: Functions.
        void CheckpointFrame();
        void CreateLabel(
                const std::string&      Label,
                const point             Origin,
                const int               FontSize = DefaultFontSize,
                const rgba              Color = rgba( 1.0f, 0.6f, 0.1f, 1.0f ),
                const rgba              BackgroundColor = rgba( color::black, 0.9f )
        );
        
//:: Overrides.
        
        void OnTick();
        void OnRefresh( ValidityState_t );
        void OnDraw();
};

} // namespace unotui

#endif
