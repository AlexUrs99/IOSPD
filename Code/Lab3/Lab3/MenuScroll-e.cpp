//--------------------------------------------------------------------
// TUCN, Computer Science Department
// Input/Output Systems and Peripheral Devices
//--------------------------------------------------------------------
// http://users.utcluj.ro/~baruch/en/pages/teaching/inputoutput-systems/laboratory.php
//--------------------------------------------------------------------
// File:		MenuScroll-e.cpp
// Changed:		24.10.2015
//--------------------------------------------------------------------
// Template file for a Windows application window with vertical scroll bar
//--------------------------------------------------------------------
// - Menus:		File -> Open
//				File -> Close
//				File -> Exit
//				Operations -> IOS Application
//				Operations -> Operation 1 (no parameters)
//				Operations -> Operation 2 (no parameters)
//				Operations -> Operation 3 (no parameters)
//				Help -> About
//--------------------------------------------------------------------
// - Win API functions used:
//   BeginPaint, CloseHandle, CreateFile, CreateFont, CreateWindowEx, 
//   DefWindowProc, DeleteObject, DestroyWindow, DialogBox, 
//   DispatchMessage, EndDialog, EndPaint, GetClientRect, GetDC, 
//   GetDeviceCaps, GetDlgItem, GetFileSize, GetMessage, GetModuleHandle,
//   GetOpenFileName, GetScrollInfo, GetStockObject, GetSysColor, 
//   GetSystemMetrics, GlobalAlloc, GlobalFree, LoadImage, MessageBox,  
//   MulDiv, PostQuitMessage, PostMessage, ReadFile, RegisterClassEx, 
//   ReleaseDC, ScrollWindowEx, SelectObject, SendMessage, SetBkColor, 
//   SetScrollInfo, SetWindowPos, ShowWindow, TranslateMessage, 
//   UpdateWindow, ZeroMemory 
//--------------------------------------------------------------------

#include <windows.h>
#include "resource.h"

#define WIN32_LEAN_AND_MEAN						// optional: no MFC
#define SCROLL_LINES 200

#define NLIN 500								// number of lines in the display window
#define NCOL 240								// number of columns in the display window

// External variables
extern char szBuffer[NLIN][NCOL];				// buffer for the window contents
extern int  cLine;								// number of current line in the display buffer

//--------------------------------------------------------------------
// To be completed: Declarations of external functions
int  AppScroll (HWND hWnd);
//--------------------------------------------------------------------

