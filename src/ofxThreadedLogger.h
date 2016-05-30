//	
//	ofxThreadedLogger
//
//  LoggerThread.h
//
//  Created by Sean Montgomery on 2016-05-29
//  http://produceconsumerobot.com/
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//

#pragma once
#include "ofMain.h"

class LoggerThread: public ofThread {
public:
	LoggerThread();
	~LoggerThread();
	void log(string logString);
	int setPath(string filePath);
	//bool isLineNew();

private:
	void threadedFunction();
	ofThreadChannel<string> toLog;
	string logFilePath;
	//string logLine;
	//bool newLine;
};
