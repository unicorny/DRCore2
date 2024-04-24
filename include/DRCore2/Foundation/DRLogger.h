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
Logging System &Auml;hnlich dem der TriBase-Engine von David Scherfgen
Programmierer: Dario Rekowski
*/
//Hier kommt alles rein, was mit Fehlerbehandlung zu tun hat

//Eine Aufz&auml;hlung f&uuml;r R&uuml;ckgabewerte f&uuml;r die meisten Funktionen
//k&ouml;nnen wir je nach dem noch erweitern
#ifndef __DR_CORE2_LOGGER__
#define __DR_CORE2_LOGGER__

#include "DRCore2/export.h"
#include "DRFile.h"
#include "DRUtils.h"

#include <string>
#include <fstream>

//Vorw&auml;rtsdeklaarionen f&uuml;r den Logger
class DRVector2;
class DRVector2i;
class DRVector3;
class DRColor;
class DRMatrix;

#if _MSC_VER < 1300
	#define DR_FUNCTION_ ("(Unbekannt)")
#else
	#define DR_FUNCTION_ (__FUNCTION__)
#endif

//Klasse f&uuml;r LogDatei wo alle Fehler reingeschrieben werden k&ouml;nnen
class DRCORE2_EXPORT DRLogger
{

private:
	//Membervariablen
	DRFile		m_File;			//Log-File
	std::string mLogFileName;
    bool		m_bPrintToConsole;

public:
	enum class LogFormat {
		INFO,
		WARNING,
		ERROR
	};

	//Und die &ouml;ffentlichen Funktionen
	DRLogger();
    DRLogger(const DRLogger& log);
	virtual ~DRLogger();

	virtual DRReturn init(const std::string& logFilename, bool printToConsole);
	virtual void exit();
        
    void setPrintToConsole(bool printToConsole) {m_bPrintToConsole = printToConsole;}

	//in die Log-Datei schreiben
	virtual DRReturn writeToLog(const char* pcText, ...);
    virtual DRReturn writeToLog(std::string text);
	virtual DRReturn writeToLogDirect(const char* pcText, ...);
    virtual DRReturn writeToLogDirect(std::string text);

	DRReturn writeVector2ToLog(const DRVector2& v, const char* name = NULL);			// 2D-Vektor ins Logbuch schreiben
    DRReturn writeVector3ToLog(const DRVector3& v, const char* pcName = NULL);			// 3D-Vektor ins Logbuch schreiben
    DRReturn writeMatrixToLog(const DRMatrix& m, const char* name = NULL);			// Matrix ins Logbuch schreiben
	//DRReturn WritePlaneToLog(DRPlane& p);				// Ebene ins Logbuch schreiben
	DRReturn writeColorToLog(const DRColor& c);				// Farbe ins Logbuch schreiben

	//! schreibe Table ins Log mit 1px * 1px großen Blöcken, und farbe
	//! \param colors[dim.x*dim.y]
	DRReturn writePixelGridToLog(DRColor* colors, DRVector2i dim);

	//inline um Variablen abzufragen
	inline DRFile* getFile() {return &m_File;};

	
//	DRReturn WriteDXError(HRESULT hr);


	//Info Text in Logbuch Datei schreiben

	#define LOG_INFO_INTERN(text) writeToLog("<tr><td><font size=\"2\"><b><font color=\"#008000\">INFO:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text,  DRRemoveDir(__FILE__), __LINE__, __FUNCTION__)
	// Fehlertext in Logbuchdatei schreiben
	#define LOG_ERROR_INTERN(text) writeToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text,  DRRemoveDir(__FILE__), __LINE__, __FUNCTION__)
	// Warnungstext in Logbuchdatei schreiben
	#define LOG_WARNING_INTERN(text) writeToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">WARNUNG:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", text, DRRemoveDir(__FILE__), __LINE__, __FUNCTION__)

};

//Globale Variablen
DRCORE2_EXPORT extern DRLogger DRLog;

//makros für Log Benutzung
#define WRITETOLOG(str, pl) DRLog.writeToLog(str, pl);
//#define DR_ERROR(str, ret) {DRLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR(str, ret) {DRLog.LOG_ERROR_INTERN(str); return ret;}
#define LOG_ERROR_VOID(str) {DRLog.LOG_ERROR_INTERN(str); return;}
#define LOG_INFO(str) DRLog.LOG_INFO_INTERN(str);
#define LOG_WARNING(str) DRLog.LOG_WARNING_INTERN(str);


#endif //__DR_CORE2_LOGGER__
