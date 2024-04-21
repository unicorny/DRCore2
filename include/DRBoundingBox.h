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

#ifndef __DR_CORE2_BOUNDING_BOX_H
#define __DR_CORE2_BOUNDING_BOX_H

class CORE2_API DRBoundingBox 
{
public:
	DRBoundingBox(DRVector2 minValues = DRVector2(0.0f), DRVector2 maxValues = DRVector2(0.0f))
		: mMin(minValues), mMax(maxValues) {}
	
	DRBoundingBox(DRVector2* vertices, u16 vertexCount);
	DRBoundingBox(const DRBoundingBox& bb) { memcpy(this, &bb, sizeof(DRBoundingBox)); }

	~DRBoundingBox() {};

	inline DRReal getCenterX() const { return mMin.x + (mMax.x - mMin.x) / 2; }
	inline DRReal getCenterY() const { return mMin.y + (mMax.y - mMin.y) / 2; }
	inline DRVector2 getCenter() { return mMin + (mMax - mMin) / 2; }
	void expandBy(const DRBoundingBox& otherBoundingBox);
	inline void moveBy(DRVector2 diff) { mMin += diff; mMax += diff; }
	inline DRReal getWidth() const { return mMax.x - mMin.x; }
	inline DRReal getHeight() const { return mMax.y - mMin.y; }
	inline DRVector2 getSize() const { return mMax - mMin; }
	// \brief  Return true if this bounding box intersects the specified bounding box. 
	bool intersects(const DRBoundingBox& otherBoundingBox) const;
	bool inside(const DRBoundingBox& otherBoundingBox) const;
						
	inline const bool operator== (const DRBoundingBox& otherBoundingBox) const {return mMin == otherBoundingBox.mMin && mMax == otherBoundingBox.mMax;}
	inline void operator+ (const DRVector2& diff) { moveBy(diff); }
	inline void operator+= (const DRBoundingBox& otherBoundingBox) { expandBy(otherBoundingBox); }
	inline DRBoundingBox& operator *= (const DRVector2& scale) { mMin *= scale; mMax *= scale; return *this; }	
	inline DRBoundingBox& operator = (const DRBoundingBox& bb) { memcpy(this, &bb, sizeof(DRBoundingBox) * 4); return *this; }
	inline DRVector2 getMin() { return mMin; }
	inline DRVector2 getMax() { return mMax; }
protected:
	DRVector2 mMin;
	DRVector2 mMax;
};

#endif //__DR_CORE2_BOUNDING_BOX_H