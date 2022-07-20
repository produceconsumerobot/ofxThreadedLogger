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


#include "ofxThreadedLogger.h"

// ------------------------------------------------------- 
// LoggerThread(string logDirPath, string logfilename)
// -------------------------------------------------------
LoggerThread::LoggerThread(string logDirPath, string logfilename) 
{
	_logDirPath = logDirPath;
	_logfilename = logfilename;
	_tempLogDirPath = logDirPath;
	_tempLogfilename = logfilename;

	pushQueue = &queue1;
	popQueue = &queue2;
}
	
LoggerThread::~LoggerThread() {
	// Stop the thread if it's still running
	ofLog(OF_LOG_VERBOSE, "LoggerThread::~LoggerThread()");
	stopThread();
}

void LoggerThread::stopThread()
{
	ofLog(OF_LOG_VERBOSE, "LoggerThread::stopThread()");
	//ofThread::stopThread();
	if (isThreadRunning()) {
		waitForThread(true);
	}
	swapQueues();
	popAll();
}

void LoggerThread::setDirPath(string logDirPath)
{
	lock();
	_tempLogDirPath = logDirPath;
	unlock();
}

void LoggerThread::setFilename(string logfilename)
{
	lock();
	_tempLogfilename = logfilename;
	unlock();
}


void LoggerThread::log(string logString) {

	ofDirectory dir(_logDirPath);
	dir.create(true);
	//_mkdir( _logDirPath.c_str() );//, S_IRWXU | S_IRWXG | S_IRWXO);
    string filename = _logDirPath + _logfilename;
    ofstream mFile;
	mFile.open(filename.c_str(), ios::out | ios::app);
	mFile << logString;
    mFile.close();
}

void LoggerThread::pop() {
	if (!popQueue->empty()) {
		log(popQueue->front());
		popQueue->pop();
	}
}

void LoggerThread::popAll() 
{
	while (!popQueue->empty()) {
		string logString = "";
		while (!popQueue->empty() && (logString.length() + popQueue->front().length() < logString.max_size()))
		{
			logString = logString + popQueue->front();
			popQueue->pop();
		}
		log(logString);
	}
}

void LoggerThread::push(string logString) 
{
	// ToDo: There may be ways to substantially optimize the push throttling
	if (pushQueue->size() > _pushThrottlingSize || popQueue->size() > _pushThrottlingSize)
	{
		while (pushQueue->size() > 0 || popQueue->size() > 0)
		{
			ofSleepMillis(1);
			#ifdef LOGGER_THREAD_DEBUG 
			cout << "*";
			#endif
		}
	}

	lock();
	pushQueue->push(logString);
	unlock();
}

void LoggerThread::threadedFunction() 
{
	while (isThreadRunning()) {
		lock();
		// Swap the push queue and the pop queue to permit pushing while
		// performing slow write operations
		swapQueues();

		// Update the log file path 
		_logDirPath = _tempLogDirPath;
		_logfilename = _tempLogfilename;
		unlock();

		// pop queue is now safe from push thread collisions
		popAll();

		sleep(4);
	}
}

void LoggerThread::swapQueues()
{
	if (pushQueue == &queue1)
	{
		pushQueue = &queue2;
		popQueue = &queue1;
	}
	else {
		pushQueue = &queue1;
		popQueue = &queue2;
	}
}

// Deprecated, use ofGetTimestampString()
string LoggerThread::fileDateTimeString(unsigned long long ofTime)
{
	string output = "";

	int year = ofGetYear();
	int month = ofGetMonth();
	int day = ofGetDay();
	int hours = ofGetHours();
	int minutes = ofGetMinutes();
	int seconds = ofGetSeconds();

	output = output + ofToString(year) + ".";
	if (month < 10) output = output + "0";
	output = output + ofToString(month) + ".";
	if (day < 10) output = output + "0";
	output = output + ofToString(day) + ", ";
	if (hours < 10) output = output + "0";
	output = output + ofToString(hours) + ".";
	if (minutes < 10) output = output + "0";
	output = output + ofToString(minutes) + ".";
	if (seconds < 10) output = output + "0";
	output = output + ofToString(seconds) + ", ";
	output = output + ofToString(ofTime - (ofTime / 1000));

	return output;
}

size_t LoggerThread::size(LoggerQueue lq)
{
	size_t out;
	lock();
	if (lq == LoggerQueue::PUSH)
	{
		out = pushQueue->size();
	}
	if (lq == LoggerQueue::POP)
	{
		out = popQueue->size();
	}
	unlock();
	return out;
}

void LoggerThread::setPushThrottlingSize(size_t pushThrottlingSize)
{
	_pushThrottlingSize = pushThrottlingSize;
}
