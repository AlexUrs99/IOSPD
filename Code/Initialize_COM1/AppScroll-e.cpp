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

#define BINARY_RATE 115_200

#include <windows.h>
#include "Hw.h"
#include "ComDef-e.h"

#define NLIN 500								// number of lines in the display window
#define NCOL 240								// number of columns in the display window

// Global variables
char szBuffer[NLIN][NCOL];						// buffer for the window contents
int  cLine;										// number of current line in the display buffer

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

void initCOM1() {
	INT lcrValue = __inp(COM1_LCR);
	__outp(COM1_LCR, lcrValue | LCR_DLAB);
	__outp(COM1_DLR_LSB, 1);
	__outp(COM1_DLR_MSB, 0);

	INT lcrConfig = 0b00000011;
	__outp(COM1_LCR, lcrConfig);

	INT mcrConfig = 0b00001011;
	__outp(COM1_MCR, mcrConfig);
}

void sendStringOverToCOM1()
{
	char toSend[] = "StringToSend";
	for (int i = 0; i < strlen(toSend); i++) {
		sendCharacter(toSend[i]);
	}
}

void sendCharacter(char c) {
	__outp(COM1_THR, c);
	while ((__inp(COM1_LSR) & LSR_THR_EMPTY) == 0); //Asa astepti pana cand se transmite caracterul
}

void receiveStringOverCOM1() {
	char receivedString[] = "ReceivedString\\";
	for (int i = 0; i < strlen(receivedString); i++) {
		if (receivedString[i] != '\\') {
			receiveCharacter();

		}
	}
}

INT receiveCharacter() {
	while ((__inp(COM1_LSR) & LSR_DATA_READY) == 0);
	return __inp(COM1_RBR);
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

	initCOM1();

	//1.13.7
	sendStringOverToCOM1();

	//1.13.8
	receiveStringOverCOM1();

	// Display the messages
	DisplWindow(hWnd);

	HwClose();
	return 0;
}
