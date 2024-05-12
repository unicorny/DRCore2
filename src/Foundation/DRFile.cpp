#include "DRCore2/Foundation/DRFile.h"
#include "DRCore2/Foundation/DRBaseExceptions.h"
#include "DRCore2/Manager/DRFileManager.h"
#include "DRCore2/DRCore2Main.h"

#include <string>

//Konstruktor, Copykonstruktor und Deskonstruktor
DRFile::DRFile()
: mFile(nullptr), mFileSize(0)
{

}

//-----------------------------------------------------------
#ifdef _WIN32
DRFile::DRFile(const wchar_t* pstFilename, const wchar_t* pstMode/* = L"a+b"*/)
: mFile(nullptr), mFileSize(0)
{
	open(pstFilename, pstMode);
}
#endif

//----------------------------------------------------------------

DRFile::DRFile(const DRFile& otherFile)
: mFile(otherFile.mFile), mFileSize(otherFile.mFileSize)
{
}

//------------------------------------------------------------------

DRFile::DRFile(const char* pcFilename, const char* pcMode/* = "a+b"*/)
: mFile(NULL), mFileSize(0)
{
	open(pcFilename, false, pcMode);
}
//------------------------------------------------------------------


DRFile::~DRFile()
{
	close();
}

//***************************************************************************

//�ffnen einer Datei
#ifdef _WIN32
DRFileErrorCodes DRFile::open(const wchar_t* pstFilename, const wchar_t* pstMode/*= "a+b"*/)
{

	//erstmal schlie�en, da mit wir nicht 2mal �ffnen
	close();

	//�ffnen wenn schon vorhanden, sonst neu erstellen
	//							Filename	lesen und schreiben, teilrecht, datensicherheit, verhalten, erstellungsoptionen (verstckt, etc), Zeiger auf T setzen
//	if ((m_pFile = CreateFileW(stFilename.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)) != INVALIDR_HANDLE_VALUE)
  //      return File_OK;
	auto result = _wfopen_s(&mFile, pstFilename, pstMode);

	if(result) return File_error_File_didnt_exist;
	//Neue Datei erstellen
	//erstellen mit normalen Rechten und allen Datei-Attributen
//	if ((m_pFile = CreateFileW(stFilename.c_str(), GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALIDR_HANDLE_VALUE)
  //      return File_error_cannot_create_File;

	return File_OK;
}
#endif //_WIN32

//---------------------------------------------------------------------------------------------

DRFileErrorCodes DRFile::open(const char* pcFilename, bool bForceNew, const char* pcMode)
{
	//erstmal schlie�en, da mit wir nicht 2mal �ffnen
	close();
	if(!pcFilename) return File_error_NullPointer;

	//�ffnen wenn schon vorhanden, sonst neu erstellen
	//							Filename	lesen und schreiben, teilrecht, datensicherheit, verhalten, erstellungsoptionen (verstckt, etc), Zeiger auf T setzen
	if(!bForceNew)
	{
		mFile = fopen(pcFilename, pcMode);
		if(mFile == NULL)
		{
			const char* pcPfad = DRFileManager::Instance().getWholePfad(pcFilename);
			if(!pcPfad) return File_error_File_didnt_exist;
			//char acFile[512];
			//sprintf(acFile, "%s/%s", pcPfad, pcFilename);
			std::string file(pcPfad);
			file += "/";
			file += pcFilename;
			mFile = fopen(pcFilename, pcMode);
			if(mFile == NULL) return File_error_File_didnt_exist;
			else return File_OK;
		}
		else return File_OK;
	}
	//if ((m_pFile = CreateFile(pcFilename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)) != INVALIDR_HANDLE_VALUE)

	//Neue Datei erstellen
	//erstellen mit normalen Rechten und allen Datei-Attributen
	//if ((m_pFile = CreateFile(pcFilename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALIDR_HANDLE_VALUE)
      //  return File_error_cannot_create_File;
	mFile = fopen(pcFilename, "wb");
	fclose(mFile);
	mFile = fopen(pcFilename, pcMode);

	return File_OK;

}

