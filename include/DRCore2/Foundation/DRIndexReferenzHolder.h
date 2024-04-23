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
 * File:   DRIndexReferenzHolder.h
 * Author: Dario
 *
 * Created on 8. November 2011, 11:56
 */

#ifndef __DR_CORE2_INDEX_REFERENZ_HOLDER__
#define	__DR_CORE2_INDEX_REFERENZ_HOLDER__

#include "DRCore2/export.h"

class DRCORE2_EXPORT DRIndexReferenzHolder
{
public:
    DRIndexReferenzHolder(unsigned int maxIndexCount);
    ~DRIndexReferenzHolder();
    
    void add(unsigned int index);
    
    void remove(unsigned int index);
    
    unsigned int getFree();
    
private:
    
    unsigned int*       mReferenzCounter;
    unsigned int*       mFreePlaces;
    unsigned int        mFreePlaceCursor;
    unsigned int        mMaxIndexCount;
    
};


#endif	/* __DR_CORE2_INDEX_REFERENZ_HOLDER__ */

