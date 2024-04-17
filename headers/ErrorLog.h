#pragma once

#include <string>
#include "Log.h"

using namespace std;

class ErrorLog : public Log {
public:
	void writeLog(void);
	ErrorLog(string text);
};