//*******************************************************************************************

//Und sclie�en derselben
void DRFile::close()
{
	//handle auf die Datei schlie�en
	//dclose_handle(&m_pFile);
	if(!mFile) return;
        fflush(mFile);
	fclose(mFile);
	mFile = NULL;
	mFileSize = 0;
}

//**********************************************************************************************

//Ist die Datei offen?
bool DRFile::isOpen()
{
	return (mFile != NULL);
}

//************************************************************************************************
//Gr��e der Datei bestimmen
size_t DRFile::getSize()
{
	//Wenn die Datei nicht ge�ffnet ist, ist schluss
	if(!isOpen()) return 0;
	if(mFileSize > 0) return mFileSize;

	long lPos = ftell(mFile);
	if(lPos == -1) return 0;

	fseek(mFile, 0, SEEK_END);
	mFileSize = ftell(mFile);
	fseek(mFile, lPos, SEEK_SET);

	if(mFileSize == -1) return 0;

	return mFileSize;
}

//*******************************************************************************************************

//In Datei schreiben
DRFileErrorCodes DRFile::write(const void* pDatenIn, size_t ulSize, size_t ulCount, size_t* plWrittenBytesOut /*= NULL*/)
{
	//wenn datei nicht offen, dann abbruch
	if(!isOpen()) return File_error_file_is_not_open;

	//Wenn pDatenIn Null auch abbruch
	if(!pDatenIn) return File_error_NullPointer;

	//zwischen speicherung
	size_t ulWrittenBytesTemp;

	// Daten schreiben und pr�fen, dass auch alle Daten vollst�ndig in die Datei geschrieben wurde.
  //  const bool bResult = WriteFile(m_pFile, pDatenIn, lSize, &ulWrittenBytesTemp, NULL) && ulWrittenBytesTemp == lSize;
	ulWrittenBytesTemp = fwrite(pDatenIn, ulSize, ulCount, mFile);

	// Wenn die tats�chlich geschriebenen Bytes angefordert werden, werden diese auch �bergeben.
    if(plWrittenBytesOut != NULL)
        (*plWrittenBytesOut) = ulWrittenBytesTemp;

	mFileSize = 0;

    // Funktion mit richtigem Fehlercode beenden, gab es einen Fehler mu� dies bekannt werden.
	return File_OK;
}

//*********************************************************************************************************

//Aus Datei lesen
DRFileErrorCodes DRFile::read(void* pDatenOut, size_t ulSize, size_t ulCount, size_t* plReadedBytesOut /*= NULL*/)
{
	//wenn datei nicht offen, dann abbruch
	if(!isOpen()) return File_error_file_is_not_open;


	//zwischen speicherung
	size_t ulReadedBytesTemp = 0;

	//Aus Dati lesen
	ulReadedBytesTemp = fread(pDatenOut, ulSize, ulCount, mFile);

	// Wenn die tats�chlich gelesenen Bytes angefordert werden, werden diese auch �bergeben.
    if (plReadedBytesOut != NULL)
        (*plReadedBytesOut) = ulReadedBytesTemp;

    // Funktion mit richtigem Fehlercode beenden, gab es einen Fehler mu� dies bekannt werden.
	return File_OK;

}

//*********************************************************************************************************

std::string DRFile::readAsString()
{
	unsigned long size = getSize();
	std::string result;
	result.resize(size, 0);
	auto errorCode = read(result.data(), size, 1);
	if(errorCode) {
		throw DRFileException("error reading file", errorCode);
	}
	return std::move(result);
}

//*****************************************************************************************************************

