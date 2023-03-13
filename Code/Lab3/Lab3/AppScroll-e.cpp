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

	for (int i = 0; i < 255; i++) {
		for (int j = 0; j < 32; j++) {
			for (int k = 0; k < 8; k++) {
				PCI_CONFIG0* computedBaseAddress = getPCIHeader(i, j, k);
				DWORD classCode = _inmw((DWORD_PTR)&computedBaseAddress->BaseClass);
				DWORD subclassCode = _inmw((DWORD_PTR)&computedBaseAddress->SubClass);
				DWORD programmingInterface = _inmw((DWORD_PTR)&computedBaseAddress->ProgInterface);
				DWORD subsystemVendorId = _inmw((DWORD_PTR)&computedBaseAddress->SubSystVendorID);
				DWORD subsystemId = _inmw((DWORD_PTR)&computedBaseAddress->SubSystID);
				DWORD vendorId = _inmw((DWORD_PTR)&computedBaseAddress->VendorID);
				DWORD deviceId = _inmw((DWORD_PTR)&computedBaseAddress->DeviceID);

				//marker that device is valid or not 
				if (vendorId != 0xFFFF && deviceId != 0xFFFF) {
					wsprintf(szBuffer[cLine], "%x %x %x", i, j, k);
					cLine++;
					wsprintf(szBuffer[cLine], "%x %x %x %x %x", classCode, subclassCode, programmingInterface, subsystemVendorId, subsystemId);
					cLine++;
					for (int k2 = 0; k2 < 179; k2++) {
						if (PciClassTable[k2].Class == classCode && PciClassTable[k2].SubClass == subclassCode && PciClassTable[k2].ProgIf == programmingInterface) {
							wsprintf(szBuffer[cLine], "%s %s", PciClassTable[k2].ClassDesc, PciClassTable[k2].ProgIfDesc);
						}
					}
					cLine++;
					for (int k2 = 0; k2 < 1568; k2++) {
						if (PciVenTable[k2].VenId == vendorId) {
							wsprintf(szBuffer[cLine], "%s %s", PciVenTable[k2].VenShort, PciVenTable[k2].VenFull);
						}
					}
					cLine++;
					for (int k2 = 0; k2 < 7839; k2++) {
						if (PciDevTable[k2].VenId == vendorId && PciDevTable[k2].DevId == deviceId) {
							wsprintf(szBuffer[cLine], "%s", PciDevTable[k2].ChipDesc);
						}
					}
					cLine++;
				}
			}
		}
	}



	// Display the messages
	DisplWindow(hWnd);

	HwClose();
	return 0;
}
