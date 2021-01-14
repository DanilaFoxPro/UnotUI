#include "w_text.h"

#include <unotui\utility\text.h>
#include <unotui\entities\ent_opengl.h>
#include <unotui\entities\ent_window.h>

namespace unotui {

void w_text::OnRefresh( ValidityState_t )
{

        Size = point( (int)Text.size()*FontSize/2, FontSize );

        gText.Clear();
        
        gText.AddText(
                this->Text,
                this->FontSize,
                this->Position,
                this->Color
        );

        gText.Update();

}

void w_text::OnDraw()
{
	
	this->gText.Draw();
	
}

} // namespace unotui
