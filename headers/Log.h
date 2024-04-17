#pragma once

#include <string>

using namespace std;

class Log {
protected:
	const string logFile="./logs.txt";
	string text;
public:
	virtual void writeLog(void);
	Log(string text);
};