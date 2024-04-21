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


#ifndef __DR_CORE2_MAIN__
#define __DR_CORE2_MAIN__

/*#ifndef _DEBUG
#define _DEBUG
#endif _DEBUG
*/

//#define _DEBUG

#ifdef _WIN32
//#define _WINSOCKAPI_
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/errno.h>
#endif

#include "DRCore2/export.h"
#include "DRTypes.h"


#ifdef _WIN32
#pragma warning (disable : 4251) 
#pragma warning (disable : 4201)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#endif //_WIN32

//ä = \303\244
//ü = \303\274
//ö = \303\266
//ß = \303\237

DRCORE2_EXPORT DRReturn Core2_init(const char* logFileName = "Logger.html");
DRCORE2_EXPORT void      Core2_exit();


#endif // __DR_CORE2_MAIN__
