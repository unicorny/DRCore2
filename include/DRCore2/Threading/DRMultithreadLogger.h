/*/*************************************************************************
 *                                                                         *
 * EngineDLL, Engine for my programs, using SDL and OpenGL		   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
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

#ifndef __DR_UNIVERSUM_LIB_ENGINE_LOGGER__
#define __DR_UNIVERSUM_LIB_ENGINE_LOGGER__

#include "DRCore2/export.h"
#include "DRCore2/Foundation/DRLogger.h"
#include "DRCore2/Utils/DRProfiler.h"
#include "DRMultithreadQueue.h"
#include "DRThread.h"

/**!
 * \author: Dario Rekowski
 *
 * \date:
 * 
 * \desc: Base Thread Safe Logging class 
 *        adding stack for logging entrys to act fast in multithreaded environment
 */
class DRCORE2_EXPORT DRMultithreadLogger : public DRLogger, DRThread
{
public:
    DRMultithreadLogger();
    virtual ~DRMultithreadLogger();

    virtual DRReturn init(const char* pcFilename, bool printToConsole);
    virtual void exit();

    //in die Log-Datei schreiben
    virtual DRReturn writeToLogDirect(std::string text);
    virtual DRReturn writeToLogDirect(const char* pcText, ...);
    virtual DRReturn writeToLog(const char* pcText, ...);
    virtual DRReturn writeToLog(std::string text);
    virtual DRReturn writeAsBinary(std::string name, u8 zahl);
    virtual DRReturn writeAsBinary(std::string name, u16 zahl);
    virtual DRReturn writeAsBinary(std::string name, u32 zahl);
    virtual DRReturn writeAsBinary(std::string name, u64 zahl);
protected:
            
    virtual DRReturn writeAsBinary_intern(std::string name, std::string zahlBuffer);
    const char* getTimeSinceStart();

	virtual int ThreadFunction();

    DRProfiler gStartTimePoint;
	DRMultithreadQueue<std::string> mMessages;
    char mTimeStringBuffer[24];
    std::mutex mTimeStringBufferMutex;

private:

};

//Globale Variablen
DRCORE2_EXPORT extern DRMultithreadLogger DRLog;

//makros für Log Benutzung
#define WRITETOLOG(str, pl) DRLog.writeToLog(str, pl);
#define LOG_ERROR(str, ret) {DRLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR_VOID(str) {DRLog.LOG_ERROR_INTERN(str); return;}
#define LOG_INFO(str) DRLog.LOG_INFO_INTERN(str);
#define LOG_WARNING(str) DRLog.LOG_WARNING_INTERN(str);
 
#endif //__DR_UNIVERSUM_LIB_ENGINE_LOGGER__