//File Pointer abfragen
unsigned long DRFile::getFilePointer()
{
	//Datei muss offen sein
	if(!isOpen()) return 0;

	// 64 Bit Integer f�r die Zwischenspeicherung der Position des File-Pointers.
    long lPosition;

     // 64 Bit Integer initialisieren.
    //ZeroMemory (&lPosition, sizeof(lPosition));
    memset(&lPosition, 0, sizeof(lPosition));

	//Aktuelle Position abfragen
	//if(!SetFilePointerEx(m_pFile, llPosition, &llPosition, SEEK_CUR))
	//	return 0;
	//lPosition = _telli64(_fileno(m_pFile));
	lPosition = ftell(mFile);

		// Wiedergabe der Position des File-Pointers.
   // return (static_cast<unsigned long>(llPosition.QuadPart));
	return (unsigned long) lPosition;

}

//***********************************************************************************************************************+

//File Pointer setzen
DRFileErrorCodes DRFile::setFilePointer(s32 ulDistance, unsigned long ulStartPoint /* = SEEK_CUR*/)
{
	//Erstmal Fehler abfragen/verhindern

	//Ist die Datei offen?
	if(!isOpen()) return File_error_file_is_not_open;

	//Ist die Distanz gr��er als die Dateigr��e?
	if(static_cast<unsigned long>(abs(ulDistance)) > getSize()) return File_error_pointer_distance_to_great;

	//Ist die Start Position die aktuelle Position?
	if(ulStartPoint == SEEK_CUR)
	{
		//Wenn die Distanz von der aktuellen Position gr��er als die
		//Datei ist, dann Abbruch
		if(ulDistance + getFilePointer() > getSize())
			return File_error_pointer_distance_to_great;
	}

	//-----------------------------------------------------------

	//Zwischenspeicher mit init
	//LARGE_INTEGER llTemp;
	long lTemp;
	memset(&lTemp, 0, sizeof(lTemp));

	//Distanz zuweisen
	//llTemp.QuadPart = ulDistance;
	lTemp = ulDistance;

	//Setzen der Position des File Pointers
	//if(!SetFilePointerEx(m_pFile, lTemp, NULL, ulStartPoint))
	//	return File_error_could_not_set_pointer;
	//lTemp = _lseeki64(_fileno(m_pFile), (__int64)ulDistance, (__int64)ulStartPoint);
	lTemp = fseek(mFile, ulDistance, ulStartPoint);
	if(lTemp < 0) return File_error_could_not_set_pointer;

	return File_OK;
}

//*************************************************************************************
//File Utils
//*************************************************************************************

bool DRIsFileExist(const char* pcFilename)
{
    const char* path = DRFileManager::Instance().getWholePfad(pcFilename);
	FILE* pFile = NULL;//fopen(pcFilename, "rb");
    
	if(!path)
	{
        pFile = fopen(pcFilename, "rb");
	}
	else
	{
        pFile = fopen(std::string(std::string(path)+"/"+pcFilename).data(), "rb");
	}
    
	if(pFile)
	{
		fclose(pFile);
		return true;
	}
	//LOG("%s nicht vorhanden", pcFilename);
	return false;
}

std::string DRFileErrorCodeToString(DRFileErrorCodes errorCode)
{
	switch (errorCode)
	{
	case File_OK:
		return "File_OK";
	case File_error_cannot_create_File:
		return "File_error_cannot_create_File";
	case File_error_file_is_not_open:
		return "File_error_file_is_not_open";
	case File_error_could_not_write_all:
		return "File_error_could_not_write_all";
	case File_error_could_not_read_all:
		return "File_error_could_not_read_all";
	case File_error_NullPointer:
		return "File_error_NullPointer";
	case File_error_pointer_distance_to_great:
		return "File_error_pointer_distance_to_great";
	case File_error_could_not_set_pointer:
		return "File_error_could_not_set_pointer";
	case File_error_could_not_get_pointer:
		return "File_error_could_not_get_pointer";
	case File_error_File_didnt_exist:
		return "File_error_File_didnt_exist";
	default:
		return "Unknown error code";
	}
}
