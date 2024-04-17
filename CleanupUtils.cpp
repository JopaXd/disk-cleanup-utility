#include "headers/CleanupUtils.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

const char* homeDir = getenv("HOME");

const string CleanupUtils::BIN_PATH = string(homeDir) + "/.local/share/Trash/";
const string CleanupUtils::APP_CACHE_PATH = string(homeDir) + "/.cache/";
const string CleanupUtils::APP_LOGS = "/var/log/";

void CleanupUtils::delete_dir_content(const filesystem::path& dir){
	for (const auto& entry: filesystem::directory_iterator(dir)) {
		filesystem::remove_all(entry.path());
	}
}

int CleanupUtils::emptyRecycleBin(void) {
	try{
		filesystem::remove(BIN_PATH + "directorysizes");
		delete_dir_content(BIN_PATH + "files");
		delete_dir_content(BIN_PATH + "info");
	}
	catch(filesystem::filesystem_error &exc) {
		return 1;
	}
	return 0;
}


int CleanupUtils::clearAppCache(void) {
	try{
		delete_dir_content(APP_CACHE_PATH);
	}
	catch(filesystem::filesystem_error &exc) {
		return 1;
	}
	return 0;
}


int CleanupUtils::clearAppLogs(void) {
	try{
		delete_dir_content(APP_LOGS);
	}
	catch(filesystem::filesystem_error &exc) {
		return 1;
	}
	return 0;
}
