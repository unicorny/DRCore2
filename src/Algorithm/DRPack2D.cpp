#include "DRCore2/Algorithm/DRPack2D.h"

#include <cassert>
#include <algorithm>

// ****************** DRPack2D_Rect *************************************************************
void DRPack2D_Rect::fillColorArrayWithColorRect(DRColor* colors, const DRVector2i& dim, const DRColor& col) const
{
	for (int y = pos.y; y < pos.y + size.y; y++) {
		assert(y < dim.y);
		for (int x = pos.x; x < pos.x + size.x; x++) {
			assert(x < dim.x);
			colors[y * dim.x + x] = col;
		}
	}
}

void DRPack2D_Size_WithKey::fillColorArrayWithColorRect(DRVector2i* outputPositions, DRColor* colors, const DRVector2i& dim, const DRColor& col) const
{
	auto pos = outputPositions[key];
	for (int y = pos.y; y < pos.y + size.y; y++) {
		assert(y < dim.y);
		for (int x = pos.x; x < pos.x + size.x; x++) {
			assert(x < dim.x);
			colors[y * dim.x + x] = col;
		}
	}
}

// *************************** DRPack2D ***********************************************************

DRPack2D::DRPack2D(DRPack2D_OrderType orderTypes/* = DRPACK2D_ORDER_ALL*/, u16 maxSide/* = 4096*/)
	: mOrderTypes(orderTypes), mMaxSide(maxSide)
{

}

DRPack2D::~DRPack2D()
{


}


DRReturn DRPack2D::findBestPack(std::vector<DRPack2D_Size_WithKey>* inputSize, DRVector2i* outputPositions, DRVector2i* finalDim /*= nullptr*/)
{
	mInputSizes = inputSize;
	mOutputPositions = outputPositions;

	auto inputSizeCount = mInputSizes->size();
	if (!inputSizeCount || !outputPositions) return DR_ZERO_POINTER;

	// 1. calculate area sum
	int areaSum = 0;
	for (u16 i = 0; i < inputSizeCount; i++) {
		areaSum += (*mInputSizes)[i].area();
	}
	int sideMultiplikator = 0;
	while (true) {

		// 2. calculate possible needed space
		// loop are faster than sqrt
		// iside pixel size for render texture, using quadratic texture
		// minimal square power-of-2 sizes rect to possible containing all input Rects
		int iside = 1;
		while (iside * iside <= areaSum) { iside <<= 1; }
		iside >>= 1;
		for (int i = 0; i < sideMultiplikator; i++) { iside <<= 1; }

		if (iside > mMaxSide) {
			return DR_ERROR;
		}

		if (finalDim) {
			finalDim->x = iside;
			finalDim->y = iside;
		}

		// 3. try out with different ordering algos
		if (DR_OK == tryOutDifferentOrders(DRVector2i(iside))) {
			return DR_OK;
		}

		// 4. calculate non square power-of-2 rect as container

		iside = 1;
		while (iside * (iside << 1) <= areaSum) { iside <<= 1; }
		for (int i = 0; i < sideMultiplikator; i++) { iside <<= 1; }

		if ((iside << 1) > mMaxSide) {
			return DR_ERROR;
		}

		// smaller side as width
		if (DR_OK == tryOutDifferentOrders(DRVector2i(iside, iside << 1))) {
			if (finalDim) {
				finalDim->x = iside;
				finalDim->y = iside << 1;
			}
			return DR_OK;
		}
		// smaller side as height
		if (DR_OK == tryOutDifferentOrders(DRVector2i(iside << 1, iside))) {
			if (finalDim) {
				finalDim->x = iside << 1;
				finalDim->y = iside;
			}
			return DR_OK;
		}

		// if not work, try to increase iside even further 
		sideMultiplikator++;
	}

	return DR_ERROR;
}

DRPACK2D_Size_compareI DRPack2D::getCompareObj(DRPack2D_OrderType type)
{
	switch (type) {
	case DRPACK2D_ORDER_WIDTH: return DRPack2D_Size_compareWidth();
	case DRPACK2D_ORDER_HEIGHT: return DRPack2D_Size_compareHeight();
	case DRPACK2D_ORDER_PATHOLOGICAL: return DRPack2D_Size_comparePathological();
	case DRPACK2D_ORDER_BIGGER_SIDE: return DRPack2D_Size_compareBiggerSide();
	case DRPACK2D_ORDER_AREA: return DRPack2D_Size_compareArea();
	}
	return DRPACK2D_Size_compareI();
}

