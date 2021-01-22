#include <unotui\entities\ent_tab.h>

namespace unotui {

std::weak_ptr<widget> ent_tab::AddWidget( widget* const widget_ )
{
        const std::shared_ptr<widget> SharedPointer =  std::shared_ptr<widget>( widget_ );
        this->Widgets.push_back( SharedPointer );;
        widget_->bConstructed = true;
        widget_->PostConstruct();
        return std::weak_ptr<widget>( SharedPointer );
}

void ent_tab::RemoveWidget( widget* const Widget )
{
        for( std::size_t i = this->Widgets.size(); i != 0;) {
                i--;
                if( this->Widgets[i].get() == Widget ) {
                        this->Widgets.erase( this->Widgets.begin()+i );
                        return;
                }
        }
}

void ent_tab::SwitchTab( w_tab* Tab )
{
        if( this->PendingTab ){
                delete this->PendingTab;
        }
        
        this->PendingTab = Tab;
        
}

/** @brief Immediately switches to pending tab.
 *  @warning Can introduce problems depening on timing of the call.
 *           Should only called when current window is this tab's window.
 */
void ent_tab::SwitchTabToPending()
{
        if( this->PendingTab ) {
                printf( "Switch to '%s'!\n", ClassName( *this->PendingTab ).c_str() );
                this->Widgets.clear();
                this->AddWidget( new w_overlay() );
                this->AddWidget( this->PendingTab );
                this->PendingTab = nullptr;
        }
}

std::shared_ptr<widget>& ent_tab::operator[]( std::size_t index )
{
	return this->Widgets[index];
}

std::shared_ptr<w_overlay> ent_tab::OverlayGet()
{
        
        for( std::shared_ptr<widget> Widget : this->Widgets ) {
                std::shared_ptr<w_overlay> Cast = std::dynamic_pointer_cast<w_overlay>(Widget);
                if( Cast ) {
                        return Cast;
                }
        }
        
        return nullptr;
        
}

} // namespace unotui
