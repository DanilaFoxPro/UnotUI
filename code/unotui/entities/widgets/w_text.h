#ifndef __W_TEXT_H_
#define __W_TEXT_H_

#include <unotui\entities\widgets\widget.h>
#include <unotui\entities\ent_window.h> // Theme.
#include <unotui\utility\colors.h>
#include <stdio.h>

namespace unotui {

struct w_text : widget
{
	//Data.
	
	std::string Text;
	rgba Color;
	int FontSize;
	
	text_geometry gText;
	
	//Constructor.
	
	w_text(void)
	{
                Color    = TheTheme.Text;
                FontSize = 16;

                Position = point( 0, 0 );
                Size     = point( 16, 16 ); // Updated on 'UpdateGeometry()'.
                Layer    = 0;
	}
	w_text(
		point Position,
		std::string Text = "",
		int FontSize     = 32,
		int Layer        = 0,
		rgb Color        = TheTheme.Text,
		float Alpha      = 1.0f
	)
	{
                this->Text     = Text;
                this->Color    = rgba( Color, Alpha );
                this->FontSize = FontSize;
        
		// Size is updated on 'OnRefresh()'.
		this->Position = Position;
		this->Layer    = Layer;
	}
	
	//Functions.
	
	void OnDraw();
	
	void OnRefresh( ValidityState_t );
	
};

} // namespace unotui

#endif
