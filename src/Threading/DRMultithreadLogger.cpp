#include "DRCore2/Threading/DRMultithreadLogger.h"
#include "DRCore2/Foundation/DRUtils.h"

#include <cstdarg>
  
DRMultithreadLogger::DRMultithreadLogger()
	: DRThread(nullptr)
{

}

DRMultithreadLogger::~DRMultithreadLogger()
{

}

DRReturn DRMultithreadLogger::init(const char* pcFilename, bool printToConsole)
{
    gStartTimePoint.reset();
    setName(pcFilename);
    DRReturn ret = DRLogger::init(pcFilename, printToConsole);
    DRThread::init();    
    return ret;
}

void DRMultithreadLogger::exit()
{
    DRThread::exit();
    DRLogger::exit();
}

const char* DRMultithreadLogger::getTimeSinceStart()
{
    double seconds = gStartTimePoint.seconds();
    double minutes = 0.0;
    seconds = modf(seconds / 60.0, &minutes);
    seconds *= 60.0;
    sprintf(mTimeStringBuffer, "[%.0f:%02.0f] ", minutes, seconds);

    return mTimeStringBuffer;
}

DRReturn DRMultithreadLogger::writeToLogDirect(std::string text)
{
    DRReturn ret = DR_OK;
            
    //DRLogger::writeToLogDirect(text);
	if (text.size() > 0) {
		mMessages.push(text);
		condSignal();
	}
    return ret;
}

DRReturn DRMultithreadLogger::writeToLogDirect(const char* pcText, ...)
{
    //Textbuffer zum schreiben
    char acBuffer[1024]; memset(acBuffer, 0, 1024);

    va_list   Argumente;

    //Buffer fuellen
    va_start(Argumente, pcText);
    vsprintf(acBuffer, pcText, Argumente);
    va_end(Argumente);

    std::lock_guard _lock(mTimeStringBufferMutex);
    std::string final = getTimeSinceStart();
    final += std::string(acBuffer);

    return writeToLogDirect(final);
}

DRReturn DRMultithreadLogger::writeToLog(const char* pcText, ...)
{
    //Textbuffer zum schreiben
    char acBuffer[1024];

    va_list   Argumente;

    //Buffer fuellen
    va_start(Argumente, pcText);
    vsprintf(acBuffer, pcText, Argumente);
    va_end(Argumente);
    std::lock_guard _lock(mTimeStringBufferMutex);
    std::string final = getTimeSinceStart();
    final += std::string(acBuffer);

    return writeToLog(final);
}
DRReturn DRMultithreadLogger::writeToLog(std::string text)
{
    std::string final = std::string("<tr><td><font size=\"2\" color=\"#000080\">");
    final += text;
    final += std::string("</font></td></tr>");

    return writeToLogDirect(final);
}
       
DRReturn DRMultithreadLogger::writeAsBinary(std::string name, u8 zahl)
{
    return writeAsBinary_intern(name, getValueAsBinaryString(zahl));
}
DRReturn DRMultithreadLogger::writeAsBinary(std::string name, u16 zahl)
{
    u8 zahl1 = (u8)zahl;
    u8 zahl2 = (u8)(zahl >> 8);
    std::string zahlBuffer = getValueAsBinaryString(zahl1) + " " + getValueAsBinaryString(zahl2);
    return writeAsBinary_intern(name, zahlBuffer);
}
DRReturn DRMultithreadLogger::writeAsBinary(std::string name, u32 zahl)
{
    u8 zahlen[4];
    std::string zahlBuffer;
    for(int i = 0; i < 4; i++) {
        zahlBuffer += getValueAsBinaryString((u8)(zahl >> 8*i));
        if(i < 3) zahlBuffer += std::string(" ");
    }
    return writeAsBinary_intern(name, zahlBuffer);
}
DRReturn DRMultithreadLogger::writeAsBinary(std::string name, u64 zahl)
{
    u8 zahlen[8];
    std::string zahlBuffer;
    for(int i = 0; i < 8; i++) {
        zahlBuffer += getValueAsBinaryString((u8)(zahl >> 8*i));
        if(i < 7) zahlBuffer += std::string(" ");
    }
    return writeAsBinary_intern(name, zahlBuffer);
}

DRReturn DRMultithreadLogger::writeAsBinary_intern(std::string name, std::string zahlBuffer)
{
    std::string final = std::string("<tr><td><font size=\"2\" color=\"#0000f0\">");
    final += name;
    final += std::string("</font>");
    final += zahlBuffer;
    final += std::string("</td></tr>");
    return writeToLogDirect(final.data());
}

int DRMultithreadLogger::ThreadFunction()
{
	std::string text;
	while (mMessages.pop(text)) {
		if (DRLogger::writeToLogDirect(text)) {
			printf("\n=========== error by writing into logfile ============\n\n");
			return -1;
		}
	}

	return 0;
}
