//	
//	ofxThreadedLogger
//
//  LoggerThread.cpp
//
//  Created by Sean Montgomery on 2016-05-29
//  http://produceconsumerobot.com/
//
//  This work is licensed under the Creative Commons 
//  Attribution-ShareAlike 3.0 Unported License. 
//  To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
//


/*
 * LoggerThread.cpp
 *
 *  Created on: 2016-05-29
 *      Author: ProduceConsumeRobot
 */

#include "ofxThreadedLogger.h"
#include "ofConstants.h"

LoggerThread::LoggerThread()
: logFilePath(ofToDataPath("") + "log.txt"){
	// start the thread as soon as the
	// class is created, it won't use any CPU
	// until we send a new frame to be analyzed
	startThread();
}

LoggerThread::~LoggerThread(){
	// when the class is destroyed
	// close both channels and wait for
	// the thread to finish
	toLog.close();
	waitForThread(true);
}

void LoggerThread::log(string & logString){
	// send the line to the thread for logging
	// this makes a copy 
	bool temp = toLog.send(logString);
}

void LoggerThread::threadedFunction() {
	// wait until there's a new frame
	// this blocks the thread, so it doesn't use
	// the CPU at all, until a frame arrives.
	// also receive doesn't allocate or make any copies
	string logString;
	while (toLog.receive(logString)) {
		// Write the string to the log
		ofstream mFile;
		mFile.open(logFilePath, ios::out | ios::app);
		mFile << logString;
		mFile.close();
		ofSleepMillis(2);
	}
	
}

int LoggerThread::setPath(string filePath) {
	// ToDo:: Add error checking
	logFilePath = filePath;
	return 0;
}

