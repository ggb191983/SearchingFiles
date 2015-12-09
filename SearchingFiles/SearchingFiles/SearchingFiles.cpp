// SearchingFiles.cpp: define el punto de entrada de la aplicación.
//

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "stdafx.h"
#include "SearchingFiles.h"
#include "FileSearcher.h"
#include "FolderBrowserDialog.h"
#include "resource.h"
#include <list>
#include <string>
#include <cctype>

using namespace std;

// global array of houses
FILEINFO rgFileInfo[] =
{
	{ _T("100 Main Street"), _T("Redmond"), 175000, 3, 2 },
	{ _T("523 Pine Lake Road"), _T("Redmond"), 125000, 4, 2 },
	{ _T("1212 112th Place SE"), _T("Redmond"),200000, 4, 3 },
	{ _T("22 Lake Washington Blvd"), _T("Bellevue"), 2500000, 4, 4 },
	{ _T("33542 116th Ave. NE"), _T("Bellevue"), 180000, 3, 2 },
	{ _T("64134 Nicholas Lane"), _T("Bellevue"), 250000, 4, 3 },
	{ _T("33 Queen Anne Hill"), _T("Seattle"), 350000, 3, 2 },
	{ _T("555 SE Fifth St"), _T("Seattle"), 140000, 3, 2 },
	{ _T("446 Mariners Way"), _T("Seattle"), 225000, 4, 3 }
};

FolderBrowserDialog *folderBrowserDialog1 = new FolderBrowserDialog();

void CustomPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	GetClientRect(hwnd, &rect);
	hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
}


LRESULT CALLBACK CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_PAINT:
		CustomPaint(hwnd);
		return 0;

	case WM_COMMAND:
		SendMessage(hMainWindow, uMsg, wParam, lParam);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CustomRegister(void)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = CustomProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = CUSTOM_FRAME;
	RegisterClass(&wc);
}

void CustomUnregister(void)
{
	UnregisterClass(CUSTOM_FRAME, NULL);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_CLASS_NAME, szWindowClass, MAX_LOADSTRING);

	CustomRegister();

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SEARCHINGFILES));

	//FileSearcher::instance()->FindFilesRecursively(_T("C:\\WINDOWS"), _T("*.wav"));

	//while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	//{
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//}

	// TODO: FinalizeConfig() is called within the WM_DESTROY to flush settings to the memory
	// to save UI settings before everything is kill.

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SEARCHINGFILES));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	hMainWindow = hWnd;
	//hMainFrame = CreateMainFrame(hWnd);	

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

