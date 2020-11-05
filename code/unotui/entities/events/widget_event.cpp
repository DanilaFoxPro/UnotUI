#include <unotui\entities\events\widget_event.h>

unotui::we_click::we_click( int ActionID, const std::string& ActionName, int Button )
{
        this->ActionID = ActionID;
        this->ActionName = ActionName;
        
        this->Button = Button;
}

unotui::we_scrollsetratio::we_scrollsetratio( float ratio_ )
{
	this->Ratio = ratio_;
}

unotui::we_scrolllines::we_scrolllines( float Lines )
{
        this->Lines = Lines;
}

unotui::we_scrollpages::we_scrollpages( float Pages )
{
        this->Pages = Pages;
}

unotui::we_textcommit::we_textcommit( std::string* const Text )
{
	this->Text = Text;
}

