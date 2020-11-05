#include <unotui\entities\events\widget_event.h>

namespace unotui {

we_click::we_click( int ActionID, const std::string& ActionName, int Button )
{
        this->ActionID = ActionID;
        this->ActionName = ActionName;
        
        this->Button = Button;
}

we_scrollsetratio::we_scrollsetratio( float ratio_ )
{
	this->Ratio = ratio_;
}

we_scrolllines::we_scrolllines( float Lines )
{
        this->Lines = Lines;
}

we_scrollpages::we_scrollpages( float Pages )
{
        this->Pages = Pages;
}

we_textcommit::we_textcommit( std::string* const Text )
{
	this->Text = Text;
}

} // namespace unotui
