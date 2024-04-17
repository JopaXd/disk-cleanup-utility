#include <fstream>
#include <string>
#include "headers/InfoLog.h"

using namespace std;

InfoLog::InfoLog(string text) : Log(text) {
}

void InfoLog::writeLog(){
	ofstream outfile;
	outfile.open(logFile, std::ios_base::app);
	outfile << "[INFO] " <<  text << endl; 
	outfile.close();
}