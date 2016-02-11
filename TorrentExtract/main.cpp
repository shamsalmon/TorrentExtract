#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <string>

using namespace std;

std::string str;

void ListFilesInDir(string dir)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	string ret;

	
	if (dir.find("\"") != std::string::npos)
		dir = dir.substr(1, dir.length() - 2);
	string search = dir + "\\*";
	
	hFind = FindFirstFileA(search.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			ret = string(data.cFileName);
			//If directory recurisve call
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (ret != ".") && (ret != ".."))
			{
				ret = dir + "\\" + ret;
				ListFilesInDir(ret);			
			}
			else if (ret.find(".rar") != std::string::npos)
			{
				//Found our rar
				string rar = "\"" + dir + "\\" + ret + "\"";
				string command = "\"C:\\Program Files\\7-Zip\\7z.exe\" e " + rar + " -o\"" + dir + "\"";

				PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter

				STARTUPINFO StartupInfo; //This is an [in] parameter

				ZeroMemory(&StartupInfo, sizeof(StartupInfo));
				StartupInfo.cb = sizeof StartupInfo; //Only compulsory field

				//Create our 7zip process
				if (CreateProcess(NULL, (LPSTR)command.c_str(),
					NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL,
					NULL, &StartupInfo, &ProcessInfo))
				{
					WaitForSingleObject(ProcessInfo.hProcess, 60000);
					CloseHandle(ProcessInfo.hThread);
					CloseHandle(ProcessInfo.hProcess);
				}
			}
			
			
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       cmdShow){
	const char * dirPath = lpCmdLine;
	str = dirPath;

	MessageBox(0, dirPath, "", 0);
	ListFilesInDir(str);

	return 0;
}