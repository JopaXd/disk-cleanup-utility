#include <fstream>
#include <string>
#include "headers/ErrorLog.h"

using namespace std;

ErrorLog::ErrorLog(string text) : Log(text) {
}

void ErrorLog::writeLog(){
	ofstream outfile;
	outfile.open(logFile, std::ios_base::app);
	outfile << "[ERROR] " <<  text << endl; 
	outfile.close();
}