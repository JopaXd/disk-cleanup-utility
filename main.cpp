#include <iostream>
#include <cstdlib>
#include <limits>
#include <filesystem>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include "headers/Directory.h"
#include "headers/File.h"
#include "headers/FSItem.h"
#include "headers/CleanupUtils.h"

#define DEBUG 1
// #define APT_CACHE = "/var/cache/apt/archives";

using namespace std;

void clear(void);
uintmax_t directory_size(string path);
void clear_input_buffer();
int create_log_file();
bool startsWith(const string& str, const string& prefix);
unique_ptr<Directory> loadDirectories(const string& path, Directory* parentDir = nullptr);

int main() {
	clear();
	create_log_file();
	int choice;
	while (true){
		cout << "*** FS CleanUp Utility***" << endl;
		cout << "1. Scan a directory. (requires root depending on the task)" << endl;
		cout << "2. Empty recycle bin." << endl;
		cout << "3. Delete app cache." << endl;
		cout << "4. Delete app logs. (requires root)" << endl;
		cout << "5. Show logs" << endl;
		cout << "6. Exit" << endl;
		cin >> choice;
		clear_input_buffer();
		//When something that is not a number is entered, it returns 0.
		if (choice == 1) {
			string path;
			int index = 0;
			int scanChoice = 0;
			cout << "Enter the path: (/ for root, this might take a considerable amount of time. Root is also required to access protected parts of the filesystem.)" << endl;
			while (true){
				cin >> path;
				clear_input_buffer();				
				if (!filesystem::exists(path)){
					cout << "The path you entered does not exist!" << endl;
				}
				else if (filesystem::is_regular_file(path)){
					cout << "This is a file, not a directory!" << endl;
				}
				else{
					break;
				}
			}
			unique_ptr<Directory> rootDir = loadDirectories(path);
			Directory* currentDir = nullptr;		
			while (true) {
				if (currentDir == nullptr){
					currentDir = rootDir.get();
				}
				vector<unique_ptr<FSItem>>& dirContents = currentDir->getContents();
				index = 0;
				for (auto& i : currentDir->getContents()) {
					cout << index << " ";
					i->print();
					index++;
				}
				if (currentDir->getParent() == nullptr){
					cout << endl << endl << endl;
					cout << "Contents of:" << currentDir->getPath() << endl;
					cout << endl << endl << endl;
					cout << "1. Sort by size (ascending)" << endl;
					cout << "2. Sort by size (descending)" << endl;
					cout << "3. Select an item." << endl;
					cout << "4. Wipe folder." << endl;
					cout << "5. Back to main menu." << endl;
				}
				else{
					cout << endl << endl << endl;
					cout << "Contents of:" << currentDir->getPath() << endl;
					cout << endl << endl << endl;
					cout << "1. Sort by size (ascending)" << endl;
					cout << "2. Sort by size (descending)" << endl;
					cout << "3. Select an item." << endl;
					cout << "4. Wipe folder." << endl;
					cout << "5. Back." << endl;
					cout << "6. Back to main menu." << endl;
				}

				cin >> scanChoice;
				clear_input_buffer();

				if (scanChoice == 1) {
					sort(dirContents.begin(), dirContents.end(), [](unique_ptr<FSItem>& a, unique_ptr<FSItem>& b){ return *a < *b; });
					clear();
				}
				else if (scanChoice == 2) {
					sort(dirContents.begin(), dirContents.end(), [](unique_ptr<FSItem>& a, unique_ptr<FSItem>& b){ return *a > *b; });
					clear();
				}
				else if(scanChoice == 3) {
					int itemIndex = 0;
					while(true){
						cout << "Select index of item you want to work with." << endl;
						cin >> itemIndex;
						clear_input_buffer();
						if (itemIndex > dirContents.size()){
							cout << "Please select a valid index." << endl;
							continue;
						}
						else{
							break;
						}
					}
					unique_ptr<FSItem>& chosenItem = dirContents[itemIndex];
					cout << endl << endl;
					chosenItem->print();
					cout << endl << endl;
					int operation;
					if (chosenItem->type() == "file"){
						cout << "What would you like to do?" << endl;
						cout << "1. Delete file" << endl;
						cout << "2. Back" << endl;;
						while(true){
							cin >> operation;
							clear_input_buffer();
							if (operation == 1) {
								clear();
								int success = chosenItem->del();
								if (success == 0) {
									cout << endl;
									cout << "----------------" << endl;
									cout << "File deleted successfully!" << endl;
									cout << "----------------" << endl;
									vector<unique_ptr<FSItem>>& dirContents = currentDir->getContents();
									dirContents.erase(dirContents.begin() + itemIndex);
									// currentDir->setContents(dirContents);
								}
								else{
									cout << endl;
									cout << "----------------" << endl;
									cout << "Error deleting file!" << endl;
									cout << "----------------" << endl;
								}
								break;
							}
							else if (operation == 2) {
								break;
							}
							else{
								cout << "Invalid option!" << endl;
								continue;
							}
						}
					}
					else if (chosenItem->type() == "dir"){
						cout << "What would you like to do?" << endl;
						cout << "1. Delete folder" << endl;
						cout << "2. Scan folder" << endl;
						cout << "3. Back" << endl;
						while(true){
							cin >> operation;
							clear_input_buffer();
							if (operation == 1) {
								clear();
								int success = chosenItem->del();
								if (success == 0) {
									cout << endl;
									cout << "----------------" << endl;
									cout << "Folder deleted successfully!" << endl;
									cout << "----------------" << endl;
									int itemIndex = 0;
									dirContents.erase(dirContents.begin() + itemIndex);
								}
								else{
									cout << endl;
									cout << "----------------" << endl;
									cout << "Error deleting folder!" << endl;
									cout << "----------------" << endl;
								}
								break;
							}
							else if (operation == 2) {
								currentDir = dynamic_cast<Directory*>(currentDir->getContents()[itemIndex].get());
								break;
							}
							else if (operation == 3) {
								break;
							}
						}
					}
				}
				else if (scanChoice == 4) {
					clear();
					if (currentDir->getParent() != nullptr) {
						int success = currentDir->del();
						if (success == 0) {
							cout << endl;
							cout << "----------------" << endl;
							cout << "Folder deleted successfully!" << endl;
							cout << "----------------" << endl;
							vector<unique_ptr<FSItem>>& parentContents = currentDir->getParent()->getContents();
							parentContents.erase(std::remove_if(parentContents.begin(), parentContents.end(), [&](auto const& fsi){ return fsi->getPath() == currentDir->getPath(); }), parentContents.end());
							currentDir = currentDir->getParent();
						}
						else{
							cout << endl;
							cout << "----------------" << endl;
							cout << "Error deleting folder!" << endl;
							cout << "----------------" << endl;
						}
						continue;
					}
					else{
						//Deleting the initially scanned path.
						int success = currentDir->del();
						if (success == 0) {
							cout << endl;
							cout << "----------------" << endl;
							cout << "Folder deleted successfully!" << endl;
							cout << "----------------" << endl;
						}
						else{
							cout << endl;
							cout << "----------------" << endl;
							cout << "Error deleting folder!" << endl;
							cout << "----------------" << endl;
						}
						//Back to main menu
						break;
					}
				}
				else if (scanChoice == 5) {
					if (currentDir->getParent() == nullptr){
						//Back to main menu.
						clear();
						break;
					}
					else{
						currentDir = currentDir->getParent();
					}
				}
				else if (scanChoice == 6) {
					if (currentDir->getParent() == nullptr) {
						continue;
					}
					else{
						//Back to main menu.
						clear();
						break;
					}
				}
			}
		}
		else if (choice == 2) {
			int success = CleanupUtils::emptyRecycleBin();
			if (success == 0){
				cout << "Trash emptied";
			}
			else{
				cout << "Error deleting trash;";
			}
		}
		else if (choice == 3) {
			CleanupUtils::clearAppCache();
		}
		else if (choice == 4) {
			CleanupUtils::clearAppLogs();
		}
		else if (choice == 5) {
			clear();
			int logChoice;
			int showingInfo = 0;
			int showingErrors = 0;
			logChoice = 0;
			vector<string> filtered_logs;
			ifstream log_file("./logs.txt");
			string line;
			vector<string> logs;
			while (getline(log_file, line)) {
				cout << line << endl;
				logs.push_back(line);
			}
			log_file.close();
			while (true) {
				cout << endl << endl << endl;
				cout << "What would you like to do?" << endl;
				if (!showingInfo && !showingErrors){
					cout << "1. Show only errors" << endl;
					cout << "2. Show only info" << endl;
					cout << "3. Clear logs" << endl;
					cout << "4. Back" << endl;
				}
				else if (showingInfo){
					cout << "1. Show only errors" << endl;
					cout << "2. Show all logs" << endl;
					cout << "3. Clear logs" << endl;
					cout << "4. Back";
				}
				else if (showingErrors) {
					cout << "1. Show only info" << endl;
					cout << "2. Show all logs" << endl;
					cout << "3. Clear logs" << endl;
					cout << "4. Back";
				}
				cin >> logChoice;
				clear_input_buffer();
				filtered_logs.clear();
				if (logChoice == 1) {
					clear();
					if (!showingInfo && !showingErrors){
						copy_if(logs.begin(), logs.end(), back_inserter(filtered_logs),
							[](const string& word) {
								string prefix = "[ERROR]"; 
								return word.compare(0, prefix.size(), prefix) == 0;
							});
						for (string log : filtered_logs) {
							cout << log << endl;
						}
						showingErrors = 1;
						showingInfo = 0;
						continue;
					}
					else if (showingInfo){
						copy_if(logs.begin(), logs.end(), back_inserter(filtered_logs),
							[](const string& word) {
								string prefix = "[ERROR]"; 
								return word.compare(0, prefix.size(), prefix) == 0;
							});
						for (string log : filtered_logs) {
							cout << log << endl;
						}
						showingErrors = 1;
						showingInfo = 0;
						continue;
					}
					else if(showingErrors){
						copy_if(logs.begin(), logs.end(), back_inserter(filtered_logs),
							[](const string& word) {
								string prefix = "[INFO]"; 
								return word.compare(0, prefix.size(), prefix) == 0;
							});
						for (string log : filtered_logs) {
							cout << log << endl;
						}
						showingErrors = 0;
						showingInfo = 1;
						continue;
					}
				}
				else if (logChoice == 2) {
					clear();
					if (!showingInfo && !showingErrors){
						copy_if(logs.begin(), logs.end(), back_inserter(filtered_logs),
							[](const string& word) {
								string prefix = "[INFO]"; 
								return word.compare(0, prefix.size(), prefix) == 0;
							});
						for (string log : filtered_logs) {
							cout << log << endl;
						}
						showingErrors = 0;
						showingInfo = 1;
						continue;
					}
					else if (showingInfo || showingErrors){
						for (string log : logs) {
							cout << log << endl;
						}
						showingErrors = 0;
						showingInfo = 0;
						continue;
					}
				}
				else if (logChoice == 3) {
					clear();
					ofstream ofs;
					ofs.open("./logs.txt", ofstream::out | ofstream::trunc);
					ofs.close();
					cout << "Cleared." << endl; 
					break;
				}
				else if (logChoice == 4) {
					clear();
					break;
				}
			}
		}
		else if (choice == 6) {
			cout << "Goodbye!" << endl;
			break;
		}
		else {
			//Clear the input buffer so that the programd oes not go into an infinite loop.
			clear();
			clear_input_buffer();
			cout << "Please select a valid option." << endl;
		}
	}
	return 0;
}

