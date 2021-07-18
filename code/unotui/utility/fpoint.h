#ifndef __FPOINT_H_
#define __FPOINT_H_

#include <unotui/utility/point.h>

namespace unotui {

struct dpoint;

struct fpoint
{
	float x, y;
	fpoint();
	fpoint( const float, const float );
	fpoint( const point& );
        fpoint( const dpoint& );
        fpoint( const unit& );
	
	std::string string() const;
};

fpoint operator+( const fpoint& , const fpoint& );
fpoint operator-( const fpoint& );
fpoint operator-( const fpoint& , const fpoint& );
fpoint& operator+=( fpoint& , const fpoint& );
fpoint& operator-=( fpoint& , const fpoint& );
fpoint operator*( const fpoint& , const fpoint& );
fpoint operator/( const fpoint& , const fpoint& );

fpoint operator*( const fpoint, const int );
fpoint operator/( const fpoint, const int );
fpoint operator*( const int, const fpoint );
fpoint operator/( const int, const fpoint );

fpoint operator*( const fpoint, const float );
fpoint operator/( const fpoint, const float );
fpoint operator*( const float, const fpoint );
fpoint operator/( const float, const fpoint );

} // namespace unotui

#endif