HWND CreateMainFrame(HWND hwndParent)
{
	RECT rcClient;                       // The parent window's client area.

	GetClientRect(hwndParent, &rcClient);

	HWND hGroupBox = CreateWindow(CUSTOM_FRAME, _T(""),
		WS_CHILD | WS_VISIBLE,
		(rcClient.right - rcClient.left) / 2,
		0,
		(rcClient.right - rcClient.left) / 2,
		(rcClient.bottom - rcClient.top) / 2,
		hwndParent, NULL, GetModuleHandle(NULL), NULL);

	return hGroupBox;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	list<WIN32_FIND_DATA> files;
	static HCURSOR 	hCursorWE;
	static HCURSOR 	hCursorNS;
	static BOOL	bSplitterMoving;
	static bool bHSplitter;
	static HWND hEdit;
	static HGDIOBJ hfDefault;
	static HWND hWndButton;
	static HWND hWndListView;
	TCHAR szBuffer[MAX_PATH + 1];
	FileSearcher *fileSearcher = FileSearcher::instance();
	int iItem = 0;
	int iSubItemCol = 1;

	static FILEINFO rgFILE[100];


	switch (message)
	{

	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
			_T("EDIT"),
			_T(""),
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			0,
			0,
			150,
			20,
			hWnd,
			(HMENU)IDC_SEARCH_TEXT,
			GetModuleHandle(NULL),
			NULL);
		hfDefault = GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)_T("Fichero a buscar..."));
		hWndButton = CreateWindowEx(NULL,
			_T("BUTTON"),
			_T("Search"),
			WS_TABSTOP | WS_VISIBLE |
			WS_CHILD | BS_DEFPUSHBUTTON,
			155,
			0,
			50,
			20,
			hWnd,
			(HMENU)IDC_SEARCH_BUTTON,
			GetModuleHandle(NULL),
			NULL);
		SendMessage(hWndButton, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

		hWndListView = CreateListView(hWnd);
		if (hWndListView == NULL)
			MessageBox(NULL, _T("Listview not created!"), NULL, MB_OK);

		break;

	case WM_NOTIFY:
		return (NotifyHandler(hWnd, message, wParam, lParam));
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDC_SEARCH_BUTTON:
				SendMessage(hEdit, WM_GETTEXT, sizeof(szBuffer) / sizeof(szBuffer[0]), reinterpret_cast<LPARAM>(szBuffer));
				//MessageBox(NULL, szBuffer, TEXT("Information"),	MB_ICONINFORMATION);
				if (folderBrowserDialog1->ShowDialog())
				{
					fileSearcher->ChangeDirectory(folderBrowserDialog1->SelectedPath);
				}			
				ListView_DeleteAllItems(hWndListView);
				files = fileSearcher->FindFilesRecursively(fileSearcher->dir, szBuffer);
				
				LV_ITEM lvI;
				memset(&lvI, 0, sizeof(lvI));
				lvI.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;   // Text Style
				lvI.cchTextMax = 256; // Max size of test
				lvI.state = 0;      //
				lvI.stateMask = 0;  //			
				LARGE_INTEGER filesize;					
								
				for each (const WIN32_FIND_DATA f in files)
				{	
					filesize.HighPart = f.nFileSizeHigh;
					filesize.LowPart = f.nFileSizeLow;
					rgFILE[iItem].dwFileSize = (DWORD)filesize.QuadPart;
					_tcscpy_s(rgFILE[iItem].cFileName, f.cFileName);		
					lvI.iItem = iItem;
					lvI.iSubItem = 0;					
					// The parent window is responsible for storing the text. The List view
					// window will send a LVN_GETDISPINFO when it needs the text to display/
					lvI.pszText = LPSTR_TEXTCALLBACK;
					lvI.cchTextMax = MAX_ITEMLEN;			
					lvI.lParam = (LPARAM)&rgFILE[iItem];

					if (ListView_InsertItem(hWndListView, &lvI) == -1)
						return NULL;

					for (int iSubItem = 1; iSubItem < NUM_COLUMNS; iSubItem++)
					{
						ListView_SetItemText(hWndListView, iItem++, iSubItem, LPSTR_TEXTCALLBACK);
					}
				}
			
				break;
			case IDC_SEARCH_TEXT:
				//SendMessage(hEdit, WM_SETTEXT, sizeof(szBuffer)/sizeof(szBuffer[0]),
				//			reinterpret_cast<LPARAM>(szBuffer));
				break;
		case IDM_ABOUT:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &rect);

		SetWindowPos(hWndListView, NULL, 0, 40, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER); 
		break;

	case WM_MOUSEMOVE:

		/*if (IsMouseOver(hDetailsTab, LOWORD(lParam), HIWORD(lParam)) && !bSplitterMoving)
		return 0;

		if (LOWORD(lParam) > 10)
		{
		SetCursor(hCursorWE);
		if ((wParam == MK_LBUTTON) && bSplitterMoving)
		{
		GetClientRect(hWnd, &rect);
		if (LOWORD(lParam) > rect.right)
		return 0;

		Config.UI.VSplitter = LOWORD(lParam);
		SendMessage(hWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
		}
		}
		//else if (HIWORD(lParam) > 10)
		//{
		//	SetCursor(hCursorNS);
		//	if ((wParam == MK_LBUTTON) && bSplitterMoving)
		//	{
		//		GetClientRect(hWnd, &rect);
		//		if (HIWORD(lParam) > rect.bottom)
		//			return 0;

		//		dwSplitterH = HIWORD(lParam);
		//		SendMessage(hWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
		//	}
		//}
		*/
		break;


	case WM_LBUTTONDOWN:
		/*if (!IsMouseOver(hDetailsTab, LOWORD(lParam), HIWORD(lParam)))
		{
		SetCursor(hCursorWE);
		bSplitterMoving = TRUE;
		SetCapture(hWnd);
		}*/
		break;

	case WM_LBUTTONUP:
		//ReleaseCapture();
		//bSplitterMoving = FALSE;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		//bRunning = FALSE;
		//FinalizeConfig();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/****************************************************************************