void clear(){
	system("clear");
}

unique_ptr<Directory> loadDirectories(const string& path, Directory* parentDir) {
    size_t found = path.find_last_of("/");
    uintmax_t currentDirSize = directory_size(path);
    string dirName = path.substr(found + 1);
    unique_ptr<Directory> dir = make_unique<Directory>(dirName, path, currentDirSize);
    dir->setParent(parentDir); // Set the parent directory

    vector<unique_ptr<FSItem>> fs;
    for (const auto& entry : filesystem::directory_iterator(path)) {
        if (filesystem::is_directory(entry)) {
            uintmax_t subdir_size = directory_size(entry.path());
            unique_ptr<Directory> subdir = make_unique<Directory>(entry.path().filename().string(), entry.path().string(), subdir_size);
            subdir = loadDirectories(entry.path().string(), dir.get()); // Pass dir as the parent directory
            fs.push_back(move(subdir));
        } else {
            fs.push_back(make_unique<File>(entry.path().filename().string(), entry.path().string(), filesystem::file_size(entry)));
        }
    }
    dir->setContents(move(fs));
    return dir;
}

uintmax_t directory_size(string path) {
	uintmax_t size = 0;
	for (const auto& entry : filesystem::recursive_directory_iterator(path)) {
		if (!filesystem::is_directory(entry)) {
			try{
				size += filesystem::file_size(entry);
			}
			//Failed to read for various reasons... (Permissions, fs errors, etc...)
			catch (filesystem::filesystem_error &exc){
				if (DEBUG){
					cout << "ERROR!" << " " << exc.what() << endl;
				}
				continue;
			}
		}
	}
	return size;
}

void clear_input_buffer() {
	//Clear the input buffer so that the programd oes not go into an infinite loop.
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//This checks if the file exists, and if not, creates one.
int create_log_file() {
	if (!filesystem::exists("./logs.txt")){
		fstream file;
		file.open("./logs.txt", ios::out);
		if (!file){
			return 1;
		}
	}
	return 0;
}

bool startsWith(const string& str, const string& prefix) {
	return str.find(prefix) == 0;
}