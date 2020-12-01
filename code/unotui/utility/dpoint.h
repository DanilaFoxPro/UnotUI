#ifndef __UNOTUI_CODE_UTILITY_DPOINT_H_
#define __UNOTUI_CODE_UTILITY_DPOINT_H_

#include <unotui\utility\point.h>

namespace unotui {

struct dpoint
{
	double x, y;
	dpoint();
	dpoint( const double, const double );
	dpoint( const point& );
        dpoint( const unit& );
	
	std::string string() const;
};

dpoint operator+( const dpoint& , const dpoint& );
dpoint operator-( const dpoint& );
dpoint operator-( const dpoint& , const dpoint& );
dpoint& operator+=( dpoint& , const dpoint& );
dpoint& operator-=( dpoint& , const dpoint& );
dpoint operator*( const dpoint& , const dpoint& );
dpoint operator/( const dpoint& , const dpoint& );

dpoint operator*( const dpoint, const int );
dpoint operator/( const dpoint, const int );
dpoint operator*( const int, const dpoint );
dpoint operator/( const int, const dpoint );

dpoint operator*( const dpoint, const double );
dpoint operator/( const dpoint, const double );
dpoint operator*( const double, const dpoint );
dpoint operator/( const double, const dpoint );

} // namespace unotui

#endif
