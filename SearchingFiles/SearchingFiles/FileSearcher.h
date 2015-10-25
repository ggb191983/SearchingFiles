#define MAXREAD 8192
#define DIRATTR (DDL_READWRITE | DDL_READONLY | DDL_HIDDEN | DDL_SYSTEM | \
 DDL_DIRECTORY | DDL_ARCHIVE | DDL_DRIVES)
#define DTFLAGS (DT_WORDBREAK | DT_EXPANDTABS | DT_NOCLIP | DT_NOPREFIX)


#pragma once
class FileSearcher
{	
public:	
	void DisplayErrorBox(LPTSTR lpszFunction);
	void Search(TCHAR fileName);
	~FileSearcher(void);

	void ChangeDirectory(TCHAR *dir);
	static FileSearcher *instance()
    {
        if (!s_instance)
          s_instance = new FileSearcher;
        return s_instance;
    }
private:
	static FileSearcher *s_instance;	
	FileSearcher();
	FileSearcher(TCHAR dir);
	TCHAR * dir;	
};
