//--------------------------------------------------------------------
// TUCN, Computer Science Department
// Input/Output Systems and Peripheral Devices
//--------------------------------------------------------------------
// http://users.utcluj.ro/~baruch/en/pages/teaching/inputoutput-systems/laboratory.php
//--------------------------------------------------------------------
// File:		AppScroll-e.cpp
// Changed:		21.02.2021
//--------------------------------------------------------------------
// IOS application example with vertical scroll bar
//--------------------------------------------------------------------

#include <windows.h>
#include "Hw.h"
#include "PCI-vendor-dev.h"
#include "PCI-e.h"

#define NLIN 500								// number of lines in the display window
#define NCOL 240								// number of columns in the display window

// Global variables
char szBuffer[NLIN][NCOL];						// buffer for the window contents
int  cLine;		// number of current line in the display buffer
DWORD64 baseAddress;
// Declarations of external functions
void DisplWindow(HWND hWnd);

//--------------------------------------------------------------------
// Function AppScroll
//--------------------------------------------------------------------
//
// Function:	IOS application example with vertical scroll bar
//
// Parameters:	hWnd - Handle to the application window
//
// Returns:		0 - Operation completed successfully
//				1 - Error initializing the Hw driver
//
//--------------------------------------------------------------------

DWORD64  PciBaseAddress();

DWORD readPCIHeader(INT busNumber, INT deviceNumber, INT functionNumber, INT offset) {
	__outpdw(0x0CF8, 1 << 31 + busNumber << 16 + deviceNumber << 11 + functionNumber << 8 + offset << 2);
	return 	__inpdw(0x0CFC);
}

PCI_CONFIG0* getPCIHeader(INT busNumber, INT deviceNumber, INT functionNumber) {
	DWORD64 headerAddress = baseAddress + busNumber << 20
		+ deviceNumber << 15 + functionNumber << 12;
	return (PCI_CONFIG0*)baseAddress;
}

int AppScroll(HWND hWnd)
{
	int   i;

	char szMes0[] = "Error initializing the Hw driver";
	char szMes1[] = "IOS Application";

	// Initialize the Hw library
	if (!HwOpen()) {
		wsprintf(szBuffer[0], szMes0);
		MessageBox(NULL, szBuffer[0], "HwOpen", MB_ICONSTOP);
		return 1;
	}

	// Clear the display buffer and the window contents
	for (i = 0; i < NLIN; i++) {
		memset(szBuffer[i], ' ', NCOL);
	}
	cLine = 1;

	// Copy the start message into the display buffer and display the message
	wsprintf(szBuffer[cLine], szMes1);
	cLine += 2;
	DisplWindow(hWnd);

	//--------------------------------------------------------------------
	// To be completed with the application code
	//--------------------------------------------------------------------
	baseAddress = PciBaseAddress();
	if (baseAddress == 0 || baseAddress == 1) {
		HwClose();
		return -1;
	}

	wsprintf(szBuffer[cLine], "%llx", baseAddress);

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 32; j++) {
			for (int k = 0; k < 8; k++) {
				DWORD classCode;
				DWORD subclassCode;

				classCode = readPCIHeader(i, j, k, 2) >> 24;
				subclassCode = (readPCIHeader(i, j, k, 2) >> 16) & 0x00FF;

				if (classCode == 0x0C && subclassCode == 0x05) {
					DWORD ba0 = readPCIHeader(i, j, k, 4);
					DWORD ba1 = readPCIHeader(i, j, k, 5);
					DWORD ba2 = readPCIHeader(i, j, k, 6);
					DWORD ba3 = readPCIHeader(i, j, k, 7);
					DWORD ba4 = readPCIHeader(i, j, k, 8);
					DWORD ba5 = readPCIHeader(i, j, k, 9);

					for (int i = 0; i < 6; i++) {
						DWORD baseAddress = readPCIHeader(i, j, k, i + 4);
						INT type = baseAddress >> 1 & 0x3;

						if (type == 2) {
							wsprintf(szBuffer[cLine], "BAR: %d TYPE: %s", baseAddress,  "64");
						} else wsprintf(szBuffer[cLine], "BAR: %d TYPE: %s", baseAddress, "32");
					}
				

					/*if (ba0 >> 1 & 0x3 == 2) {
						wsprintf(szBuffer[cLine], "%llx %s", ba0, ba1, "64");
					}
					else wsprintf(szBuffer[cLine], "%llx %s", ba0, "32");
					cLine++;

					if (ba1 >> 1 & 0x3 == 2) {
						wsprintf(szBuffer[cLine], "%llx %s", ba1, ba2, "64");
					}
					else wsprintf(szBuffer[cLine], "%llx %s", ba1, "32");
					cLine++;

					if (ba2 >> 1 & 0x3 == 2) {
						wsprintf(szBuffer[cLine], "%llx %s", ba2, ba3, "64");
					}
					else wsprintf(szBuffer[cLine], "%llx %s", ba2, "32");
					cLine++;

					if (ba3 >> 1 & 0x3 == 2) {
						wsprintf(szBuffer[cLine], "%llx %s", ba3, ba4, "64");
					}
					else wsprintf(szBuffer[cLine], "%llx %s", ba3, "32");
					cLine++;

					if (ba4 >> 1 & 0x3 == 2) {
						wsprintf(szBuffer[cLine], "%llx %s", ba4, ba5, "64");
					}
					else wsprintf(szBuffer[cLine], "%llx %s", ba5, "32");
					cLine++;

					wsprintf(szBuffer[cLine], "%llx %s", ba5, "64");
					cLine++;
					*/
				}
			}
		}
	}



	// Display the messages
	DisplWindow(hWnd);

	HwClose();
	return 0;
}
