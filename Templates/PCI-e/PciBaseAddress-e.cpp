#include <windows.h>
#include "Hw.h"

//-----------------------------------------------------------------------------------
// Function PciBaseAddress
//-----------------------------------------------------------------------------------
// Function:	- Searches in memory the RSDP (Root System Description Pointer)
//				  structure
//				- Determines the address of RSDT (Root System Description Table)
//				- Reads from RSDT pointers to description tables and searches
//				  the ACPI MCFG table
//				- Reads from the ACPI MCFG table the base address of the PCI
//				  Express extended configuration space
//-----------------------------------------------------------------------------------
// Parameters	-
// Returns:		Base address of the PCI Express extended configuration space
//				0 - ACPI MCFG table not found
//				1 - RSDP structure not found
//-----------------------------------------------------------------------------------

DWORD64 PciBaseAddress(void)
{
	int i, j;
	bool bMcfg;
	DWORD dwAdr;
	DWORD dwMemVal;
	PBYTE pbAdrRSDT = NULL;
	PBYTE pbAdrTab  = NULL;
	PBYTE pbAdrMCFG = NULL;
	DWORD dwLungRSDT;
	DWORD dwAdrExtLo;
	DWORD dwAdrExtHi;
	DWORD64 qwAdrExt = 0;

	// Scan the memory for searching the "RSD PTR " signature
	for (dwAdr = 0x000E0000; dwAdr < 0x000FFFF0; dwAdr += 0x10)  {
		dwMemVal = _inmdw((DWORD_PTR)dwAdr);
		if (LOBYTE(LOWORD(dwMemVal)) == 'R' && HIBYTE(LOWORD(dwMemVal)) == 'S' &&
			LOBYTE(HIWORD(dwMemVal)) == 'D' && HIBYTE(HIWORD(dwMemVal)) == ' ') {
			dwMemVal = _inmdw((DWORD_PTR)dwAdr + 4);
			if (LOBYTE(LOWORD(dwMemVal)) == 'P' && HIBYTE(LOWORD(dwMemVal)) == 'T' &&
				LOBYTE(HIWORD(dwMemVal)) == 'R' && HIBYTE(HIWORD(dwMemVal)) == ' ') {

				// Read the physical address of RSDT
				dwMemVal = _inmdw((DWORD_PTR)dwAdr + 16);
				pbAdrRSDT = (PBYTE)(UINT_PTR)dwMemVal;
				dwLungRSDT = _inmdw((DWORD_PTR)pbAdrRSDT + 4);
				dwLungRSDT -= 36;					// subtract the header length
				pbAdrRSDT += 36;					// start address of pointer area

				// At the address indicated by each pointer, search the "MCFG" signature
				for (i = 0, bMcfg = FALSE; i < (int)(dwLungRSDT / 4); i++) {
					// Read a pointer from RSDT
					dwMemVal = _inmdw ((DWORD_PTR)pbAdrRSDT);
					pbAdrRSDT += 4;
					pbAdrTab = (PBYTE)(UINT_PTR)dwMemVal;
					// Read the signature
					dwMemVal = _inmdw((DWORD_PTR)pbAdrTab);
					if (LOBYTE(LOWORD(dwMemVal)) == 'M' && HIBYTE(LOWORD(dwMemVal)) == 'C' &&
						LOBYTE(HIWORD(dwMemVal)) == 'F' && HIBYTE(HIWORD(dwMemVal)) == 'G') {
						bMcfg = TRUE;
						break;
					}
				}
				if (bMcfg == FALSE) {				// ACPI MCFG table not found
					return 0;
				}

				pbAdrMCFG = pbAdrTab;				// save the start address of the MCFG table

				// Determine the number of structures with information about 
				// the extended configuration space
				// Read the length of MCFG table
				dwMemVal = _inmdw((DWORD_PTR)pbAdrTab + 4);
				dwMemVal -= 44;						// subtract the header length
				j = (int)(dwMemVal / 16);			// number of structures
				pbAdrTab += 44;						// start address of first structure

				// Read the base address from structure 0 of the MCFG table
				dwAdrExtLo = _inmdw((DWORD_PTR)pbAdrTab);
				dwAdrExtHi = _inmdw((DWORD_PTR)pbAdrTab + 4);
				qwAdrExt = ((DWORD64)dwAdrExtHi << 32) | dwAdrExtLo;
			}
			else return 1;							// RSDP structure not found
		}
	}
	return (qwAdrExt);
}

