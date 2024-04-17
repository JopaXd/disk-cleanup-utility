#pragma once


#include <string>
#include "Log.h"

using namespace std;

class InfoLog : public Log {
public:
	void writeLog(void);
	InfoLog(string text);
};