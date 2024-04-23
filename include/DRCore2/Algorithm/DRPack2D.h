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
/*
* File:   DRPack2.h
* Author: einhornimmond
*
* Created on 29.04.19
*/

#ifndef __DR_CORE2_PACK_2D__
#define	__DR_CORE2_PACK_2D__

#include "DRCore2/export.h"
#include "DRCore2/DRTypes.h"

#include "DRCore2/Foundation/DRVector2.h"
#include "DRCore2/Foundation/DRColor.h"

#include <vector>

/*!  
 *
 * \author einhornimmond
 *
 * \date 29.04.19
 *
 * \brief 2D Pack algorithm implementation of https://github.com/TeamHypersomnia/rectpack2D#algorithm
 *
*/

struct DRCORE2_EXPORT DRPack2D_Rect {
	DRPack2D_Rect(DRVector2i _pos, DRVector2i _size)
		: pos(_pos), size(_size) {}
	DRPack2D_Rect() {}

	DRVector2i pos;
	DRVector2i size;

	inline int area() const {
		return size.x * size.y;
	}
	void fillColorArrayWithColorRect(DRColor* colors, const DRVector2i& dim, const DRColor& col) const;
};



struct DRCORE2_EXPORT DRPack2D_Size_WithKey
{
	DRPack2D_Size_WithKey(u32 _key, DRVector2i _size)
		: key(_key), size(_size) {}
	DRPack2D_Size_WithKey() : key(0) {}

	u32 key;
	DRVector2i size;

	inline int area() const {
		return size.x * size.y;
	}
	void fillColorArrayWithColorRect(DRVector2i* outputPositions, DRColor* colors, const DRVector2i& dim, const DRColor& col) const;
};


struct DRPACK2D_Size_compareI
{

};

struct DRPack2D_Size_compareWidth : public DRPACK2D_Size_compareI
{
	inline bool operator() (const DRPack2D_Size_WithKey& a, const DRPack2D_Size_WithKey& b) const
	{
		return a.size.x < b.size.x;
	}
};

struct DRPack2D_Size_compareHeight : public DRPACK2D_Size_compareI
{
	inline bool operator() (const DRPack2D_Size_WithKey& a, const DRPack2D_Size_WithKey& b) const
	{
		return a.size.y < b.size.y;
	}

};

struct DRPack2D_Size_comparePathological : public DRPACK2D_Size_compareI
{
	inline bool operator() (const DRPack2D_Size_WithKey& a, const DRPack2D_Size_WithKey& b) const
	{
		//max(w, h) / min(w, h) * w * h
		int resA, resB;
		if (a.size.x > a.size.y) { resA = a.size.x / a.size.y * a.size.x * a.size.y; }
		else { resA = a.size.y / a.size.x * a.size.x * a.size.y; }

		if (b.size.x > b.size.y) { resB = b.size.x / b.size.y * b.size.x * b.size.y; }
		else { resB = b.size.y / b.size.x * b.size.x * b.size.y; }

		return resA < resB;
	}
};

struct DRPack2D_Size_compareBiggerSide : public DRPACK2D_Size_compareI
{
	inline bool operator() (const DRPack2D_Size_WithKey& a, const DRPack2D_Size_WithKey& b) const
	{
		auto aMax = a.size.x;
		auto bMax = b.size.y;
		if (aMax < a.size.y) { aMax = a.size.y; }
		if (bMax < b.size.y) { bMax = b.size.y; }
		return aMax < bMax;
	}
};

struct DRPack2D_Size_compareArea :  public DRPACK2D_Size_compareI
{
	inline bool operator() (const DRPack2D_Size_WithKey& a, const DRPack2D_Size_WithKey& b) const
	{
		return a.area() < b.area();
	}
};

enum DRPack2D_OrderType {
	DRPACK2D_ORDER_WIDTH = 1,
	DRPACK2D_ORDER_HEIGHT = 2,
	DRPACK2D_ORDER_PATHOLOGICAL = 4,
	DRPACK2D_ORDER_BIGGER_SIDE = 8,
	DRPACK2D_ORDER_AREA = 16,
	DRPACK2D_ORDER_MAX = 32,
	DRPACK2D_ORDER_ALL = DRPACK2D_ORDER_WIDTH | DRPACK2D_ORDER_HEIGHT | DRPACK2D_ORDER_PATHOLOGICAL | DRPACK2D_ORDER_BIGGER_SIDE | DRPACK2D_ORDER_AREA
};

class DRCORE2_EXPORT DRPack2D
{
public:
	DRPack2D(DRPack2D_OrderType orderTypes = DRPACK2D_ORDER_ALL, u16 maxSide = 4096);
	~DRPack2D();

	//! \param inputSize array with input rect sizes, will be sorted several times
	//! \param outputPositions rect pos will be set with key from input as array-index
	//! \param finalDim will be set on final dimension which can contain all inputRects, if set
	//! \return DR_ERROR if solution rect would exceed maxSide on at least on side
	DRReturn findBestPack(std::vector<DRPack2D_Size_WithKey>* inputSize, DRVector2i* outputPositions, DRVector2i* finalDim = nullptr);

	static DRPACK2D_Size_compareI getCompareObj(DRPack2D_OrderType type);
	
protected:
	DRReturn tryOutDifferentOrders(const DRVector2i& iDimension);
	DRReturn pack2D(const DRVector2i& iDimension);

	std::vector<DRPack2D_Rect> mSpaces;
	DRPack2D_OrderType mOrderTypes;
	//! \brief maximal value for rectangular side
	u16 mMaxSide;

	// working parameters
	std::vector<DRPack2D_Size_WithKey>* mInputSizes;
	DRVector2i* mOutputPositions;

	// infos
	DRPack2D_OrderType mFinalWorkingOrderType;
};


#endif	/* __DR_CORE2_PACK_2D__ */