*
*    FUNCTION: CreateListView(HWND)
*
*    PURPOSE:  Creates the list view window and initializes it
*
****************************************************************************/
HWND CreateListView(HWND hWndParent)
{
	HWND hWndList;      // Handle to the list view window
	RECT rcl;           // Rectangle for setting the size of the window
	HICON hIcon;        // Handle to an icon
	int index;          // Index used in for loops
	HIMAGELIST hSmall, hLarge; // Handles to image lists for large and small icons
	LV_COLUMN lvC;      // List View Column structure
	TCHAR szText[MAX_PATH];    // Place to store some text
	LV_ITEM lvI;        // List view item structure
	int iSubItem;       // Index for inserting sub items

						// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Get the size and position of the parent window
	GetClientRect(hWndParent, &rcl);

	// Create the list view window that starts out in report view
	// and allows label editing.
	hWndList = CreateWindowEx(0L,
		WC_LISTVIEW,                // list view class
		_T(""),                         // no default text
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT |
		LVS_EDITLABELS | WS_EX_CLIENTEDGE,
		0, 40,
		rcl.right - rcl.left,
		rcl.bottom - rcl.top,
		hWndParent,
		(HMENU)ID_LISTVIEW,
		GetModuleHandle(NULL),
		NULL);

	if (hWndList == NULL)
		return NULL;

	// initialize the list view window
	// First, initialize the image lists we will need
	// create an image list for the small and large icons
	// FALSE specifies large icons - TRUE specifies small

	hSmall = ImageList_Create(BITMAP_WIDTH, BITMAP_HEIGHT, FALSE, 3, 0);
	hLarge = ImageList_Create(LG_BITMAP_WIDTH, LG_BITMAP_HEIGHT, FALSE, 3, 0);

	// load the icons and add them to the image lists
	for (index = IDI_ICON2; index <= IDI_ICON4; index++)
	{
		hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(index));
		// there are 3 of each type of icon here, so add 3 at a time.
		for (iSubItem = 0; iSubItem < 3; iSubItem++)
		{
			if ((ImageList_AddIcon(hSmall, hIcon) == -1) ||
				(ImageList_AddIcon(hLarge, hIcon) == -1))
				return NULL;
		}
	}

	// Make sure that all of the icons were added
	if (ImageList_GetImageCount(hSmall) < 3)
		return FALSE;

	// Make sure that all of the icons were added
	if (ImageList_GetImageCount(hLarge) < 3)
		return FALSE;

	// Associate the image list with the list view
	ListView_SetImageList(hWndList, hSmall, LVSIL_SMALL);
	ListView_SetImageList(hWndList, hLarge, LVSIL_NORMAL);

	// Now initialize the columns we will need
	// Initialize the LV_COLUMN structure
	// the mask specifies that the .fmt, .ex, width, and .subitem members 
	// of the structure are valid,
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left align the column
	lvC.cx = 75;            // width of the column, in pixels
	lvC.pszText = szText;

	// Add the columns.
	for (index = 0; index <= NUM_COLUMNS; index++)
	{
		lvC.iSubItem = index;
		LoadString(GetModuleHandle(NULL), IDS_FILENAME + index, szText,	sizeof(szText));
		if (ListView_InsertColumn(hWndList, index, &lvC) == -1)
			return NULL;
	}

	// Finally, let's add the actual items to the control
	// Fill in the LV_ITEM structure for each of the items to add
	// to the list.
	// The mask specifies the the .pszText, .iImage, .lParam and .state
	// members of the LV_ITEM structure are valid.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvI.state = 0;      //
	lvI.stateMask = 0;  //

	for (index = 0; index < NUM_ITEMS; index++)
	{
		lvI.iItem = index;
		lvI.iSubItem = 0;
		// The parent window is responsible for storing the text. The List view
		// window will send a LVN_GETDISPINFO when it needs the text to display/
		lvI.pszText = LPSTR_TEXTCALLBACK;
		lvI.cchTextMax = MAX_ITEMLEN;
		lvI.iImage = index;
		lvI.lParam = (LPARAM)&rgFileInfo[index];

		if (ListView_InsertItem(hWndList, &lvI) == -1)
			return NULL;

		for (iSubItem = 1; iSubItem < NUM_COLUMNS; iSubItem++)
		{
			ListView_SetItemText(hWndList, index, iSubItem,	LPSTR_TEXTCALLBACK);
		}
	}
	return (hWndList);
}

