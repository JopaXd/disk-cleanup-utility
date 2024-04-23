#include <fstream>
#include <string>
#include "headers/Log.h"

using namespace std;

Log::Log(string text) {
	this->text = text;
}

void Log::writeLog(){
	ofstream outfile;
	outfile.open(logFile, ios_base::app);
	outfile << "[LOG] " <<  text << endl; 
	outfile.close();
}