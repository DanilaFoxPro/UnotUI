#include <unotui\entities\widgets\w_scrollbar.h>

#include <unotui\entities\ent_opengl.h>
#include <unotui\entities\ent_window.h>

#include <unotui\workers\widget_worker.h>
#include <unotui\workers\window_worker.h>
#include <unotui\utility\general.h>
#include <unotui\utility\text.h>
#include <unotui\utility\colors.h>
#include <unotui\utility\shortcuts.h>
#include <unotui\utility\widget.h>

#include <unotui\entities\events\widget_event.h>

#include <unotui\utility\font_symbols.h>

#include <cmath>
#include <stdio.h> //TODO: DEBUG.

namespace unotui {

void w_scrollbar::OnMousePressed( const int button )
{
        switch( button )
        {
                case GLFW_MOUSE_BUTTON_LEFT:
                {
                        this->bHasMouse = true;
                        break;
                }
        }
}

void w_scrollbar::OnMouseReleased( const int button, const bool )
{
        switch( button )
        {
                case GLFW_MOUSE_BUTTON_LEFT:
                {
                        this->bHasMouse = false;
                        break;
                }
        }
}

bool w_scrollbar::DoesHintCollide( point Position )
{
        const point HintMargin = pixel(32);
        const point Position2 = SecondPosition( this->Position, this->Size );
        const std::pair< point, point > Area = ApplyPadding( this->Position, Position2, -HintMargin );
        return DoesCollideRectangle( Position, Area.first, Area.second );
}

void w_scrollbar::OnTick()
{
        
        if( !this->bHasMouse ) {
                return;
        }
        
        this->ScrollOffsetSet( this->PositionAsScrollRatio( MousePosition().y ) * this->MaximumOffsetGet() );
        
}

void w_scrollbar::OnRefresh( ValidityState_t Reason )
{
        
        if( Reason & ValidityState::Resized ) {
                // Clamps offset to maximum.
                this->ScrollOffsetSet( this->ScrollOffsetGet() );
        }
        
        const rgb BackgroundColor = TheTheme.Background;
        const rgb ButtonColor = TheTheme.Primary2;
        
        const point Position2 = SecondPosition( Position, Size );

        const fpoint FPosition  = this->Position;
        const fpoint FPosition2 = Position2;
        
        const dpoint ButtonSize = dpoint(
                Size.xratio(),
                clamp(
                        (double)pixel( Size.x.xpixels() ).yratio(),
                        0.0,
                        Size.y.yratio()*0.5
                )
        );
        
        gColor.Clear();
        gText.Clear();
        gPreviewColor.Clear();
        
        // Don't draw anything if 'Size' isn't sane.
        if( Size.xpixels() <= 0 || Size.ypixels() <= 0 ) {
                gColor.Update();
                gText.Update();
                gPreviewColor.Update();
                return;
        }
        
        //:: Color.

        // Background.
        gColor.AddRectangle( colored_rectangle( Position, Position2, BackgroundColor ) );
        // Top button.
        gColor.AddRectangle(
                colored_rectangle(
                        Position,
                        point
                        (
                                Position2.x,
                                Position.y.yratio()-ratio(ButtonSize.y)
                        ),
                        ButtonColor
                )
        );
        // Bottom button.
        gColor.AddRectangle(
                colored_rectangle(
                        point
                        (
                                Position.x,
                                Position2.y.yratio()+ratio(ButtonSize.y)
                        ),
                        Position2,
                        ButtonColor
                )
        );
        // Scrollbar.
        {
                //TODO: Do something about this mess. D:
                
                const double ScrollSpace     = FPosition.y-FPosition2.y-ButtonSize.y*2;
                const double InputSpace      = ScrollSpace * MaximumOffsetRatioGet();
                
                const double ScrollbarHeight  = ScrollSpace * ViewzoneRatioGet();
                const double ScrollSpaceBegin = FPosition.y-ButtonSize.y;
                
                const double ScrollbarStart  = ScrollSpaceBegin - OffsetRatioGet() * InputSpace;
                const double ScrollbarEnd    = ScrollbarStart-ScrollbarHeight;

                gColor.AddRectangle
                (
                        colored_rectangle(
                                Position.x.xratio(),
                                ScrollbarStart,
                                
                                Position2.x.xratio(),
                                ScrollbarEnd,
                                
                                ButtonColor
                        )
                );
        }

        //:: Texture.
        // Top button.
        gText.AddText(
                std::string( 1, fsym::arrow_up ),
                ratio(ButtonSize.y).ypixels(),
                point (
                        (float)(Position.x.xratio()+ButtonSize.x*1.0/4.0),
                        Position.y
                ),
                color::white
        );
        // Bottom button.
        gText.AddText(
                std::string( 1, fsym::arrow_down ),
                ratio(ButtonSize.y).ypixels(),
                point (
                        (float)(Position.x.xratio()+ButtonSize.x*1.0/4.0),
                        Position2.y+ratio(ButtonSize.y)
                ),
                color::white
        );
        
        //:: Preview color.
        {
                
                const float PreviewOpacity = 0.9f;
                
                const float BeginX = Position2.x.xratio() - pixel(this->IdealPreviewWidth).xratio();
                const float EndX   = Position2.x.xratio();
                const float BeginY = Position.y.yratio();
                const float EndY   = Position2.y.yratio();
                
                const float SizeY       = Size.y.yratio();
                const float OffsetSizeY = SizeY*MaximumOffsetRatioGet();
                
                const float ScrollbarStart = BeginY - OffsetSizeY*OffsetRatioGet();
                const float ScrollbarEnd   = ScrollbarStart - SizeY*ViewzoneRatioGet();
                
                // Top background.
                gPreviewColor.AddRectangle(
                        colored_rectangle(
                                BeginX,
                                BeginY,
                                
                                EndX,
                                ScrollbarStart,
                                
                                rgba( BackgroundColor, PreviewOpacity )
                        )
                );
                
                // Viewzone.
                gPreviewColor.AddRectangle(
                        colored_rectangle(
                                BeginX,
                                ScrollbarStart,
                                EndX,
                                ScrollbarEnd,
                                rgba( ButtonColor, PreviewOpacity )
                        )
                );
                
                
                // Bottom background.
                gPreviewColor.AddRectangle(
                        colored_rectangle(
                                BeginX,
                                ScrollbarEnd,
                                EndX,
                                EndY,
                                rgba( BackgroundColor, PreviewOpacity )
                        )
                );
                
        }
        
        gColor.Update();
        gText.Update();
        gPreviewColor.Update();
}

void w_scrollbar::OnDraw()
{
        if( this->bHintFocused || this->bHasMouse ) {
                //:: Draw rectangles.
                this->gColor.Draw();
                
                //:: Draw text.
                this->gText.Draw();
        } else {
                this->gPreviewColor.Draw();
        }
}

/** Offset scrollbar offset by offset specified. */
void w_scrollbar::Offset( const double Offset )
{
        this->ScrollOffsetSet( ScrollOffsetGet()+Offset );
}

/** Offset scrollbar offset by viewzones specified. */
void w_scrollbar::OffsetByViewzone( double Ratio )
{
        this->ScrollOffsetSet( this->ScrollOffsetGet() + Ratio * this->ScrollViewzoneGet() );
        this->Invalidate( ValidityState::ParametersUpdated );
}

/** Offset scrollbar offset by a ratio of total scroll. */
void w_scrollbar::OffsetByRatio( double Ratio )
{
        this->ScrollOffsetSet( this->ScrollOffsetGet() + Ratio / this->MaximumOffsetGet() );
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_scrollbar::ScrollLengthSet( const double ScrollLength )
{
        this->ScrollLength = ScrollLength < 0.0 ? 0.0 : ScrollLength;
        this->ScrollOffset = clamp( this->ScrollOffset, 0.0, this->MaximumOffsetGet() );
        this->Invalidate( ValidityState::ParametersUpdated );
}

/** @note Offset gets clamped according to scroll length. */
void w_scrollbar::ScrollOffsetSet( const double ScrollOffset )
{
        this->ScrollOffset = clamp( ScrollOffset, 0.0, this->MaximumOffsetGet() );
        this->Invalidate( ValidityState::ParametersUpdated );
        this->ThrowEvent (
                std::shared_ptr<widget_event> (
                        new we_scrollsetratio( this->ScrollOffsetGet() )
                )
        );
}

void w_scrollbar::ScrollViewzoneSet( const double ScrollViewzone )
{
        this->ScrollViewzone = ScrollViewzone < 0.0 ? 0.0 : ScrollViewzone;
        this->Invalidate( ValidityState::ParametersUpdated );
}

double w_scrollbar::ScrollLengthGet() const
{
        return this->ScrollLength;
}

double w_scrollbar::ScrollOffsetGet() const
{
        return this->ScrollOffset;
}

double w_scrollbar::ScrollViewzoneGet() const
{
        if( this->ScrollViewzone > this->ScrollLengthGet() ) {
                return this->ScrollLengthGet();
        } else {
                return this->ScrollViewzone;
        }
}

/** Maximum offset possible before scrolling outside of the scroll range. */
double w_scrollbar::MaximumOffsetGet() const
{
        const double Result = ScrollLength-ScrollViewzoneGet();
        return Result < 0.0 ? 0.0 : Result;
}

/** Maximum offset as a ratio between maximum offset and scroll length. */
double w_scrollbar::MaximumOffsetRatioGet() const
{
        if( ScrollLengthGet() == 0.0 ) {
                return 0.0;
        } else {
                return (ScrollLengthGet()-ScrollViewzoneGet())/ScrollLengthGet();
        }
}

/** Viewzone as a ratio of scroll length. */
double w_scrollbar::ViewzoneRatioGet() const
{
        if( ScrollLengthGet() == 0.0 ) {
                return 0.0;
        } else {
                return ScrollViewzoneGet()/this->ScrollLengthGet();
        }
}

double w_scrollbar::OffsetRatioGet() const
{
        if( this->MaximumOffsetGet() == 0.0 ) {
                return 0.0;
        } else {
                return this->ScrollOffsetGet()/this->MaximumOffsetGet();
        }
}

/**
 *  @brief Y coordinate position mapped to scroll ratio.
 *  @note  Used for mouse input.
 */
double w_scrollbar::PositionAsScrollRatio( const unit ExternalPosition ) const
{
        const point Position2 = SecondPosition( Position, Size );
        
        const fpoint FPosition = this->Position;
        const fpoint FPosition2 = Position2;
        
        const double ButtonSize      = pixel(Size.x.xpixels()).yratio();
        
        const double ScrollSpace     = FPosition.y-FPosition2.y-ButtonSize*2.0;
        const double ScrollbarHeight = ScrollSpace * this->ViewzoneRatioGet();
        const double InputSpace      = ScrollSpace - ScrollbarHeight;
        
        // InputBegin - Starting point of the scrollbar input zone.
        // '-ScrollbarHeight/2' so the position corresponds to the middle of the scrollbar.
        //
        const double InputBegin  = FPosition.y-ButtonSize;
        const double LocalOffset = InputBegin-ExternalPosition.yratio()-ScrollbarHeight/2.0;
        
        
        
        return clamp( LocalOffset / InputSpace, 0.0, 1.0 );
}

} // namespace unotui