// Declarations of functions used
LRESULT CALLBACK WndProc (HWND hWnd, UINT uMes, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDlgProc (HWND hWnd, UINT uMes, WPARAM wParam, LPARAM lParam);
int  ExecFileOpen (HWND hWnd);
void InitText (HWND hWnd);
void CharSize (HWND hWnd);
void GetVScroll (HWND hWnd);
void SetVScroll (HWND hWnd, WORD ClientH);
void DisplWindow (HWND hWnd);

// Gglobal variables
static DWORD dwCharH;							// character height
static DWORD dwCharW;							// average character width
static int nVscrollPos = 0;						// current vertical scroll position
PAINTSTRUCT ps;									// structure for displaying the application window
SCROLLINFO si;									// structure with parameters of the scroll bar
HGDIOBJ hfFontMain;								// handler to the main window font
HGDIOBJ hfFontFile;								// handler to the file window font
BOOL bOper0 = FALSE;							// flag for launching opration 0
BOOL bOper1 = FALSE;							// flag for launching opration 1
BOOL bOper2 = FALSE;							// flag for launching opration 2
BOOL bOper3 = FALSE;							// flag for launching opration 3

//--------------------------------------------------------------------
// Function ExecFileOpen
//--------------------------------------------------------------------
// 
// Function:	- Performs the dialog for selecting the file to open
//				- Opens the file
//				- Reads the file into a buffer
//				- Displays the file contents in an editing window
//
// Parameters:	hWnd - Handle to the application window
//
// Returns:		0 - The file has been opened and read successfully
//				1 - The dialog for open has been abandoned
//				2 - File open error
//				3 - Memory allocation error
//				4 - File read error
//
//--------------------------------------------------------------------

int ExecFileOpen (HWND hWnd)
{
	HANDLE hFile;								// handle to file
	OPENFILENAME ofn;							// structure for calling the dialog 
												// function GetOpenFileName
	char szFileName [MAX_PATH+1] = "";			// buffer for the file path and name
	DWORD dwFileSize;							// file size
	LPSTR pszBuffer;							// pointer to buffer for the file
	DWORD dwRead;								// number of bytes read
	HWND hDisplay;								// handle to display window
	HDC hDc;									// handle to device context
	int lfSize;									// height of characters in logical units
	RECT rcClient;								// structure with the coordinates of client window

	ZeroMemory (&ofn, sizeof (ofn));
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = "D:\\Laborator\\SIE\\";
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if (GetOpenFileName (&ofn) == 0)
		return 1;								// the dialog for file selection has been abandoned
	hFile = CreateFile (szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
						OPEN_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox (hWnd, "File open error",
					"IOS Application", MB_OK | MB_ICONERROR);
		return 2;								// file open error
	}
	dwFileSize = GetFileSize (hFile, NULL);
	pszBuffer = (LPSTR) GlobalAlloc (GPTR, dwFileSize + 1);
												// allocate memory for the buffer
	if (pszBuffer == NULL)
		return 3;								// memory allocation error
	if (!ReadFile (hFile, pszBuffer, dwFileSize, &dwRead, NULL)) {
		MessageBox (hWnd, "File read error",
					"IOS Application", MB_OK | MB_ICONERROR);
		return 4;								// file read error
	}
	pszBuffer [dwFileSize] = 0;					// append a byte of 0 to the end
	GetClientRect (hWnd, &rcClient);			// determine the coordinates of client window
	hDisplay = CreateWindowEx (					// create the display window
		WS_EX_CLIENTEDGE,
		"EDIT",									// name of system class Edit
		"TextFile",								// name of window
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
		ES_MULTILINE | ES_AUTOVSCROLL,
		0,										// horizontal position of window
		0,										// vertical position of window
		rcClient.right,							// window width
		rcClient.bottom,						// window height
		hWnd,									// handle to parent window
		(HMENU) IDC_DISPLAY,					// identifier of display window
		GetModuleHandle (NULL),					// handle to module instance
		NULL);
	if (hDisplay == NULL)
		MessageBox (hWnd, "Error creating the display window",
					"Error", MB_OK | MB_ICONERROR);
	hDc = GetDC (hDisplay);						// determine the device context
	lfSize = -MulDiv (9, GetDeviceCaps (hDc, LOGPIXELSY), 72);
												// compute the size in logical units
	ReleaseDC (hDisplay, hDc);
	hfFontFile = CreateFont (lfSize, 0, 0, 0, 0,
						 0, 0, 0, 0, 0,
						 0, CLEARTYPE_QUALITY, 0,
						 "Tahoma");				// create a logical font
	if (hfFontFile == NULL)
		hfFontFile = GetStockObject (DEFAULT_GUI_FONT);
	SendMessage (hDisplay, WM_SETFONT,			// set the font for the file window
		(WPARAM) hfFontFile, (LPARAM) FALSE);
	SendMessage (hDisplay, WM_SETTEXT,			// display the text
		NULL, (LPARAM) pszBuffer);
	GlobalFree (pszBuffer);						// release the memory
	CloseHandle (hFile);						// close the file
	return 0;
}

//--------------------------------------------------------------------
// Function InitText
//--------------------------------------------------------------------
//
// Function:	- Determines the context of the display device
//				- Sets the type of characters to the default type
//				  used for menus and dialog boxes (Tahoma)
//				- Sets the size of characters to 9 points
//				- Sets the background color to COLOR_BTNFACE
//
// Parameters:	hWnd - Handle to the application window
//
// Returns:		-
//
//--------------------------------------------------------------------

void InitText (HWND hWnd)
{
	HDC hDc;									// handle to device context
	int lfSize;									// height of characters in logical units
	DWORD dwSysCol;								// RGB value of the system color

	hDc = GetDC (hWnd);							// determine the device context
	lfSize = -MulDiv (9, GetDeviceCaps (hDc, LOGPIXELSY), 72);
												// compute the size in logical units
	hfFontMain = CreateFont (lfSize, 0, 0, 0, 0,
						 0, 0, 0, 0, 0,
						 0, CLEARTYPE_QUALITY, 0,
						 "Tahoma");				// create a logical font
	if (hfFontMain == NULL)
		hfFontMain = GetStockObject (DEFAULT_GUI_FONT);
	SelectObject (hDc, hfFontMain);				// set the new font for the main window
	dwSysCol = GetSysColor (COLOR_BTNFACE);		// determine the RGB value of the system color
	SetBkColor (hDc, dwSysCol);					// set the background color
	ReleaseDC (hWnd, hDc);
	CharSize (hWnd);							// determine the height and average width of a character
}

//--------------------------------------------------------------------
// Function WinMain
//--------------------------------------------------------------------

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX Wnd;								// structure for the window class
	HWND hWnd;									// handle to window
	MSG Msg;									// structure for the messages
	HICON hIcon;
	HICON hIconSm;
	HCURSOR hCursor;
	const char szClassName [] = "SieWindowClass";

// Initialize the icons and window cursor
	hIcon = (HICON) LoadImage (NULL, "sie.ico", IMAGE_ICON, 
							   32, 32, LR_LOADFROMFILE);
	hIconSm = (HICON) LoadImage (NULL, "sie.ico", IMAGE_ICON, 
							   16, 16, LR_LOADFROMFILE);
	hCursor = (HCURSOR) LoadImage (NULL, IDC_ARROW, IMAGE_CURSOR, 
								   0, 0, LR_SHARED);

// Initialize the WNDCLASSEX structure
	Wnd.cbSize			= sizeof (WNDCLASSEX);	// size of the structure in bytes
	Wnd.style			= CS_HREDRAW | CS_VREDRAW |
						  CS_OWNDC;				// class style: unique device context
	Wnd.lpfnWndProc		= (WNDPROC) WndProc;	// pointer to window function
	Wnd.cbClsExtra		= 0;
	Wnd.cbWndExtra		= 0;
	Wnd.hInstance		= hInstance;			// handle to application instance
	Wnd.hIcon			= hIcon;
	Wnd.hCursor			= hCursor;
	Wnd.hbrBackground	= (HBRUSH) (COLOR_BTNFACE+1);
	Wnd.lpszMenuName	= MAKEINTRESOURCE (IDR_SIEMENU1);
												// pointer to menu name
	Wnd.lpszClassName	= szClassName;			// pointer to class name
	Wnd.hIconSm			= hIconSm;

// Register the window class
	if (!RegisterClassEx (&Wnd)) {
		MessageBox (NULL, "Class registration error",
					"Error!", MB_OK | MB_ICONERROR);
		return 0;
	}

// Create the application window
	hWnd = CreateWindowEx (
		WS_EX_CLIENTEDGE,						// extended style
		szClassName,							// pointer to class name
		"IOS Application",						// window title
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,		// window style
		GetSystemMetrics (SM_CXSCREEN) / 4,		// horizontal position
		GetSystemMetrics (SM_CYSCREEN) / 4,		// vertical position
		GetSystemMetrics (SM_CXSCREEN) / 2,		// window width
		GetSystemMetrics (SM_CYSCREEN) / 2,		// window height
		NULL,									// handle to parent window
		NULL,									// handle to window menu
		hInstance,								// handle to application instance
		NULL);									// pointer to a value sent to the window
	if (hWnd == NULL) {
		MessageBox (NULL, "Error creating the application window",
					"Error!", MB_OK | MB_ICONERROR);
		return 0;
	}

// Display the window
	ShowWindow (hWnd, nCmdShow); 
	UpdateWindow (hWnd);

// Message loop
	while (GetMessage (&Msg, NULL, 0, 0) > 0) {
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
// Starting operation 0
		if (bOper0 == TRUE) {
			bOper0 = FALSE;
			AppScroll (hWnd);
		}
// Starting operation 1 
		if (bOper1 == TRUE) {
			bOper1 = FALSE;
//--------------------------------------------------------------------
// To be completed: Call to function for Operation 1
//--------------------------------------------------------------------
		}
// Starting operation 2 
		if (bOper2 == TRUE) {
			bOper2 = FALSE;
//--------------------------------------------------------------------
// To be completed: Call to function for Operation 2
//--------------------------------------------------------------------
		}
// Starting operation 3 
		if (bOper3 == TRUE) {
			bOper3 = FALSE;
//--------------------------------------------------------------------
// To be completed: Call to function for Operation 3
//--------------------------------------------------------------------
		}
	}
	return (int)Msg.wParam;
}

