#include "DRCore2/Foundation/DRVector2i.h"
#include "DRCore2/Foundation/DRVector2.h"
#include <math.h>

DRVector2i::DRVector2i(const DRVector2& v) 
	: x(static_cast<int>(ceil(v.x))), 
	  y(static_cast<int>(ceil(v.y))) 
{
}