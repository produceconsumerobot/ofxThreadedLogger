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

//#include <algorithm>
//#include <vector>

#include "ofMain.h"

//#define LOGGER_THREAD_DEBUG // Comment out prior to release


class LoggerThread : public ofThread {
private:
	//string _logFilePath;		// Path of log file
	
	string _logDirPath;			// Path of the directory to put the log file in
	string _logfilename;		// Name of the log file
	string _tempLogDirPath;		// Temporary path to avoid collisions if path changes
	string _tempLogfilename;	// Temporary name to avoid collisions if filename changes
	
	queue<string> queue1;		// Multiple queues to reduce logging delays
	queue<string> queue2;		// Multiple queues to reduce logging delays

	queue<string> * pushQueue;	// Pointer to queue that's accepting incoming data
	queue<string> * popQueue;	// Pointer to queue that's being written

	size_t _pushThrottlingSize = SIZE_MAX / 2; // queue size trigger to impose a queue popping delay
	int _loopSleep = 4;

	void threadedFunction();	
	void log(string logString);
	void pop();
	void popAll();
	void swapQueues();

public:
	static string fileDateTimeString(unsigned long long ofTime);	// Deprecated, use ofGetTimestampString()

	//LoggerThread();
	LoggerThread(string logDirPath = ofToDataPath(""), string filename = "log.txt");
	//LoggerThread(string logDirPath="../LogData/", string fileName=fileDateTimeString(ofGetElapsedTimeMillis()));
	~LoggerThread();
	//int setPath(string filePath);			// Set the path of the log file
	void setDirPath(string logDirPath);		
	void setFilename(string filename);		
	void push(string logString);
	void stopThread();

	enum LoggerQueue 
	{
		PUSH,
		POP
	};

	/*!
		@brief Sets the queue size above which pushes are delayed to allow queue to pop
		@return Size of specified queue
	*/
	size_t size(LoggerQueue lq);

	/*!
		@brief Returns size of PUSH or POP queue
		@param pushThrottlingSize Queue size trigger to impose a queue popping delay
	*/
	void setPushThrottlingSize(size_t pushThrottlingSize = SIZE_MAX / 2);
};




