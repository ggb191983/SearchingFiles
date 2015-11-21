#include "stdafx.h"
#include "FileSearcher.h"
#include <strsafe.h>
#include <string>
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


TCHAR * FileSearcher::Search2(const TCHAR * strFile, const TCHAR * strFilePath, const bool& bRecursive, const bool& bStopWhenFound) const
{
    TCHAR strFoundFilePath[255];
    WIN32_FIND_DATA file;

    TCHAR strPathToSearch[255];
	_tcscpy(strPathToSearch, strFilePath);
    if (strPathToSearch != _T('\0'))
        strPathToSearch = IncludeTrailingPathDelimiter(strPathToSearch);

    HANDLE hFile = FindFirstFile((strPathToSearch + "*").c_str(), &file); 
    if (hFile != INVALID_HANDLE_VALUE) 
    {
        std::auto_ptr<TStringList> subDirs;

        do
        {
            String strTheNameOfTheFile = file.cFileName;

            // It could be a directory we are looking at
            // if so look into that dir
            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ((strTheNameOfTheFile != ".") && (strTheNameOfTheFile != "..") && (bRecursive))
                {
                    if (subDirs.get() == NULL)
                        subDirs.reset(new TStringList);

                    subDirs->Add(strPathToSearch + strTheNameOfTheFile);
                }
            }
            else
            {
                if (strTheNameOfTheFile == strFile)
                {
                    strFoundFilePath = strPathToSearch + strFile;

                    /// TODO
                    // ADD TO COLLECTION TYPE

                    if (bStopWhenFound)
                        break;
                }
            }
        }
        while (FindNextFile(hFile, &file));

        FindClose(hFile);

        if (!strFoundFilePath.IsEmpty() && bStopWhenFound)
            return strFoundFilePath;

        if (subDirs.get() != NULL)
        {
            for (int i = 0; i < subDirs->Count; ++i)
            {
                strFoundFilePath = Search2(strFile, subDirs->Strings[i], bRecursive, bStopWhenFound);

                if (!strFoundFilePath.IsEmpty() && bStopWhenFound)
                    break;
            }
        }
    }

    return strFoundFilePath;
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
