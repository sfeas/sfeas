//Author		: Xiao Tuzi
//Date(Create)	: 09/28/2022
//Description	: this function is getting file name from windows


#include<windows.h>
#include "SFEAS/Tools/getFileFromWindows.h"

std::string getFileFromWindow() {
	OPENFILENAME ofn = { 0 };
	char szFile[256] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"dat\0*.dat\0";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	std::string fileName = "";
	if (GetOpenFileName(&ofn)) {
		// change Lpcwstr to String
		int len = WideCharToMultiByte(CP_ACP, 0, ofn.lpstrFile, -1, NULL, 0, NULL, NULL);
		char* dest = new char[len];
		WideCharToMultiByte(CP_ACP, 0, ofn.lpstrFile, -1, dest, len, NULL, NULL);
		dest[len - 1] = 0;
		std::string str(dest);
		delete[] dest;
		fileName = str;
	}
	return fileName;

}