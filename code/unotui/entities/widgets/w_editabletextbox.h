#ifndef CODE_ENTITIES_WIDGETS_EDITABLETEXTBOX_H_
#define CODE_ENTITIES_WIDGETS_EDITABLETEXTBOX_H_

#include <unotui/entities/widgets/widget.h>
#include <unotui/entities/widgets/w_textscrollbox.h>
#include <unotui/entities/modules/m_caret.h>

namespace unotui {

struct w_editabletextbox : public w_textscrollbox, public m_caret
{
//:: Constructors.
	w_editabletextbox
	(
		const point& Position	= point( 0.0f, 1.0f ),
		const point& Size		= point( 1.0f, 1.0f ),
		
		const std::string&	Text		= "",
		const int&			FontSize	= 16,
		const rgba&			TextColor	= rgba( color::black, 1.0f ),
		
		const rgba& BackgroundColor   = rgba( color::gray*1.4f ), 
		const rgba& OutlineColor      = rgba( color::gray*0.5f ),
                const int&  OutlineThickness  = 2
	);
	virtual ~w_editabletextbox() = default; // Just to be safe.
	
//:: Data.
        
                /** Used by the mouse selection system. @see OnMousePressed() */
        bool bBegunClickSelection = false;
	
//:: Overrides.
	virtual void OnTick();
	virtual void OnRefresh( ValidityState_t );
	virtual void OnDraw();
	
	virtual void OnKeyboardFocused();
	
	virtual void OnCharacterInput( const std::string& ) final;
	virtual void OnKeyInput( const int&, const int&, const int& ) final;
        
        virtual void OnMousePressed( const int Button ) final;
        virtual void OnMouseReleased( const int Button, const bool bFocusingClick ) final;
	
//:: Geometry.
	text_geometry gText; // Only for a caret. What a waste.
};

} // namespace unotui

#endif
