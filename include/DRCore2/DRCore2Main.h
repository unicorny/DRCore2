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

/*
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/errno.h>
#endif
*/

#include "DRCore2/export.h"
#include "DRTypes.h"
#include "DRCore2/Threading/DRMultithreadLogger.h"

#ifdef DEBUG
#define PROFILING
#endif

//Globale Variablen
extern DRMultithreadLogger DRLog;
#ifdef PROFILING
extern DRMultithreadLogger DRSpeedLog;
#endif

//makros für Log Benutzung
#define WRITETOLOG(str, pl) DRLog.writeToLog(str, pl);
#define LOG_ERROR(str, ret) {DRLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR_VOID(str) {DRLog.LOG_ERROR_INTERN(str); return;}
#define LOG_INFO(str) DRLog.LOG_INFO_INTERN(str);
#define LOG_WARNING(str) DRLog.LOG_WARNING_INTERN(str);


/*#ifdef _WIN32
#pragma warning (disable : 4251) 
#pragma warning (disable : 4201)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#endif //_WIN32
*/
//ä = \303\244
//ü = \303\274
//ö = \303\266
//ß = \303\237

DRCORE2_EXPORT DRReturn Core2_init(const char* logFileName = "Logger.html");
DRCORE2_EXPORT void     Core2_exit();


#endif // __DR_CORE2_MAIN__
