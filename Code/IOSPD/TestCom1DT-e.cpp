//--------------------------------------------------------------------
// TUCN, Computer Science Department
// Input/Output Systems and Peripheral Devices
//--------------------------------------------------------------------
// http://users.utcluj.ro/~baruch/en/pages/teaching/inputoutput-systems/laboratory.php
//--------------------------------------------------------------------
// File:		TestCom1DT-e.cpp
// Changed: 	21.02.2021
//--------------------------------------------------------------------
// Testing the COM1 serial port
//--------------------------------------------------------------------
// - Application example to illustrate port accesses
// - The MarvinHW driver is used for port accesses
// - The DrawText function is used for displaying the messages
//--------------------------------------------------------------------
// - Win API functions used:
//   CreateFile, DrawText, GetClientRect, GetDC, GetLastError, 
//   MessageBox, ReleaseDC
//--------------------------------------------------------------------

#include <windows.h>
#include "Hw.h"
#include "ComDef-e.h"

// Global variables
char szBuffer[4096+1];							// buffer for the window contents

// Declarations of functions used
void DisplWindow(HWND hWnd);

//--------------------------------------------------------------------
// Function TestCom1DT
//--------------------------------------------------------------------
//
// Function:	Tests the existence of the COM1 serial port
//
// Parameters:	hWnd - Handle to the application window
//
// Returns:		0 - Operation completed successfully
//				1 - Error initializing the Hw driver
//				2 - Error opening the COM1 port
//
//--------------------------------------------------------------------

int TestCom1DT(HWND hWnd)
{
	DWORD dwLastError;							// code of last error
	HANDLE hFile;
	BYTE  byVal;								// value read from port
	BOOL  bSucc;
	int   i;

	char szMes0[] = "Error initializing the Hw driver\nCode: 0x%08X";
	char szMes1[] = "Testing the COM1 serial port using the MarvinHW driver\r\n\r\n";
	char szMes2[] = "Serial port with base address 0x%03X\r\n\r\n";
	char szMes3[] = "Value written to LCR register: 0xAA\r\n";
	char szMes4[] = "Value written to LCR register: 0x55\r\n";
	char szMes5[] = "Value read from LCR register: 0x%02X\r\n";
	char szMes6[] = "The port exists and works correctly\r\n";
	char szMes7[] = "The port does not exist or does not work correctly\r\n";
	char szMes8[] = "Error opening the COM1 port\nCode: 0x%08X";

	// Initialize the Hw library
	if (!HwOpen()) {
		dwLastError = GetLastError();
		wsprintf(szBuffer, szMes0, dwLastError);
		MessageBox(NULL, szBuffer, "HwOpen", MB_ICONSTOP);
		return 1;
	}

	// Open the COM1 port
	hFile = CreateFile("COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		dwLastError = GetLastError();
		wsprintf(szBuffer, szMes8, dwLastError);
		MessageBox(NULL, szBuffer, "CreateFile", MB_ICONSTOP);
		return 2;
	}

	// Erase the buffer for the window contents
	memset(szBuffer, ' ', 4096);
	szBuffer[4096] = 0;

	// Copy the start message
	i = wsprintf(szBuffer, szMes1);

	// Test the COM1 serial port and display the messages
	i += wsprintf(&szBuffer[i], szMes2, COM1);
	__outp(COM1_LCR, 0xAA);						// write the value 0xAA to the LCR register
	byVal = __inp(COM1_LCR);					// read the value written
	i += wsprintf(&szBuffer[i], szMes3);
	i += wsprintf(&szBuffer[i], szMes5, byVal);	// prepare the message for the value read
	if (byVal == 0xAA)
		bSucc = TRUE;
	else
		bSucc = FALSE;
	__outp(COM1_LCR, 0x55);						// write the value 0x55 to the LCR register
	byVal = __inp(COM1_LCR);					// read the value written
	i += wsprintf(&szBuffer[i], szMes4);
	i += wsprintf(&szBuffer[i], szMes5, byVal);	// prepare the message for the value read
	if (bSucc && (byVal == 0x55))
		i += wsprintf(&szBuffer[i], szMes6);
	else
		i += wsprintf(&szBuffer[i], szMes7);

	// Set the coordinates of the display area and display the messages
	DisplWindow(hWnd);

	CloseHandle(hFile);
	HwClose();
	return 0;
}

//--------------------------------------------------------------------
// Function DisplWindow
//--------------------------------------------------------------------
//
// Function:	Displays the contents of the application window
//
// Parameters:	hWnd - Handle to the application window
//
// Returns:		-
//
//--------------------------------------------------------------------

void DisplWindow(HWND hWnd)
{
	RECT rc;									// structure with the coordinates of window corners
	HDC  hDc;									// handle to the device context

	GetClientRect(hWnd, &rc);					// determine the coordinates of client window
	rc.left = 10;								// set the margins of display area
	rc.top = 10;
	hDc = GetDC (hWnd);							// determine the device context
	DrawText(hDc, szBuffer, -1, &rc, DT_LEFT | DT_EXTERNALLEADING | DT_EXPANDTABS);
	ReleaseDC(hWnd, hDc);
}
