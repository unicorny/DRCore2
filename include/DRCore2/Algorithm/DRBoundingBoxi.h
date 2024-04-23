/*/*************************************************************************
*                                                                         *
* Core, Core-Lib for my programs, Core doesn't need any libraries	   *
* Copyright (C) 2012, 2013, 2014 Dario Rekowski                           *
* Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
*                                                                         *
* This program is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* any later version.                                                      *
*									   *
* This program is distributed in the hope that it will be useful,	   *
* but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
* GNU General Public License for more details.				   *
*									   *
* You should have received a copy of the GNU General Public License	   *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                         *
***************************************************************************/

/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 17.04.2016
 *
 * \brief class for calculating and using bounding box volume for collision
 */

#ifndef __DR_CORE2_BOUNDING_BOX_I_H
#define __DR_CORE2_BOUNDING_BOX_I_H

#include "DRCore2/export.h"
#include "DRCore2/Foundation/DRVector2i.h"

class DRCORE2_EXPORT DRBoundingBoxi 
{
public:
	DRBoundingBoxi(DRVector2i minValues = DRVector2i(0), DRVector2i maxValues = DRVector2i(0))
		: mMin(minValues), mMax(maxValues) {}
	~DRBoundingBoxi() {};

	inline int getCenterX() const { return mMin.x + (mMax.x - mMin.x) / 2; }
	inline int getCenterY() const { return mMin.y + (mMax.y - mMin.y) / 2; }
	inline DRVector2i getCenter() { return mMin + (mMax - mMin) / 2; }
	void expandBy(const DRBoundingBoxi& otherBoundingBox);
	inline void moveBy(DRVector2i diff) { mMin += diff; mMax += diff; }
	inline int getWidth() const { return mMax.x - mMin.x; }
	inline int getHeight() const { return mMax.y - mMin.y; }
	inline DRVector2i getSize() const { return mMax - mMin; }
	// \brief  Return true if this bounding box intersects the specified bounding box. 
	bool intersects(const DRBoundingBoxi& otherBoundingBox) const;
	bool inside(const DRBoundingBoxi& otherBoundingBox) const;
						
	inline const bool operator== (const DRBoundingBoxi& otherBoundingBox) const {return mMin == otherBoundingBox.mMin && mMax == otherBoundingBox.mMax;}
	inline void operator+ (const DRVector2i& diff) { moveBy(diff); }
	inline void operator+= (const DRBoundingBoxi& otherBoundingBox) { expandBy(otherBoundingBox); }
	inline DRVector2i getMin() { return mMin; }
	inline DRVector2i getMax() { return mMax; }
protected:
	DRVector2i mMin;
	DRVector2i mMax;

};

#endif //__DR_CORE2_BOUNDING_BOX_I_H