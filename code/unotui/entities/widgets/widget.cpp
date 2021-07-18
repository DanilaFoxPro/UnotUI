#include <unotui/entities/widgets/widget.h>

#include <stdio.h>//TODO: DEBUG
#include <algorithm>// std::find.

#include <unotui/utility/widget.h>
#include <unotui/utility/shortcuts.h>
#include <unotui/workers/widget_worker.h>

namespace unotui {

////::Base

////::Widget Base

//:: Operators.

bool operator<( const widget& a, const widget& b )
{
	return a.Layer < b.Layer;
}

bool operator>=( const widget& a, const widget& b )
{
	return !( a < b );
}

bool operator>( const widget& a, const widget& b )
{
	return a.Layer > b.Layer;
}
bool operator<=( const widget& a, const widget& b )
{
	return !( a > b );
}

//:: Constructors.

widget::widget( const widget& Widget )
{
	this->SimpleCopy( Widget );
	if( !Widget.Parent.expired() )
	{
		const std::string Message =
				std::string("Const reference copy constructor of ") +
				ClassName(*this) +
				" invoked with a " +
				ClassName(*this) +
				" that has a parent. ";
		throw std::invalid_argument( Message.c_str() );
	}
	if( Widget.Children.size() != 0 )
	{
		const std::string Message =
				std::string("Const reference copy constructor of ") +
				ClassName(*this) +
				" invoked with a " +
				ClassName(*this) +
				" that has children.";
		throw std::invalid_argument( Message.c_str() );
	}
}

widget::widget( widget& Widget )
{
	this->SimpleCopy( Widget );
	
	if( Widget.Children.size() != 0 )
	{
		this->Children = Widget.Children;
		Widget.Children.clear();
	}
	
}

//:: Functions.

bool widget::DoesCollide( point Pos )
{
	point Position2 = SecondPosition( Position, Size );
	return DoesCollideRectangle( Pos, this->Position, Position2 );
}

bool widget::DoesHintCollide(point)
{
        return false;
}


void widget::ThrowEvent( std::shared_ptr<widget_event> Event, bool bSelfHandle )
{
        
        if( Event->Thrower.expired() ) {
                Event->Thrower = WidgetByPointer( this );
        }
        
        if( bSelfHandle ) {
                this->HandleEvent( Event );
        } else {
                std::shared_ptr<widget> strong_parent = Parent.lock();
                if( strong_parent )
                {
                        strong_parent->HandleEvent( Event );
                }
        }
                
	
}

void widget::HandleEvent( std::shared_ptr<widget_event> Event )
{
	this->OnEvent( Event );
	if( !Event->bHandled )
	{
		this->ThrowEvent( Event );
	}
}

// Helpers.

/**
  * @brief Copies basic parameters from the widget specified.
  * 
  * Used internally for copy constructors.
  * 
  */
void widget::SimpleCopy( const widget& Widget )
{
	// Position stuff.
	this->Position = Widget.Position;
	this->Size = Widget.Size;
	this->Layer = Widget.Layer;
		
	// Collision.
	this->boSolid = Widget.boSolid; // Solid -- consume input.
	
	// Input.
	
	this->boKeyboardFocusable = Widget.boKeyboardFocusable;
	this->boLoseFocusOnESC = Widget.boLoseFocusOnESC;
	
	// Other.
	this->boValidateOnRefresh = Widget.boValidateOnRefresh;
	this->boInvalidateOnMousePass = Widget.boInvalidateOnMousePass;
	this->boInvalidateOnKeyboardFocus = Widget.boInvalidateOnKeyboardFocus;
	
	//
	
	this->ValidityState = Widget.ValidityState;
	
}

void widget::SetSecondPosition( const point& position2 )
{
	this->Size = AreaSize( Position, position2 );
}

/** @brief Sets parent of the widget to specified pointer, if valid.
 *  @warning Doesn't add the widget to parent's widget list, for internal use only.
 */
void widget::SetParent( std::weak_ptr<widget> Parent )
{
        const std::shared_ptr<widget> Old = this->Parent.lock();
	if( !Parent.expired() )
	{
		this->Parent = Parent;
                
                if( Old ) {
                        Old->Invalidate( ValidityState::ChildrenUpdated );
                }
	}
}

/** @brief Sets parent of the widget to specified pointer, if valid.
 *  @warning Doesn't add the widget to parent's widget list, for internal use only.
 */
bool widget::SetParent( widget* const Parent )
{
	std::weak_ptr<widget> Pointer = WidgetByPointer( Parent );
	this->SetParent( Pointer );
	return !Pointer.expired();
}

std::shared_ptr<widget> widget::AddChild( std::shared_ptr<widget> Child )
{
	if( Child ) {
		widget* Parent = Child->Parent.lock().get();
		if( Parent && Parent != this ) {
                        Parent->RemoveChild( Child.get() );
		}
		
		Child->SetParent( this ); // Also invalidates this widget. (ChildrenUpdated.)
		
		if( std::find( this->Children.begin(), this->Children.end(), Child ) != this->Children.end() ) {
                        return Child;
                }
		this->Children.push_back( Child );
        
                if( this->bsConstructed && this->Children.size() != 0 ) {
                        RecursivePostConstruct( this->Children );
                }
                
		return Child;
	} else {
		return std::shared_ptr<widget>();
	}
}
std::shared_ptr<widget> widget::AddChild( widget* Child )
{
	return this->AddChild( std::shared_ptr<widget>( Child ) );
}

void widget::RemoveChild( widget* Child )
{
	for( std::size_t i = 0; i < this->Children.size(); i++ )
	{
		if( this->Children[i].get() == Child )
		{
			this->RemoveChild( i );
                        this->Invalidate( ValidityState::ChildrenUpdated );
			return;
		}
	}
}

void widget::RemoveChild( const std::size_t& Index )
{
	widget* const Child = this->Children[Index].get();
	if( Child )
	{
		Child->Parent.reset();
	}
	this->Children.erase( this->Children.begin()+Index );
        this->Invalidate( ValidityState::ChildrenUpdated );
}

void widget::ClearChildren()
{
        if( this->Children.size() != 0 ) {
                this->Invalidate( ValidityState::ChildrenUpdated );
        }
        this->Children.clear();
}

/** @brief Removes the widget from its parent. */
void widget::Remove()
{
        std::shared_ptr<widget> SelfPointer = WidgetByPointer( this ).lock();
        std::shared_ptr<widget> Parent      = this->Parent.lock();
        
        if( Parent ) {
                Parent->RemoveChild( this );
        } else {
                TheWindowManager.Cur().Tab.RemoveWidget( this );
        }
        
        // SelfPointer should get destroyed here.
        
}

void widget::Invalidate( ValidityState_t Reason )
{
    this->ValidityState |= Reason;
}

} // namespace unotui
