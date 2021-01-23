#include <unotui\utility\ipoint.h>

#include <unotui\utility\point.h>

namespace unotui {

ipoint::ipoint()
{
        this->x = 0;
        this->y = 0;
}

ipoint::ipoint( const int Integer )
{
        this->x = Integer;
        this->y = Integer;
}

ipoint::ipoint( const int X, const int Y )
{
        this->x = X;
        this->y = Y;
}

ipoint::ipoint( const unotui::point& Point )
{
        this->x = Point.xpixels();
        this->y = Point.ypixels();
}

ipoint::ipoint( const unotui::unit& Unit )
{
        this->x = Unit.xpixels();
        this->y = Unit.xpixels();
}

ipoint  operator+( const ipoint& A, const ipoint& B )
{
        return ipoint(
                A.x + B.x,
                A.y + B.y
        );
}

ipoint  operator-( const ipoint& Point )
{
        return ipoint(
                -Point.x,
                -Point.y
        );
}

ipoint  operator-( const ipoint& A, const ipoint& B )
{
        return ipoint(
                A.x - B.x,
                A.y - B.y
        );
}

ipoint& operator+=( ipoint& A, const ipoint& B )
{
        A.x += B.x;
        A.y += B.y;
        return A;
}

ipoint& operator-=( ipoint& A, const ipoint& B )
{
        A.x -= B.x;
        A.y -= B.y;
        return A;
}

ipoint  operator*( const ipoint& A, const ipoint& B )
{
        return ipoint(
                A.x * B.x,
                A.y * B.y
        );
}

ipoint  operator/( const ipoint& A, const ipoint& B )
{
        return ipoint(
                A.x / B.x,
                A.y / B.y
        );
}

} // namespace unotui
