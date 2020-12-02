#include <unotui\utility\dpoint.h>

namespace unotui {

//:: Constructors.

dpoint::dpoint()
{
        this->x = 0.0f;
        this->y = 0.0f;
}

dpoint::dpoint( const double x, const double y )
{
        this->x = x;
        this->y = y;
}

dpoint::dpoint( const point& p )
{
        this->x = p.x.xratio();
        this->y = p.y.yratio();
}

dpoint::dpoint( const unit& Unit )
{
        this->x = Unit.xratio();
        this->y = Unit.yratio();
}


std::string dpoint::string() const
{
	return "< " + std::to_string( this->x ) + ", " + std::to_string( this->y ) + " >"; 
}

//:: Operators.

dpoint operator+( const dpoint& dpoint_1, const dpoint& dpoint_2 )
{
	return dpoint( dpoint_1.x + dpoint_2.x, dpoint_1.y + dpoint_2.y );
}

dpoint operator-( const dpoint& dpoint_ )
{
	return dpoint( -dpoint_.x, -dpoint_.y );
}

dpoint operator-( const dpoint& dpoint_1, const dpoint& dpoint_2 )
{
	return dpoint( dpoint_1.x - dpoint_2.x, dpoint_1.y - dpoint_2.y );
}

dpoint& operator+=( dpoint& dpoint_1, const dpoint& dpoint_2 )
{
	dpoint_1 = dpoint_1 + dpoint_2;
	return dpoint_1;
}

dpoint& operator-=( dpoint& dpoint_1, const dpoint& dpoint_2 )
{
	dpoint_1 = dpoint_1 - dpoint_2;
	return dpoint_1;
}

dpoint operator*( const dpoint& dpoint_1, const dpoint& dpoint_2 )
{
	return { dpoint_1.x * dpoint_2.x, dpoint_1.y * dpoint_2.y };
}

dpoint operator/( const dpoint& dpoint_1, const dpoint& dpoint_2 )
{
	return { dpoint_1.x / dpoint_2.x, dpoint_1.y / dpoint_2.y };
}

dpoint operator*( const dpoint Point, const int Integer )
{
        return { Point.x * Integer, Point.y * Integer };
}

dpoint operator/( const dpoint Point, const int Integer )
{
        return { Point.x / Integer, Point.y / Integer };
}

dpoint operator*( const int Integer, const dpoint Point )
{
        return { Point.x * Integer, Point.y * Integer };
}

dpoint operator/( const int Integer, const dpoint Point )
{
        return { Point.x / Integer, Point.y / Integer };
}


dpoint operator*( const dpoint Point, const double Double )
{
        return dpoint( Point.x*Double, Point.y*Double );
}

dpoint operator/( const dpoint Point, const double Double )
{
        return dpoint( Point.x/Double, Point.y/Double );
}

dpoint operator*( const double Double, const dpoint Point )
{
        return dpoint( Point.x*Double, Point.y*Double );
}

dpoint operator/( const double Double, const dpoint Point )
{
        return dpoint( Point.x/Double, Point.y/Double );
}

} // namespace unotui
