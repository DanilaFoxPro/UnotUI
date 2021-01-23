#ifndef __UNOTUI_CODE_UTILITY_IPOINT_H_
#define __UNOTUI_CODE_UTILITY_IPOINT_H_

namespace unotui {

struct point;
struct unit;
        
/** @brief Integer point. */
struct ipoint
{
        int x, y;

        ipoint();
        ipoint( const int );
        ipoint( const int, const int );
        ipoint( const point& );
        ipoint( const unit& );
        
};

ipoint  operator+( const ipoint& , const ipoint& );
ipoint  operator-( const ipoint& );
ipoint  operator-( const ipoint& , const ipoint& );
ipoint& operator+=( ipoint& , const ipoint& );
ipoint& operator-=( ipoint& , const ipoint& );
ipoint  operator*( const ipoint& , const ipoint& );
ipoint  operator/( const ipoint& , const ipoint& );

} // namespace unotui

#endif
