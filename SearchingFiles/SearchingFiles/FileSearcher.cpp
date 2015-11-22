#include "stdafx.h"
#include "FileSearcher.h"
#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include "Shlwapi.h"

using namespace std;
#pragma comment(lib, "User32.lib")

#define BUFSIZE MAX_PATH
TCHAR szBuffer[BUFSIZE + 1]; 
TCHAR buf[BUFSIZE + 1]; 

FileSearcher *FileSearcher::s_instance = 0;

FileSearcher::FileSearcher(){
	if (GetCurrentDirectory(BUFSIZE, szBuffer) == 0) 
	{
		// Handle an error condition.
		_tprintf (TEXT("GetLongPathName failed (%d)\n"), GetLastError());
		return;
	}else{
		this->dir = &szBuffer[0];
	}
}

FileSearcher::FileSearcher(TCHAR dir)
{
	this->dir = &dir;
	DWORD dwRet;
	dwRet = GetCurrentDirectory(BUFSIZE, szBuffer);

	if( dwRet == 0 )
	{
		_tprintf (TEXT("GetCurrentDirectory failed (%d)\n"), GetLastError());
		return;
	}
	if(dwRet > BUFSIZE)
	{
		_tprintf (TEXT("Buffer too small; need %d characters\n"), dwRet);
		return;
	}

	if(!SetCurrentDirectory(&dir))
	{
		_tprintf (TEXT("SetCurrentDirectory failed (%d)\n"), GetLastError());
		return;
	}
	_tprintf(TEXT("Set current directory to %s\n"), dir);

	if(!SetCurrentDirectory(szBuffer) )
	{
		_tprintf (TEXT("SetCurrentDirectory failed (%d)\n"), GetLastError());
		return;
	}
	_tprintf(TEXT("Restored previous directory (%s)\n"), szBuffer);
}


FileSearcher::~FileSearcher(void)
{
	if(this->dir!= NULL){
		delete this->dir;
	}
}

void FileSearcher::ChangeDirectory(TCHAR *dir){
	this->dir = dir;
}

void FileSearcher::Search(TCHAR fileName){
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;


	StringCchCopy(szDir, MAX_PATH, &fileName);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(dir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		DisplayErrorBox(TEXT("FindFirstFile"));      
	} 
	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
}


void FileSearcher::FindFilesRecursively(LPCTSTR lpFolder, LPCTSTR lpFilePattern)
{
	TCHAR szFullPattern[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;
	// first we are going to process any subdirectories
	PathCombine(szFullPattern, lpFolder, _T("*"));//FindFilesRecursively(_T("C:\\WINDOWS"), _T("*.wav"));
	hFindFile = FindFirstFile(szFullPattern, &FindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// found a subdirectory; recurse into it
				PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
				if (FindFileData.cFileName[0] == '.')
					continue;
				FindFilesRecursively(szFullPattern, lpFilePattern);				
			}
		} while (FindNextFile(hFindFile, &FindFileData));
		FindClose(hFindFile);
	}
	// now we are going to look for the matching files
	PathCombine(szFullPattern, lpFolder, lpFilePattern);
	hFindFile = FindFirstFile(szFullPattern, &FindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// found a file; do something with it
				PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);				
				_tprintf_s(_T("%s\n"), szFullPattern);
			}
		} while (FindNextFile(hFindFile, &FindFileData));
		FindClose(hFindFile);
	}
}

void FileSearcher::DisplayErrorBox(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