DRReturn DRPack2D::tryOutDifferentOrders(const DRVector2i& iDimension)
{
	// try out different ordered input, return with OK if all input could be stored
	// didn't work, maybe operator in struct must be virtual, but than we cannot inline it
/*	int orderType = 1;
	while (orderType != DRPACK2D_ORDER_MAX) {
		if ((mOrderTypes & orderType) == orderType) {
			std::sort(mInputSizes->begin(), mInputSizes->end(), getCompareObj(static_cast<DRPack2D_OrderType>(orderType)));
			if (DR_OK == pack2D(iDimension)) {
				mFinalWorkingOrderType = static_cast<DRPack2D_OrderType>(orderType);
				return DR_OK;
			}
		}
		orderType <<= 1;
	}
	
	*/
	if ((mOrderTypes & DRPACK2D_ORDER_WIDTH) == DRPACK2D_ORDER_WIDTH) {
		std::sort(mInputSizes->begin(), mInputSizes->end(), DRPack2D_Size_compareWidth());
		if (DR_OK == pack2D(iDimension)) {
			mFinalWorkingOrderType = DRPACK2D_ORDER_WIDTH;
			return DR_OK;
		}
	}
	if ((mOrderTypes & DRPACK2D_ORDER_HEIGHT) == DRPACK2D_ORDER_HEIGHT) {
		std::sort(mInputSizes->begin(), mInputSizes->end(), DRPack2D_Size_compareHeight());
		if (DR_OK == pack2D(iDimension)) {
			return DR_OK;
		}
	}
	if ((mOrderTypes & DRPACK2D_ORDER_PATHOLOGICAL) == DRPACK2D_ORDER_PATHOLOGICAL) {
		std::sort(mInputSizes->begin(), mInputSizes->end(), DRPack2D_Size_comparePathological());
		if (DR_OK == pack2D(iDimension)) {
			return DR_OK;
		}
	}
	if ((mOrderTypes & DRPACK2D_ORDER_BIGGER_SIDE) == DRPACK2D_ORDER_BIGGER_SIDE) {
		std::sort(mInputSizes->begin(), mInputSizes->end(), DRPack2D_Size_compareBiggerSide());
		if (DR_OK == pack2D(iDimension)) {
			return DR_OK;
		}
	}
	if ((mOrderTypes & DRPACK2D_ORDER_AREA) == DRPACK2D_ORDER_AREA) {
		std::sort(mInputSizes->begin(), mInputSizes->end(), DRPack2D_Size_compareArea());
		if (DR_OK == pack2D(iDimension)) {
			return DR_OK;
		}
	}
	return DR_ERROR;

}

DRReturn DRPack2D::pack2D(const DRVector2i& iDimension)
{
	mSpaces.clear();
	mSpaces.push_back(DRPack2D_Rect(0, iDimension));

	auto inputSizeCount = mInputSizes->size();
	u16 foundFitting = 0;
	u16 checkedFitting = 0;
	for (int iInputSize = inputSizeCount - 1; iInputSize >= 0; iInputSize--) {
		auto local_inputSize = (*mInputSizes)[iInputSize];
		checkedFitting++;
		
		// search fitting empty space backwards
		for (int iEmpty = mSpaces.size() - 1; iEmpty >= 0; iEmpty--) {
			auto space = mSpaces[iEmpty];
			
			if (space.size.x >= local_inputSize.size.x && space.size.y >= local_inputSize.size.y) {
				foundFitting++;
				mOutputPositions[local_inputSize.key] = space.pos;
				
				// remove not longer empty space
				mSpaces[iEmpty] = mSpaces.back();
				mSpaces.pop_back();

				// add new space(s)
				// right split
				auto rightSplit = DRPack2D_Rect(
					DRVector2i(space.pos.x + local_inputSize.size.x, space.pos.y),			// position
					DRVector2i(space.size.x - local_inputSize.size.x, local_inputSize.size.y)  // size
				);
				
				// bottom split
				auto bottomSplit = DRPack2D_Rect(
					DRVector2i(space.pos.x, space.pos.y + local_inputSize.size.y),
					DRVector2i(space.size.x, space.size.y - local_inputSize.size.y)
				);
				
				// add bigger split first
				if (rightSplit.area() > bottomSplit.area()) {
					if (rightSplit.area() > 0) mSpaces.push_back(rightSplit);
					if (bottomSplit.area() > 0) mSpaces.push_back(bottomSplit);
				}
				else {
					if (bottomSplit.area() > 0) mSpaces.push_back(bottomSplit);
					if (rightSplit.area() > 0) mSpaces.push_back(rightSplit);
				}
				break;
			}

		}
		// last didn't fit, break
		if (foundFitting < checkedFitting) {
			return DR_ERROR;
		}
	}

	return DR_OK;
}