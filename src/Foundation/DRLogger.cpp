#include "DRCore2/Foundation/DRLogger.h"
#include "DRCore2/Foundation/DRVector2.h"
#include "DRCore2/Foundation/DRVector2i.h"
#include "DRCore2/Foundation/DRVector3.h"
#include "DRCore2/Foundation/DRColor.h"
#include "DRCore2/Foundation/DRMatrix.h"

#include <sstream>
#include <cstdarg>

DRLogger DRLog;

//Konstuktor und Deskonstruktor
DRLogger::DRLogger()
: m_bPrintToConsole(false)
{
	//m_pFile = NULL;
#ifdef _WIN32 
	strcpy_s(m_acFilename, "NotInitLogger.html");
#else 
	strcpy(m_acFilename, "NotInitLogger.html");
#endif
}

DRLogger::DRLogger(const DRLogger& log)
{
#ifdef _WIN32 
	strcpy_s(m_acFilename, log.m_acFilename);
#else
    strcpy(m_acFilename, log.m_acFilename);
#endif

}

//------------------------------------------------------
DRLogger::~DRLogger()
{
}

//-------------------------------------------------------
//--------------------------------------------------------
//init und Exit
DRReturn DRLogger::init(const char* pcFilename, bool printToConsole)
{
    m_bPrintToConsole = printToConsole;
    
	//Filenamen kopieren
	//sprintf(m_acFilename, pcFilename);
#ifdef _WIN32
	strcpy_s(m_acFilename, pcFilename);
#else 
	strcpy(m_acFilename, pcFilename);
#endif

	//Oeffnen zum ueberschreiben
	//m_pFile = fopen(m_acFilename, "w");
	m_File.open(m_acFilename, true, "wt");

	//pruefen
	if(!m_File.isOpen()) return DR_ERROR;

	//Was reinschreiben
//	fprintf(m_pFile, "Init des Loggers erfolgreich!\n");

	m_File.setFilePointer(0, SEEK_SET);
	char acTemp[] = "<HTML>\n<head><style type='text/css'>table.pixelGrid td {width:4px; height:4px;padding:0;}</style>\n<title>Log Datei</title></head><body>\n<font face=\"Arial\" size=\"2\">\n<table>";
	m_File.write(acTemp, sizeof(char), strlen(acTemp));
	writeToLog("Init des Loggers erfolgreich!");
	//schließen
//	fclose(m_pFile);
//	m_pFile = NULL;
	m_File.close();
	return DR_OK;
}

//----------------------------------------------------------

void DRLogger::exit()
{
	//if(!m_pFile) m_pFile = fopen(m_acFilename, "a");
	if(!m_File.isOpen()) m_File.open(m_acFilename, false, "at");
	m_File.setFilePointer(0, SEEK_END);
	//fprintf(m_pFile, "-----------------------------------------------------\nEnde gut alles gut.");
	//fclose(m_pFile);
	writeToLog("-----------------------------------------------------\nEnde gut alles gut.");
//	char acTemp[] = "</body>\n</table></HTML>";
//	m_File.write(acTemp, sizeof(char)*strlen(acTemp));
	writeToLogDirect("\n</table></body></HTML>");


	m_File.close();
}


// *****************************************************************
// Einen 2D-Vektor in die Logbuchdatei schreiben
DRReturn DRLogger::writeVector2ToLog(const DRVector2& v, const char* name)
{
	// 2D-Vektor in die Logbuchdatei schreiben
    if(name)
        writeToLogDirect("<tr><td><b>%s</b></td></tr>", name);
    return writeToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">2D-Vektor:</font></b> x = <i>%.5f</i>, y = <i>%.5f</i>, Laenge = <i>%.5f</i></td></tr>",
                            v.x, v.y, v.length());
}

// *****************************************************************
// Einen 3D-Vektor in die Logbuchdatei schreiben
DRReturn DRLogger::writeVector3ToLog(const DRVector3& v, const char* pcName /* = NULL */)
{
	// 3D-Vektor in die Logbuchdatei schreiben
	std::stringstream name;
	name << "3D-Vektor";
	if (pcName) name << " " << pcName;
	else name << ":";

	return writeToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">%s</font></b> x = <i>%.3f</i>, y = <i>%.3f</i>, z = <i>%.3f</i>, Laenge = <i>%.3f</i></td></tr>",
		                     name.str().data(), v.x, v.y, v.z, v.length());
}

// ******************************************************************
// Eine Matrix in die Logbuchdatei schreiben
DRReturn DRLogger::writeMatrixToLog(const DRMatrix& m, const char* name)
{
	// Matrix in die Logbuchdatei schreiben
    if(name)
        writeToLogDirect("<tr><td><b>%s</b></td></tr>", name);
    writeToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Matrix:</font></b><table>");
    for(int iRow = 0; iRow < 4; iRow++)
    {
            writeToLogDirect("<tr>");
            for(int iColumn = 0; iColumn < 4; iColumn++)
            {
                    writeToLogDirect("<td><font size=\"1\">");
                    writeToLogDirect("<i>%.3f</i>", m.m[iRow][iColumn]);
                    writeToLogDirect("</td>");
            }

            writeToLogDirect("</tr>");
    }

    return writeToLogDirect("</table>");
}

