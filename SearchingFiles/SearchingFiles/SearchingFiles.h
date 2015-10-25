#pragma once

#include "resource.h"


#define MAX_LOADSTRING 100
#define C_COLUMNS 5

#define CUSTOM_FRAME   _T("CustomFrame")

// Variables globales:
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HWND hStatusTree;								// Sidebar tree
HWND hDetailsTab;								// Details tab pane
HWND hToolBar;
HWND hMainFrame;
HWND hMainWindow;

// Function prototypes
HWND				CreateListView(HWND hWndParent);
BOOL				InitListViewColumns(HWND hWndListView);
HWND				CreateMainFrame(HWND hwndParent);
HWND				CreateAListView(HWND hwndParent);
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#define IDC_SEARCH_TEXT 1000
#define IDC_SEARCH_BUTTON 1001
#define IDM_LIST_VIEW 1002
#define IDS_FIRSTCOLUMN 1003
#define	ID_LISTVIEW 1004


#define NUM_ITEMS       9
#define NUM_COLUMNS     5
#define MAX_ITEMLEN     64
#define MAX_ADDRESS     64
#define MAX_CITY        16
#define MAX_PRICE       12
#define BITMAP_WIDTH    16
#define BITMAP_HEIGHT   16
#define LG_BITMAP_WIDTH	32
#define LG_BITMAP_HEIGHT 32

// Options menu

#define IDM_LARGEICON   100
#define IDM_SMALLICON   101
#define IDM_LISTVIEW    102
#define IDM_REPORTVIEW  103

// icons
#define APP_ICON        400
#define REDMOND         401
#define BELLEVUE        402
#define SEATTLE         403

typedef struct tagHOUSEINFO
{
	char szAddress[MAX_ADDRESS];
	char szCity[MAX_CITY];
	int iPrice;
	int iBeds;
	int iBaths;
} HOUSEINFO;
