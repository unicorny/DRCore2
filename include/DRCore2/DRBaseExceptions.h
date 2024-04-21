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
 * \date: 23.08.2022
 *
 * \brief basic exception classes 
 */

#ifndef __DR_CORE2_BASE_EXCEPTIONS_H
#define __DR_CORE2_BASE_EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include "DRCore2/export.h"

class DRCORE2_EXPORT DRCoreBaseException: public std::runtime_error
{
public:
	explicit DRCoreBaseException(const char* what) noexcept;
	virtual ~DRCoreBaseException();
	virtual std::string toString();
protected:
};

class DRCORE2_EXPORT DRMemoryException : public DRCoreBaseException
{
public: 
	explicit DRMemoryException(const char* what, size_t size) : DRCoreBaseException(what), mSize(size) {}
	virtual std::string toString();
protected:
	size_t mSize;

};
#endif //__DR_CORE2_BASE_EXCEPTIONS_H