// ******************************************************************
// Eine Ebene in die Logbuchdatei schreiben
/*DRReturn DRLogger::WritePlaneToLog(tbPlane& p)
{
	// Ebene in die Logbuchdatei schreiben
	return tbWriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Ebene:</font></b> a = <i>%.3f</i>, b = <i>%.3f</i>, c = <i>%.3f</i>, d = <i>%.3f</i>, Normalenvektorl�nge = <i>%.3f</i></td></tr>",
                              p.a, p.b, p.c, p.d, tbVector3Length(p.n));
}
*/

// ******************************************************************
// Eine Farbe in die Logbuchdatei schreiben

DRReturn DRLogger::writeColorToLog(const DRColor& c)
{
	char acHexColor[9];
#ifdef _WIN32
	sprintf_s(acHexColor, "%x", (u32)(c) << 8);
#else 
	sprintf(acHexColor, "%x", (u32)(c) << 8);
#endif
	acHexColor[6] = 0;

	// Farbe in die Logbuchdatei schreiben
	return writeToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Farbe:</font></b> a = <i>%.3f</i>, r = <i>%.3f</i>, g = <i>%.3f</i>, b = <i>%.3f</i>, Hexadezimal: <i>0x%x</i>, <font color=\"#%s\"><i>Probetext</i></font></td></tr>",
                              c.a, c.r, c.g, c.b, (u32)(c), acHexColor);
}

// *************************************************************************
// schreibe Table ins Log mit 1px * 1px großen Blöcken, und farbe

DRReturn DRLogger::writePixelGridToLog(DRColor* colors, DRVector2i dim)
{
	std::stringstream ss;
	ss << "<tr><td><table class='pixelGrid'>";
	for (int y = 0; y < dim.y; y++) {
		ss << "<tr>";
		for (int x = 0; x < dim.x; x++) {
			char acHexColor[9];
			auto c = colors[y*dim.x + x];
			sprintf(acHexColor, "%08x", (u32)(c) << 8);
			//acHexColor[6] = 0;
			acHexColor[6] = 'f';
			acHexColor[7] = 'f';
			
			ss << "<td style='background-color:#";
			ss << acHexColor;
			ss << ";'></td>";
		}
		ss << "</tr>";
	}
	ss << "</table></td></tr>";
	return writeToLogDirect(ss.str());
}

//**************************************************************************************************************************
//*/
DRReturn DRLogger::writeToLog(const char* pcText, ...)
{
    //Textbuffer zum schreiben
	char acBuffer[1024];
	
	va_list   Argumente;

	//Buffer fuellen
	va_start(Argumente, pcText);
#ifdef _WIN32
	vsprintf_s(acBuffer, pcText, Argumente);
#else 
	vsprintf(acBuffer, pcText, Argumente);
#endif
	va_end(Argumente);
    
    return writeToLog(std::string(acBuffer));
}

DRReturn DRLogger::writeToLog(std::string text)
{
	std::string final = std::string("<tr><td><font size=\"2\" color=\"#000080\">");
    final += text;
    final += std::string("</font></td></tr>");
    
    return writeToLogDirect(final);
}

//***************************************************************************

DRReturn DRLogger::writeToLogDirect(const char* pcText, ...)
{
	//Textbuffer zum schreiben
	char acBuffer[1024];
	
	va_list   Argumente;

	//Buffer fuellen
	va_start(Argumente, pcText);
#ifdef _WIN32
	vsprintf_s(acBuffer, pcText, Argumente);
#else 
	vsprintf(acBuffer, pcText, Argumente);
#endif
	va_end(Argumente);
    
    return writeToLogDirect(std::string(acBuffer));
}

DRReturn DRLogger::writeToLogDirect(std::string text)
{    
	//Datei zum anhängen öffnen (wenn sie es nicht schon ist)
	if(!m_File.isOpen()) m_File.open(m_acFilename, false, "at");

	//wenns nicht geht, Fehler
	if(!m_File.isOpen()) return DR_ERROR;

	m_File.setFilePointer(0, SEEK_END);

    if(m_bPrintToConsole)
    {
        size_t size = text.size();
        char* buffer1 = new char[size+1];
        char* buffer2 = new char[size+1];
		memset(buffer1, 0, sizeof(size+1));
		memset(buffer2, 0, sizeof(size+1));
        if(buffer1 && buffer2)
        {
#ifdef _WIN32
            strcpy_s(buffer1, size+1, text.data());
#else
			strcpy(buffer1, text.data());
#endif
            DRRemoveHTMLTags(buffer1, buffer2, size);
            printf("%s\n", buffer2);
            DR_SAVE_DELETE_ARRAY(buffer1);
            DR_SAVE_DELETE_ARRAY(buffer2);
        }
    }
        
	//ersetzen der \n durch <br>
	std::string final = text;
	while(text.find('\n') != std::string::npos)
		text.replace(text.find('\n'),1,"<br>");

	//einfügen eines Zeilenumbruchs und Formationen
	if(m_File.getFile())
            fprintf(m_File.getFile(), text.data());

	//und Datei schließen (nur im Debug Modus)
#ifdef _DEBUG
//	fclose(m_pFile);
	fflush(m_File.getFile());
	//m_File.close();
//	m_pFile = NULL;
#endif
    
	return DR_OK;
}


//********************************************************************************************************************++

