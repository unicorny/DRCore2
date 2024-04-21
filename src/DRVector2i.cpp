#include "Core2Main.h"

DRVector2i::DRVector2i(const DRVector2& v) 
	: x(static_cast<int>(ceil(v.x))), 
	  y(static_cast<int>(ceil(v.y))) 
{
}