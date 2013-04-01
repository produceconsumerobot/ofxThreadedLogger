//
//  telephoneRewired.h
//
//  Created by Sean Montgomery on 12/18/12.
//  http://produceconsumerobot.com/
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//

#ifndef _OFX_THREADED_LOGGER
#define _OFX_THREADED_LOGGER

#include <algorithm>
#include <vector>

#include "ofMain.h"


class LoggerThread : public ofThread {
private:
	string _logDirPath;
	string _fileName;
public:
	static string fileDateTimeString(unsigned long long ofTime);

	LoggerThread(string logDirPath="../LogData/", string fileName=fileDateTimeString(ofGetElapsedTimeMillis()));
	~LoggerThread();
	void setDirPath(string logDirPath);
	void setFileName(string fileName);
	void log(string data);
	void pop();
	void popAll();

	void threadedFunction();

	queue<string> loggerQueue;
};

#endif


