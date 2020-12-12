#include <unotui\utility\dpoint.h>
#include <math.h> // sqrt()
#include <stdexcept>

namespace unotui {

//:: Constructors.

dpoint::dpoint()
{
        this->x = 0.0;
        this->y = 0.0;
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

double dpoint::Length() const
{
        return sqrt( this->x*this->x + this->y*this->y );
}

/** If dpoint represents a vector, this would return the vector rotated 90 degrees clockwise. */
dpoint dpoint::RightVector() const
{
        return dpoint( this->y, -this->x );
}

/** Assuming dpoint represents a vector, makes length 1.0 while preserving direction.
 *  @note Precision is not guaranteed, length may be a bit off. (Bit in non-formal sense.)
 *  @note Zero-vector isn't normalized.
 */
dpoint dpoint::Normalize() const
{
        dpoint Result = dpoint( 0.0, 0.0 );
        
        const double Length = this->Length();
        
        if( Length != 0.0 ) {
                // This is faster than dividing by length, since division is generally
                // faster than multiplication. And in this case, there would be two
                // divisions. So they're turned into one division, and two
                // multiplications. Could also use '-freciprocal-math' when compiling,
                // but that would turn every 'x/y' into 'x*(1.0/y)'.
                //
                // Additionally, IEEE says: 'x/x=1 for all finite nonzero x', while
                // this optimization doesn't have that guarantee. I still choose the
                // optimization, since I already f**ked the precision by using 'sqrt()'
                // in 'Length()'.
                Result = (*this)*(1.0/Length);
                
                if( Result.Length() > 1.0 ) {
                        throw std::logic_error(
                                "Vector normalization resulted in a non-normalized vector. "
                                "(Length " + std::to_string( Result.Length()) + ".)\n"
                                "For vector: " + this->String() + "."
                        );
                }
                
        }
        
        return Result;
}

std::string dpoint::String() const
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
        // TODO: Consider making use of x/y -> x*(1/y).
        //       It would turn two divisions into one division
        //       and two multiplications and decrease the
        //       precision, while possibly being faster.
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