//--------------------------------------------------------------------
// About dialog function
//--------------------------------------------------------------------

INT_PTR CALLBACK AboutDlgProc (HWND hWnd, UINT uMes,
							   WPARAM wParam, LPARAM lParam)
{
	switch (uMes) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD (wParam)) {
		case IDOK:
			EndDialog (hWnd, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog (hWnd, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------------
// Window function
//--------------------------------------------------------------------

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMes, WPARAM wParam, LPARAM lParam)
{
	HWND hDisplay;
	RECT rcClient;
	int  nDelta;

	switch (uMes) {
	case WM_COMMAND:
		switch (LOWORD (wParam)) {
		case ID_FILE_EXIT:
			PostMessage (hWnd, WM_CLOSE, 0, 0);
			break;
		case ID_FILE_OPEN:
			ExecFileOpen (hWnd);
			break;
		case ID_FILE_CLOSE:
			hDisplay = GetDlgItem (hWnd, IDC_DISPLAY);
			DestroyWindow (hDisplay);
			DeleteObject (hfFontFile);
//--------------------------------------------------------------------
//			Restoring the window contents when closing the file
			DisplWindow (hWnd);
//--------------------------------------------------------------------
			break;
		case ID_HELP_ABOUT:
			DialogBox (GetModuleHandle (NULL),
				MAKEINTRESOURCE (IDD_ABOUT), hWnd, AboutDlgProc);
			break;
		case ID_OPER_OP0:
//--------------------------------------------------------------------
//			Setting the flag for starting Operation 0
			bOper0 = TRUE;
//--------------------------------------------------------------------
			break;
		case ID_OPER_OP1:
//--------------------------------------------------------------------
//			To be modified: Setting the flag for starting Operation 1
//			bOper1 = TRUE;
			MessageBox (hWnd, "No operation to be performed!",
						"IOS Application", MB_OK | MB_ICONINFORMATION);
//--------------------------------------------------------------------
			break;
		case ID_OPER_OP2:
//			To be modified: Setting the flag for starting Operation 2
//			bOper2 = TRUE;
			MessageBox (hWnd, "No operation to be performed!",
						"IOS Application", MB_OK | MB_ICONINFORMATION);
//--------------------------------------------------------------------
			break;
		case ID_OPER_OP3:
//			To be modified: Setting the flag for starting Operation 3
//			bOper3 = TRUE;
			MessageBox (hWnd, "No operation to be performed!",
						"IOS Application", MB_OK | MB_ICONINFORMATION);
//--------------------------------------------------------------------
			break;
		}
		break;
	case WM_CREATE:
		CharSize (hWnd);						// determine the height and average width of a character
		GetClientRect (hWnd, &rcClient);		// determine the coordinates of client area
		break;
	case WM_SIZE:
		GetClientRect (hWnd, &rcClient);
		hDisplay = GetDlgItem (hWnd, IDC_DISPLAY);
		SetWindowPos (hDisplay, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
		SetVScroll (hWnd, HIWORD (lParam));
		break;
	case WM_VSCROLL:
		GetVScroll (hWnd);						// determine and save the position of the scroll box
		switch (LOWORD (wParam)) {
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}
		si.fMask = SIF_POS;						// set the position of the scroll box
		SetScrollInfo (hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo (hWnd, SB_VERT, &si);		// retrieve the position of the scroll box
		if (si.nPos != nVscrollPos) {			// if the position has changed, scroll the window
			ScrollWindowEx (hWnd, 0, (nVscrollPos - si.nPos) * dwCharH, NULL, NULL, NULL, NULL, SW_INVALIDATE);
			UpdateWindow (hWnd);				// update the window
		}
		break;
	case WM_MOUSEWHEEL:
		nDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (nDelta > 0)							// forward rotation => scroll up
			SendMessage (hWnd, WM_VSCROLL, SB_LINEUP, 0);
		else									// backward rotation => scroll down
			SendMessage (hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
		break;
	case WM_PAINT:
		BeginPaint (hWnd, &ps);
		GetVScroll (hWnd);						// determine and save the position of the scroll box
//--------------------------------------------------------------------
//		Restoring the window contents
		InitText (hWnd);
		DisplWindow (hWnd);
//--------------------------------------------------------------------
		EndPaint (hWnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow (hWnd);
		break;
	case WM_DESTROY:
		DeleteObject (hfFontMain);
		DeleteObject (hfFontFile);
		PostQuitMessage (0);
		break;
	default:
		return DefWindowProc (hWnd, uMes, wParam, lParam);
	}
	return 0;
}

//--------------------------------------------------------------------
// Function CharSize
//--------------------------------------------------------------------
//
// Function:	Determine the height and average width of a character
//
// Parameters:	- Handle to the application window
//
// Returns:		-
//
//--------------------------------------------------------------------

void CharSize(HWND hWnd)
{
	HDC hDc;									// handle to the device context
	TEXTMETRIC tm;								// structure with the display metrics

	hDc = GetDC(hWnd);							// determine the device context
	GetTextMetrics(hDc, &tm);					// determine the display metrics
	dwCharH = tm.tmHeight + tm.tmExternalLeading;
	dwCharW = tm.tmAveCharWidth;
	ReleaseDC(hWnd, hDc);
}

//--------------------------------------------------------------------
// Function GetVScroll
//--------------------------------------------------------------------
//
// Function:	Determine and save the position of the vertical scroll box
//
// Parameters:	- Handle to the application window
//
// Returns:		-
//
//--------------------------------------------------------------------

void GetVScroll(HWND hWnd) {

	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(hWnd, SB_VERT, &si);		// determine the information about the vertical scroll bar
	nVscrollPos = si.nPos;					// save the position of the scroll box
}

//--------------------------------------------------------------------
// Function SetVScroll
//--------------------------------------------------------------------
//
// Function:	Set the vertical scrolling range and page size
//
// Parameters:	- Handle to the application window
//				- Height of the client area
//
// Returns:		-
//
//--------------------------------------------------------------------

void SetVScroll(HWND hWnd, WORD ClientH)
{
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE;			// the scrolling range and page size are set
	si.nMin   = 0;								// minimum scrolling position
	si.nMax   = SCROLL_LINES;					// maximum scrolling position
	si.nPage  = ClientH / dwCharH;				// page size
	SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}

//--------------------------------------------------------------------
// Function DisplWindow
//--------------------------------------------------------------------
//
// Function:	Displays the contents of the application window
//
// Parameters:	- Handle to the application window
//
// Returns:		-
//
//--------------------------------------------------------------------

void DisplWindow(HWND hWnd)
{
	HDC hDc;									// handle to the device context
	int nStartLine;								// first line in the invalidated area
	int nEndLine;								// last line in the invalidated area
	int i;

	hDc = GetDC(hWnd);							// determine the device context
	nStartLine = max(0, nVscrollPos + ps.rcPaint.top / dwCharH);
	nEndLine = min(SCROLL_LINES, nVscrollPos + ps.rcPaint.bottom / dwCharH);
	for (i = nStartLine; i <= nEndLine; i++) {
		TextOut(hDc, 10, (i - nVscrollPos) * dwCharH, szBuffer[i], NCOL);
	}
	ReleaseDC(hWnd, hDc);
}
