#pragma once

#include <filesystem>

using namespace std;


class CleanupUtils {
private:
	static const string BIN_PATH;
	static const string APP_CACHE_PATH;
	static const string APP_LOGS;
	static void delete_dir_content(const filesystem::path& dir);

public:
	static int emptyRecycleBin(void);
	static int clearAppCache(void);
	static int clearAppLogs(void);
};