LRESULT NotifyHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam;
	NM_LISTVIEW *pNm = (NM_LISTVIEW *)lParam;
	FILEINFO *pFileInfo = (FILEINFO *)(pLvdi->item.lParam);
	static TCHAR szText[10];

	if (wParam != ID_LISTVIEW)
		return 0L;

	switch (pLvdi->hdr.code)
	{
	case LVN_GETDISPINFO:

		switch (pLvdi->item.iSubItem)
		{
		case 0:     // Nombre
			pLvdi->item.pszText = pFileInfo->cFileName;
			break;

		case 1:     // Tamaño		
			wsprintf(szText, _T("%ld"), pFileInfo->dwFileSize);
			pLvdi->item.pszText = szText;
			break;	

		default:
			break;
		}
		break;

	case LVN_BEGINLABELEDIT:
	{
		HWND hWndEdit;

		// Get the handle to the edit box.
		hWndEdit = (HWND)SendMessage(hWnd, LVM_GETEDITCONTROL, 0, 0);
		// Limit the amount of text that can be entered.
		SendMessage(hWndEdit, EM_SETLIMITTEXT, (WPARAM)20, 0);
	}
	break;

	case LVN_ENDLABELEDIT:
		// Save the new label information
		if ((pLvdi->item.iItem != -1) && (pLvdi->item.pszText != NULL))
			lstrcpy(pFileInfo->cFileName, pLvdi->item.pszText);
		break;

	case LVN_COLUMNCLICK:
		// The user clicked on one of the column headings - sort by
		// this column.
		ListView_SortItems(pNm->hdr.hwndFrom, ListViewCompareProc, (LPARAM)(pNm->iSubItem));
		break;

	default:
		break;
	}
	return 0L;
}

int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	FILEINFO *pFileInfo1 = (FILEINFO *)lParam1;
	FILEINFO *pFileInfo2 = (FILEINFO *)lParam2;
	int iResult;


	if (pFileInfo1 && pFileInfo2)
	{
		switch (lParamSort)
		{
			case 0:     // Ordenar por nombre		
				iResult = _tcscmp(pFileInfo1->cFileName, pFileInfo2->cFileName);
				//iResult = lstrcmpi(lpStr1, lpStr2);
				break;

			case 1:     // Ordenar por tamaño		
				iResult = pFileInfo1->dwFileSize - pFileInfo2->dwFileSize;
				break;

			default:
				iResult = 0;
				break;
		}

	}
	return iResult;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}