#ifndef __POINT_H_
#define __POINT_H_

#include <unotui\utility\unit.h>

namespace unotui {

struct fpoint;
/** @brief Represents a UI point. Stored as Ratio+Pixels.
 */
struct point
{
	unit x;
	unit y;
	point();
	point( const unit& );
	point( const unit&, const unit& );
        point( const fpoint& );
	
	std::string string() const;
	
	void rationalize( int x_limit, int y_limit );
        
        int xpixels()  const { return x.xpixels(); }
        int ypixels()  const { return y.ypixels(); }
        
        float xratio() const { return x.xratio(); }
        float yratio() const { return y.yratio(); }
        
};

point operator+( const point& , const point& );
point operator-( const point& );
point operator-( const point& , const point& );
point operator*( const point& , const point& );
point operator/( const point& , const point& );

point operator*( const point& , const int& );
point operator/( const point& , const int& );

} // namespace unotui

#endif
