#include <unotui\utility\window.h>

#include <unotui\entities\ent_window.h>

void ViewportToWindow()
{
        const ent_window& Current = TheWindowManager.Cur();
        glViewport( 0, 0, Current.x, Current.y );
}
