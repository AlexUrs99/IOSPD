/****************************************************************************

 FILE     : HW.H
 PURPOSE  : Function prototypes for HW/64.SYS/.VXD/.DLL
 CREATED  : March 1996
 BY       : Ron Yazma, Albert Quan
            Copyright (c), Marvin Test Solutions, Inc. All Rights Reserved.

 COMMENTS : To use the HW.SYS/VXD/DLL driver include this file
			where you open and close the device.
			This file should also be included in all sources
			that uses port/memory I/O to replace the compiler intrinsic functions
			and to provide compatibility to 16 apps that
			uses outp instead of _outp.
			You should also include CONIO.H where you have port I/O.
 DEFINES :              
            _MSC_VER - when compiling under VC
            _WINDOWS - _MSC_VER, _WIN32, _WIN64, LVRT
            _WIN32   - _MSC_VER, _WINDOWS, LVRT
            _WIN64   - _MSC_VER, _WINDOWS, _WIN32
            LVRT     - _MSC_VER, _WINDOWS, _WIN32
            __GNUC__ - when compiling under LINUX
            LINUX    - __GNUC__, LVRT

****************************************************************************/
#ifdef _MSC_VER
#pragma once
#endif

#ifndef __HW_HEADER__
#define __HW_HEADER__

#ifndef HW_SYS
#ifdef _MSC_VER
#include <Windows.h>
#if _MSC_VER>1400		// VS 2005
#include <VersionHelpers.h>
#endif					// _MSC_VER<900
#endif      // _MSC_VER
#endif      // HW_SYS

#ifdef __GNUC__
#define LVRT
#define LINUX
#include <wchar.h>
#include <unistd.h>
#define	PASCAL
#define WINAPI
#define HWCALL
typedef void *      HANDLE;
typedef void *      HMODULE;
typedef void *      HINSTANCE;
typedef void *      HHOOK;
typedef void *      HWND;
#elif defined(HWDLL)		// when using hw.dll
#define HWCALL WINAPI
#else
#define HWCALL _cdecl
#endif      // __GNUC__

// define data types and constants
#define	VOID							void
typedef VOID							*PVOID, *LPVOID;
typedef char							CHAR, *PCHAR, *LPCHAR, *PSTR, *LPSTR;
typedef const CHAR 						*PCSTR, *LPCSTR;
typedef wchar_t							WCHAR, *PWCHAR, *LPWCH, *PWSTR, *LPWSTR;
typedef const WCHAR						*PCWCH, *LPCWCH;
typedef unsigned char				    BYTE, UCHAR, *PBYTE, *LPBYTE, *PUCHAR;
typedef short							SHORT, *PSHORT, *LPSHORT;
typedef unsigned short      			WORD, USHORT, *PWORD, *LPWORD, *PUSHORT;
#ifdef _LP64
typedef int							    LONG, *PLONG, *LPLONG;
typedef unsigned int       			    DWORD, ULONG, *PDWORD, *LPDWORD, *PULONG;
#else
typedef long						    LONG, *PLONG, *LPLONG;
typedef unsigned long       			DWORD, ULONG, *PDWORD, *LPDWORD, *PULONG;
#endif
#ifndef HW_SYS
typedef int		 						BOOL, *PBOOL, *LPBOOL;
typedef int								INT, *PINT, *LPINT;
typedef unsigned int					UINT, *PUINT;
typedef float							FLOAT, *PFLOAT;
typedef double							DOUBLE, *PDOUBLE;

#ifdef _WIN32
typedef signed __int64					INT64, LONGLONG, *PINT64;
typedef unsigned __int64				UINT64, ULONGLONG, *PUINT64;
#else
typedef long long						INT64, LONGLONG, *PINT64;
typedef unsigned long long 				UINT64, ULONGLONG, *PUINT64;
#endif      //_WIN32

#ifdef UNICODE
typedef WCHAR							TCHAR;
#else
typedef CHAR							TCHAR;
#endif      //UNICODE
typedef	TCHAR							*PTSTR, *LPTSTR;
typedef	const	TCHAR					*PCTSTR, *LPCTSTR;

#ifdef _WIN64
#define POINTER_32	__ptr32
typedef	INT64							INT_PTR, LONG_PTR;
typedef	UINT64							UINT_PTR, ULONG_PTR, DWORD_PTR;
#elif defined(_LP64)
#define POINTER_32
typedef	long							INT_PTR, LONG_PTR;
typedef unsigned long                   UINT_PTR, ULONG_PTR, DWORD_PTR;
#else
#define POINTER_32
typedef	INT								INT_PTR;
typedef	UINT							UINT_PTR;
typedef	LONG							LONG_PTR;
typedef	ULONG							ULONG_PTR, DWORD_PTR;
#endif      // _WIN64

#define FALSE							0
#define TRUE							1

#endif      // HW_SYS

typedef void * POINTER_32				PVOID32;

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#if _MSC_VER<=1400		// VS 2005
	#define IsWinNT2K()	(((LONG)GetVersion())>0 && LOBYTE(LOWORD(GetVersion()))>4)
	#define IsWinNT4()	(((LONG)GetVersion())>0 && LOBYTE(LOWORD(GetVersion()))<5)
	#define IsWinNT()	(((LONG)GetVersion())>0)
	#define IsWin9x()	(((LONG)GetVersion())<0 && LOBYTE(LOWORD(GetVersion()))>3)
#else
	// XP or newer
	#define IsWinNT2K()	IsWindowsVersionOrGreater(5, 0, 0)  
	// if exactly Win NT 4.0
	#define IsWinNT4()	(IsWindowsVersionOrGreater(4, 0, 0) && !IsWinNT2K())   
	// Win NT or greater (not 9x )
	#define IsWinNT()	IsWindowsVersionOrGreater(4, 0, 0)
	// not supported any more (9x) so always false
	#define IsWin9x()	FALSE													
#endif					// _MSC_VER<900
	BOOL IsWow64();
	BOOL IsWin64();
	BOOL DisableWow64FsRedirection(PVOID * pOldValue);
	BOOL RevertWow64FsRedirection(PVOID pOldValue);
#elif _WINDOWS
	#define WF_WINNT    0x4000
	#define IsWinNT()	((GetWinFlags() & WF_WINNT)!=0)
#elif defined (__GNUC__)
	#define IsWinNT4()	(FALSE)
#else
	#define IsWinNT()	(!stricmp(getenv("OS"), "Windows_NT"))
#endif

#define HW_SIGNATURE	0xA123B456ul					// signature for special encoding of the buffer in string inp/out from 16 bit code to the VDD

BOOL HWCALL		HwOpen(void);							// Opens the HW device, must be called before port I/O
BOOL HWCALL		HwClose(void);							// Closes the HW device must be called before exiting
BOOL HWCALL		HwIsOpen(void);							// TRUE if device is allready open
DWORD HWCALL	HwGetVersion(PSTR sz, LONG lSzSize);	// returns HW version from registry
BOOL HWCALL		HwIsInUse(void);						// TRUE if HW is being used

// for compat to version 1.x
#define HWOpen HwOpen
#define HWClose HwClose
#define HWIsOpen HwIsOpen

INT	HWCALL		__inp(WORD wPort);
WORD HWCALL		__inpw(WORD wPort);
DWORD HWCALL	__inpd(WORD wPort);
INT HWCALL		__outp(WORD wPort, INT iData);
WORD HWCALL		__outpw(WORD wPort, WORD wData);
DWORD HWCALL	__outpd(WORD wPort, DWORD dwData);

#define outp	__outp									// for compatiability of 16 bits apps
#define outpw	__outpw
#define outpdw	__outpdw
#define inp		__inp
#define inpw	__inpw
#define inpdw	__inpdw

#if defined(_WIN32) || defined(LINUX)
// VC 1.5 cannot handle _asm with 32 bit operands, so, these are excluded
DWORD  HWCALL	__outpdw(WORD wPort, DWORD dwData);		
DWORD HWCALL	__inpdw(WORD wPort);
#endif		// WIN32

// port i/o string
DWORD HWCALL	_outps(WORD wPort, PUCHAR pucBuffer, DWORD dwCount);
DWORD HWCALL	_outpsx(WORD wPort, PUCHAR pcuBuffer, DWORD dwCount);
				
DWORD HWCALL	_inps(WORD wPort, PUCHAR pucBuffer, DWORD dwCount);
DWORD HWCALL	_inpsx(WORD wPort, PUCHAR pucBuffer, DWORD dwCount);
				
DWORD HWCALL	_outpsw(WORD wPort, PUSHORT pwBuffer, DWORD dwCount);
DWORD HWCALL	_outpswx(WORD wPort, PUSHORT pwBuffer, DWORD dwCount);
				
DWORD HWCALL	_inpsw(WORD wPort, PUSHORT pwBuffer, DWORD dwCount);
DWORD HWCALL	_inpswx(WORD wPort, PUSHORT pwBuffer, DWORD dwCount);

#if defined(_WIN32) || defined(__GNUC__)

// VC 1.5 cannot handle _asm with 32 bit operands, so, these are excluded
DWORD HWCALL	_outpsdw(WORD wPort, PULONG pdwBuffer, DWORD dwCount);
DWORD HWCALL	_outpsdwx(WORD wPort, PULONG pdwBuffer, DWORD dwCount);
				
DWORD HWCALL	_inpsdw(WORD wPort, PULONG pdwBuffer, DWORD dwCount);
DWORD HWCALL	_inpsdwx(WORD wPort, PULONG pdwBuffer, DWORD dwCount);

// memory i/o
BOOL  HWCALL	_outm(DWORD_PTR dwAddress, int iData);
BOOL  HWCALL	_outmw(DWORD_PTR dwAddress, WORD wData);
BOOL  HWCALL	_outmdw(DWORD_PTR dwAddress, DWORD dwData);
BYTE  HWCALL	_inm(DWORD_PTR dwAddress);
WORD  HWCALL	_inmw(DWORD_PTR dwAddress);
DWORD HWCALL	_inmdw(DWORD_PTR dwAddress);

BOOL HWCALL		_inms(DWORD_PTR dwAddress, PUCHAR pucBuffer, DWORD_PTR dwCount);
BOOL HWCALL		_inmsw(DWORD_PTR dwAddress, PUSHORT pwBuffer, DWORD_PTR dwCount);
BOOL HWCALL		_inmsdw(DWORD_PTR dwAddress, PULONG pdwBuffer, DWORD_PTR dwCount);
BOOL HWCALL		_outms(DWORD_PTR dwAddress, PUCHAR pucBuffer, DWORD_PTR dwCount);
BOOL HWCALL		_outmsw(DWORD_PTR dwAddress, PUSHORT pwBuffer, DWORD_PTR dwCount);
BOOL HWCALL		_outmsdw(DWORD_PTR dwAddress, PULONG pdwBuffer, DWORD_PTR dwCount);

// memory descriptor
#ifdef __GNUC__
typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;
#endif
#define TIME LARGE_INTEGER
typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

// Memory descriptor

typedef struct tagHWMEMORYDESC32
{
	PHYSICAL_ADDRESS	phaddr;				// physical address (after map: 9x page number in low part, win32 contain pmdl in high part, win64 contains pmdl. used by unmap)
	ULONG				dwLength;			// length
	PVOID32				pvVirtualAddress;	// kernel or user mode virtual address, force 32 bits for win64

} HWMEMORYDESC32, * PHWMEMORYDESC32;

typedef struct tagHWMEMORYDESC
{
	PHYSICAL_ADDRESS	phaddr;				// physical address (after map: 9x page number in low part, win32 contain pmdl in high part, win64 contains pmdl. used by unmap)
	ULONG_PTR			dwLength;			// 32/64 bits length
	PVOID				pvVirtualAddress;	// kernel or user mode virtual address, 32/64 bits
} HWMEMORYDESC, * PHWMEMORYDESC;

// Memory alloc/free DMA parametrs
typedef struct tagHWMEMORYDMA32
{
	UCHAR				ucSize;				// structure size for future HW versions
	UCHAR				ucBusNumber;		// requested pci bus # 
	ULONG				dwLength;			// requested byte length
	HWMEMORYDESC32		memdesc;			// filled by hw.sys
	PVOID32				pDmaAdapter;		// filled by hw.sys	
} HWMEMORYDMA32, * PHWMEMORYDMA32;

typedef struct tagHWMEMORYDMA
{
	UCHAR			ucSize;				// structure size for future HW versions
	UCHAR			ucBusNumber;		// requested pci bus # 
	ULONG_PTR		dwLength;			// requested byte length
	HWMEMORYDESC	memdesc;			// filled by hw.sys
	PVOID			pDmaAdapter;		// filled by hw.sys	
} HWMEMORYDMA, * PHWMEMORYDMA;

// memory mapping
BOOL HWCALL		HwMemoryMap(PHWMEMORYDESC pmemdesc, DWORD_PTR dwAddress, DWORD_PTR dwLength);
BOOL HWCALL		HwMemoryUnMap(PHWMEMORYDESC pmemdesc);
VOID HWCALL		HwMemoryFlush(PVOID pMemAddr);		// Flush the write pipe

// DMA memory allocation
BOOL HWCALL     HwMemoryAlloc(PHWMEMORYDMA pmemdma, BYTE ucBusNumber, DWORD_PTR dwLength);
BOOL HWCALL     HwMemoryFree(PHWMEMORYDMA pmemdma);

// PXI P&P pxixsys constants
#define HW_PXISYS_INI				"pxisys.ini"
#define HW_PXIESYS_INI				"pxiesys.ini"
#define HW_GXPXISYS_INI				"GxPxiSys.ini"
#define HW_PXISYS_CHASSIS			"Chassis%d"
#define HW_PXISYS_CHASSIS_SLOT		"Chassis%dSlot%d"
#define HW_PXISYS_SLOT				"Slot%d"
#define HW_GXPXISYS_LEGACY_SLOT		"Legacy_Slot%d"
#define HW_GXPXISYS_ALIAS			"Alias_%s"
#define HW_VISA_RESOURCE_SLOT_FUNC	"PXI0::CHASSIS%i::SLOT%i::FUNC::%i::INSTR"
#define HW_VISA_RESOURCE_SLOT		"PXI0::CHASSIS%i::SLOT%i::INSTR"
#define HW_VISA_RESOURCE_BUS_FUNC	"PXI0::%i-%i.%i::INSTR"
#define HW_VISA_RESOURCE_BUS		"PXI0::%i-%i::INSTR"
#define HW_VISA_RESOURCE_BUSDEV		"PXI%d::%d::INSTR"
#define HW_VISA_RESOURCE_BUSDEV_FUNC "PXI%d::%d::%d::INSTR"

#define HW_PXISYS_EXPRESS_BUS		"ExpressBusNumber"
#define HW_PXISYS_BUS				"PCIBusNumber"
#define HW_PXISYS_DEVICE			"PCIDeviceNumber"
#define HW_PXISYS_FUNCTION			"PCIFunctionNumber"
#define HW_GXPXISYS_VISA_RESOURCE	"VisaResource"

#define PCI_MAX_BUS				256
#define PCI_MAX_DEVICES			32
#define PCI_MAX_FUNCTION		8
#define PCI_MAX_RES				8
#define PCI_MAX_BUSESDEVICES	256
#define PCI_MAX_SLOTS           256

#define PCI_INVALID_VENDORID    0xFFFF
#define PCI_VENDORID_MTS		0x16E2

// NTDDK.h
#ifndef _NTDDK_			
#ifndef WIN9X	// included in 95 DDK (ndis.h)

typedef struct _PCI_SLOT_NUMBER {
    union {
        struct {
            ULONG   DeviceNumber:5;
            ULONG   FunctionNumber:3;
            ULONG   Reserved:24;
        } bits;
        ULONG   AsULONG;
    } u;
} PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;

#define PCI_TYPE0_ADDRESSES             6
#define PCI_TYPE1_ADDRESSES             2
#define PCI_TYPE2_ADDRESSES             5

typedef struct _PCI_COMMON_CONFIG {
    USHORT  VendorID;                   // (ro)
    USHORT  DeviceID;                   // (ro)
    USHORT  Command;                    // Device control
    USHORT  Status;
    UCHAR   RevisionID;                 // (ro)
    UCHAR   ProgIf;                     // (ro)
    UCHAR   SubClass;                   // (ro)
    UCHAR   BaseClass;                  // (ro)
    UCHAR   CacheLineSize;              // (ro+)
    UCHAR   LatencyTimer;               // (ro+)
    UCHAR   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test

    union {
        struct _PCI_HEADER_TYPE_0 {
            ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];	// bit 0 is 1 for I/O registers space if 0 (memory space) ignore bits 0-2
            ULONG   CIS;
            USHORT  SubVendorID;
            USHORT  SubSystemID;
            ULONG   ROMBaseAddress;
            UCHAR   CapabilitiesPtr;
            UCHAR   Reserved1[3];
            ULONG   Reserved2;
            UCHAR   InterruptLine;      //
            UCHAR   InterruptPin;       // (ro)
            UCHAR   MinimumGrant;       // (ro)
            UCHAR   MaximumLatency;     // (ro)
        } type0;

// end_wdm end_ntminiport end_ntndis

        //
        // PCI to PCI Bridge
        //

        struct _PCI_HEADER_TYPE_1 {
            ULONG   BaseAddresses[PCI_TYPE1_ADDRESSES];
            UCHAR   PrimaryBus;
            UCHAR   SecondaryBus;
            UCHAR   SubordinateBus;
            UCHAR   SecondaryLatency;
            UCHAR   IOBase;
            UCHAR   IOLimit;
            USHORT  SecondaryStatus;
            USHORT  MemoryBase;
            USHORT  MemoryLimit;
            USHORT  PrefetchBase;
            USHORT  PrefetchLimit;
            ULONG   PrefetchBaseUpper32;
            ULONG   PrefetchLimitUpper32;
            USHORT  IOBaseUpper16;
            USHORT  IOLimitUpper16;
            UCHAR   CapabilitiesPtr;
            UCHAR   Reserved1[3];
            ULONG   ROMBaseAddress;
            UCHAR   InterruptLine;
            UCHAR   InterruptPin;
            USHORT  BridgeControl;
        } type1;

        //
        // PCI to CARDBUS Bridge
        //

        struct _PCI_HEADER_TYPE_2 {
            ULONG   SocketRegistersBaseAddress;
            UCHAR   CapabilitiesPtr;
            UCHAR   Reserved;
            USHORT  SecondaryStatus;
            UCHAR   PrimaryBus;
            UCHAR   SecondaryBus;
            UCHAR   SubordinateBus;
            UCHAR   SecondaryLatency;
            struct  {
                ULONG   Base;
                ULONG   Limit;
            }       Range[PCI_TYPE2_ADDRESSES-1];
            UCHAR   InterruptLine;
            UCHAR   InterruptPin;
            USHORT  BridgeControl;
        } type2;

// begin_wdm begin_ntminiport begin_ntndis

    } u;

    UCHAR   DeviceSpecific[192];

} PCI_COMMON_CONFIG, * PPCI_COMMON_CONFIG;

#define PCI_COMMON_HDR_LENGTH (FIELD_OFFSET (PCI_COMMON_CONFIG, DeviceSpecific))

// Bit encodings for  PCI_COMMON_CONFIG.HeaderType
#define PCI_MULTIFUNCTION                   0x80
#define PCI_DEVICE_TYPE                     0x00
#define PCI_BRIDGE_TYPE                     0x01

// Bit encodings for PCI_COMMON_CONFIG.Command
#define PCI_ENABLE_IO_SPACE                 0x0001
#define PCI_ENABLE_MEMORY_SPACE             0x0002
#define PCI_ENABLE_BUS_MASTER               0x0004
#define PCI_ENABLE_SPECIAL_CYCLES           0x0008
#define PCI_ENABLE_WRITE_AND_INVALIDATE     0x0010
#define PCI_ENABLE_VGA_COMPATIBLE_PALETTE   0x0020
#define PCI_ENABLE_PARITY                   0x0040  // (ro+)
#define PCI_ENABLE_WAIT_CYCLE               0x0080  // (ro+)
#define PCI_ENABLE_SERR                     0x0100  // (ro+)
#define PCI_ENABLE_FAST_BACK_TO_BACK        0x0200  // (ro)

// Bit encodings for PCI_COMMON_CONFIG.Status
#define PCI_STATUS_FAST_BACK_TO_BACK        0x0080  // (ro)
#define PCI_STATUS_DATA_PARITY_DETECTED     0x0100
#define PCI_STATUS_DEVSEL                   0x0600  // 2 bits wide
#define PCI_STATUS_SIGNALED_TARGET_ABORT    0x0800
#define PCI_STATUS_RECEIVED_TARGET_ABORT    0x1000
#define PCI_STATUS_RECEIVED_MASTER_ABORT    0x2000
#define PCI_STATUS_SIGNALED_SYSTEM_ERROR    0x4000
#define PCI_STATUS_DETECTED_PARITY_ERROR    0x8000


// Bit encodes for PCI_COMMON_CONFIG.u.type0.BaseAddresses
#define PCI_ADDRESS_IO_SPACE                0x00000001  // (ro)
#define PCI_ADDRESS_MEMORY_TYPE_MASK        0x00000006  // (ro)
#define PCI_ADDRESS_MEMORY_PREFETCHABLE     0x00000008  // (ro)

#define PCI_TYPE_32BIT      0
#define PCI_TYPE_20BIT      2
#define PCI_TYPE_64BIT      4

// Bit encodes for PCI_COMMON_CONFIG.u.type0.ROMBaseAddresses
#define PCI_ROMADDRESS_ENABLED              0x00000001

// Reference notes for PCI configuration fields:

// Define the I/O bus interface types.
typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

// Define types of bus information.
typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos,
    EisaConfiguration,
    Position,
    CbusConfiguration,
    PCIConfiguration,
    VMEConfiguration,
    NuBusConfiguration,
    PCMCIAConfiguration,
    MPIConfiguration,
    MPSAConfiguration,
    PNPISAConfiguration,
    MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;

// Define the DMA transfer widths.
typedef enum _DMA_WIDTH {
    Width8Bits,
    Width16Bits,
    Width32Bits,
    MaximumDmaWidth
} DMA_WIDTH, *PDMA_WIDTH;

// Define DMA transfer speeds.
typedef enum _DMA_SPEED {
    Compatible,
    TypeA,
    TypeB,
    TypeC,
    MaximumDmaSpeed
} DMA_SPEED, *PDMA_SPEED;

// Resource List definitions

// Defines the Type in the RESOURCE_DESCRIPTOR
typedef enum _CM_RESOURCE_TYPE {
    CmResourceTypeNull = 0,    // Reserved
    CmResourceTypePort,
    CmResourceTypeInterrupt,
    CmResourceTypeMemory,
    CmResourceTypeDma,
    CmResourceTypeDeviceSpecific,
    CmResourceTypeMaximum
} CM_RESOURCE_TYPE;

// Defines the ShareDisposition in the RESOURCE_DESCRIPTOR
typedef enum _CM_SHARE_DISPOSITION {
    CmResourceShareUndetermined = 0,    // Reserved
    CmResourceShareDeviceExclusive,
    CmResourceShareDriverExclusive,
    CmResourceShareShared
} CM_SHARE_DISPOSITION;

// Define the bit masks for Flags when type is CmResourceTypeInterrupt
#define CM_RESOURCE_INTERRUPT_LEVEL_SENSITIVE 0
#define CM_RESOURCE_INTERRUPT_LATCHED         1

// Define the bit masks for Flags when type is CmResourceTypeMemory
#define CM_RESOURCE_MEMORY_READ_WRITE       0x0000
#define CM_RESOURCE_MEMORY_READ_ONLY        0x0001
#define CM_RESOURCE_MEMORY_WRITE_ONLY       0x0002
#define CM_RESOURCE_MEMORY_PREFETCHABLE     0x0004

// Define the bit masks for Flags when type is CmResourceTypePort
#define CM_RESOURCE_PORT_MEMORY 0
#define CM_RESOURCE_PORT_IO 1

// This structure defines one type of resource used by a driver.
//
// There can only be *1* DeviceSpecificData block. It must be located at
// the end of all resource descriptors in a full descriptor block.
//

// BUGBUG Make sure alignment is made properly by compiler; otherwise move
// flags back to the top of the structure (common to all members of the
// union).

#pragma pack(push, 4)

typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        // Range of port numbers, inclusive. These are physical, bus
        // relative. The value should be the same as the one passed to
        // HalTranslateBusAddress().
        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        // IRQL and vector. Should be same values as were passed to
        // HalGetInterruptVector().
        struct {
            ULONG Level;
            ULONG Vector;
            ULONG Affinity;
        } Interrupt;

        // Range of memory addresses, inclusive. These are physical, bus
        // relative. The value should be the same as the one passed to
        // HalTranslateBusAddress().
        struct {
            PHYSICAL_ADDRESS Start;    // 64 bit physical addresses.
            ULONG Length;
        } Memory;

        // Physical DMA channel.
        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        // Device Specific information defined by the driver.
        // The DataSize field indicates the size of the data in bytes. The
        // data is located immediately after the DeviceSpecificData field in
        // the structure.
        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;
    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;

typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR64 {
    UCHAR Type;
    UCHAR ShareDisposition;
    USHORT Flags;
    union {

        // Range of port numbers, inclusive. These are physical, bus
        // relative. The value should be the same as the one passed to
        // HalTranslateBusAddress().
        struct {
            PHYSICAL_ADDRESS Start;
            ULONG Length;
        } Port;

        // IRQL and vector. Should be same values as were passed to
        // HalGetInterruptVector().
        struct {
            ULONG Level;
            ULONG Vector;
            UINT64 Affinity;
        } Interrupt;

        // Range of memory addresses, inclusive. These are physical, bus
        // relative. The value should be the same as the one passed to
        // HalTranslateBusAddress().
        struct {
            PHYSICAL_ADDRESS Start;    // 64 bit physical addresses.
            ULONG Length;
        } Memory;

        // Physical DMA channel.
        struct {
            ULONG Channel;
            ULONG Port;
            ULONG Reserved1;
        } Dma;

        // Device Specific information defined by the driver.
        // The DataSize field indicates the size of the data in bytes. The
        // data is located immediately after the DeviceSpecificData field in
        // the structure.
        struct {
            ULONG DataSize;
            ULONG Reserved1;
            ULONG Reserved2;
        } DeviceSpecificData;
    } u;
} CM_PARTIAL_RESOURCE_DESCRIPTOR64, *PCM_PARTIAL_RESOURCE_DESCRIPTOR64;

#pragma pack(pop)

//
// A Partial Resource List is what can be found in the ARC firmware
// or will be generated by ntdetect.com.
// The configuration manager will transform this structure into a Full
// resource descriptor when it is about to store it in the regsitry.
//
// Note: There must a be a convention to the order of fields of same type,
// (defined on a device by device basis) so that the fields can make sense
// to a driver (i.e. when multiple memory ranges are necessary).
//

typedef struct _CM_PARTIAL_RESOURCE_LIST {
    USHORT Version;
    USHORT Revision;
    ULONG Count;
    CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];
} CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;

#endif		// WIN9X

//
// PCI Express Capability
//
typedef union _PCI_EXPRESS_CAPABILITIES_REGISTER {
    struct {
        USHORT CapabilityVersion:4;
        USHORT DeviceType:4;               // PCI_EXPRESS_DEVICE_TYPE
        USHORT SlotImplemented:1;
        USHORT InterruptMessageNumber:5;
        USHORT Rsvd:2;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_CAPABILITIES_REGISTER, *PPCI_EXPRESS_CAPABILITIES_REGISTER;

typedef union _PCI_EXPRESS_DEVICE_CAPABILITIES_REGISTER {
    struct {
        ULONG MaxPayloadSizeSupported:3;     // EXPRESS_MAX_PAYLOAD_SIZE
        ULONG PhantomFunctionsSupported:2;
        ULONG ExtendedTagSupported:1;
        ULONG L0sAcceptableLatency:3;        // EXPRESS_L0S_LATENCY
        ULONG L1AcceptableLatency:3;         // EXPRESS_L1_LATENCY
        ULONG Undefined:3;
        ULONG RoleBasedErrorReporting:1;
        ULONG Rsvd1:2;
        ULONG CapturedSlotPowerLimit:8;
        ULONG CapturedSlotPowerLimitScale:2;
        ULONG FunctionLevelResetCapability:1;
        ULONG Rsvd2:3;
    } DUMMYSTRUCTNAME;
    ULONG AsULONG;
} PCI_EXPRESS_DEVICE_CAPABILITIES_REGISTER, *PPCI_EXPRESS_DEVICE_CAPABILITIES_REGISTER;

//
// PCI Capability IDs
//

#define PCI_CAPABILITY_ID_POWER_MANAGEMENT  0x01
#define PCI_CAPABILITY_ID_AGP               0x02
#define PCI_CAPABILITY_ID_VPD               0x03
#define PCI_CAPABILITY_ID_SLOT_ID           0x04
#define PCI_CAPABILITY_ID_MSI               0x05
#define PCI_CAPABILITY_ID_CPCI_HOTSWAP      0x06
#define PCI_CAPABILITY_ID_PCIX              0x07
#define PCI_CAPABILITY_ID_HYPERTRANSPORT    0x08
#define PCI_CAPABILITY_ID_VENDOR_SPECIFIC   0x09
#define PCI_CAPABILITY_ID_DEBUG_PORT        0x0A
#define PCI_CAPABILITY_ID_CPCI_RES_CTRL     0x0B
#define PCI_CAPABILITY_ID_SHPC              0x0C
#define PCI_CAPABILITY_ID_P2P_SSID          0x0D
#define PCI_CAPABILITY_ID_AGP_TARGET        0x0E
#define PCI_CAPABILITY_ID_SECURE            0x0F
#define PCI_CAPABILITY_ID_PCI_EXPRESS       0x10
#define PCI_CAPABILITY_ID_MSIX              0x11
#define PCI_CAPABILITY_ID_SATA_CONFIG       0x12
#define PCI_CAPABILITY_ID_ADVANCED_FEATURES 0x13

//
// All PCI Capability structures have the following header.
//
// CapabilityID is used to identify the type of the structure (is
// one of the PCI_CAPABILITY_ID values above.
//
// Next is the offset in PCI Configuration space (0x40 - 0xfc) of the
// next capability structure in the list, or 0x00 if there are no more
// entries.
//
typedef struct _PCI_CAPABILITIES_HEADER {
    UCHAR   CapabilityID;
    UCHAR   Next;
} PCI_CAPABILITIES_HEADER, *PPCI_CAPABILITIES_HEADER;

//
// The low 3 bits of the PCI Express device control register dictate whether
// a device that implements AER routes error messages to the root complex.
// This mask is used when programming the AER bits in the device control
// register.
//
#define PCI_EXPRESS_AER_DEVICE_CONTROL_MASK 0x07;

typedef union _PCI_EXPRESS_DEVICE_CONTROL_REGISTER {
    struct {
        USHORT CorrectableErrorEnable:1;
        USHORT NonFatalErrorEnable:1;
        USHORT FatalErrorEnable:1;
        USHORT UnsupportedRequestErrorEnable:1;
        USHORT EnableRelaxedOrder:1;
        USHORT MaxPayloadSize:3;                 // EXPRESS_MAX_PAYLOAD_SIZE
        USHORT ExtendedTagEnable:1;
        USHORT PhantomFunctionsEnable:1;
        USHORT AuxPowerEnable:1;
        USHORT NoSnoopEnable:1;
        USHORT MaxReadRequestSize:3;             // EXPRESS_MAX_PAYLOAD_SIZE
        USHORT BridgeConfigRetryEnable:1;
    } DUMMYSTRUCTNAME;

    //
    // Hack to allow alias of BridgeConfigRetryEnable bit for FLR.
    //
    struct {
        USHORT :15;
        USHORT InitiateFunctionLevelReset:1;
    } DUMMYSTRUCTNAME2;
    USHORT AsUSHORT;
} PCI_EXPRESS_DEVICE_CONTROL_REGISTER, *PPCI_EXPRESS_DEVICE_CONTROL_REGISTER;

//
// The low 4 bits of the PCI Express device status register hold AER device
// status. This mask is used when programming the AER bits in the device status
// register.
//

#define PCI_EXPRESS_AER_DEVICE_STATUS_MASK 0x0F;

typedef union _PCI_EXPRESS_DEVICE_STATUS_REGISTER {
    struct {
        USHORT CorrectableErrorDetected:1;
        USHORT NonFatalErrorDetected:1;
        USHORT FatalErrorDetected:1;
        USHORT UnsupportedRequestDetected:1;
        USHORT AuxPowerDetected:1;
        USHORT TransactionsPending:1;
        USHORT Rsvd:10;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_DEVICE_STATUS_REGISTER, *PPCI_EXPRESS_DEVICE_STATUS_REGISTER;

typedef union _PCI_EXPRESS_LINK_CAPABILITIES_REGISTER {
    struct {
        ULONG MaximumLinkSpeed:4;
        ULONG MaximumLinkWidth:6;
        ULONG ActiveStatePMSupport:2;   // EXPRESS_ASPM_CONFIG
        ULONG L0sExitLatency:3;         // EXPRESS_L0S_LATENCY
        ULONG L1ExitLatency:3;          // EXPRESS_L1_LATENCY
        ULONG ClockPowerManagement:1;
        ULONG SurpriseDownErrorReportingCapable:1;
        ULONG DataLinkLayerActiveReportingCapable:1;
        ULONG LinkBandwidthNotificationCapability:1;
        ULONG AspmOptionalityCompliance:1;
        ULONG Rsvd:1;
        ULONG PortNumber:8;
    } DUMMYSTRUCTNAME;
    ULONG AsULONG;
} PCI_EXPRESS_LINK_CAPABILITIES_REGISTER, *PPCI_EXPRESS_LINK_CAPABILITIES_REGISTER;

typedef union _PCI_EXPRESS_LINK_CONTROL_REGISTER {
    struct {
        USHORT ActiveStatePMControl:2;    // EXPRESS_ASPM_CONFIG
        USHORT Rsvd1:1;
        USHORT ReadCompletionBoundary:1;  // EXPRESS_RCB
        USHORT LinkDisable:1;
        USHORT RetrainLink:1;
        USHORT CommonClockConfig:1;
        USHORT ExtendedSynch:1;
        USHORT EnableClockPowerManagement:1;
        USHORT Rsvd2:7;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_LINK_CONTROL_REGISTER, *PPCI_EXPRESS_LINK_CONTROL_REGISTER;

typedef union _PCI_EXPRESS_LINK_STATUS_REGISTER {
    struct {
        USHORT LinkSpeed:4;
        USHORT LinkWidth:6;
        USHORT Undefined:1;
        USHORT LinkTraining:1;
        USHORT SlotClockConfig:1;
        USHORT DataLinkLayerActive:1;
        USHORT Rsvd:2;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_LINK_STATUS_REGISTER, *PPCI_EXPRESS_LINK_STATUS_REGISTER;

typedef union _PCI_EXPRESS_SLOT_CAPABILITIES_REGISTER {
    struct {
        ULONG AttentionButtonPresent:1;
        ULONG PowerControllerPresent:1;
        ULONG MRLSensorPresent:1;
        ULONG AttentionIndicatorPresent:1;
        ULONG PowerIndicatorPresent:1;
        ULONG HotPlugSurprise:1;
        ULONG HotPlugCapable:1;
        ULONG SlotPowerLimit:8;
        ULONG SlotPowerLimitScale:2;
        ULONG ElectromechanicalLockPresent:1;
        ULONG NoCommandCompletedSupport:1;
        ULONG PhysicalSlotNumber:13;
    } DUMMYSTRUCTNAME;
    ULONG AsULONG;
} PCI_EXPRESS_SLOT_CAPABILITIES_REGISTER, *PPCI_EXPRESS_SLOT_CAPABILITIES_REGISTER;

typedef union _PCI_EXPRESS_SLOT_CONTROL_REGISTER {
    struct {
        USHORT AttentionButtonEnable:1;
        USHORT PowerFaultDetectEnable:1;
        USHORT MRLSensorEnable:1;
        USHORT PresenceDetectEnable:1;
        USHORT CommandCompletedEnable:1;
        USHORT HotPlugInterruptEnable:1;
        USHORT AttentionIndicatorControl:2;  // EXPRESS_INDICATOR_STATE
        USHORT PowerIndicatorControl:2;      // EXPRESS_INDICATOR_STATE
        USHORT PowerControllerControl:1;     // EXPRESS_POWER_STATE
        USHORT ElectromechanicalLockControl:1;
        USHORT DataLinkStateChangeEnable:1;
        USHORT Rsvd:3;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_SLOT_CONTROL_REGISTER, *PPCI_EXPRESS_SLOT_CONTROL_REGISTER;

typedef union _PCI_EXPRESS_SLOT_STATUS_REGISTER {
    struct {
        USHORT AttentionButtonPressed:1;
        USHORT PowerFaultDetected:1;
        USHORT MRLSensorChanged:1;
        USHORT PresenceDetectChanged:1;
        USHORT CommandCompleted:1;
        USHORT MRLSensorState:1;        // EXPRESS_MRL_STATE
        USHORT PresenceDetectState:1;   // EXPRESS_CARD_PRESENCE
        USHORT ElectromechanicalLockEngaged:1;
        USHORT DataLinkStateChanged:1;
        USHORT Rsvd:7;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_SLOT_STATUS_REGISTER, *PPCI_EXPRESS_SLOT_STATUS_REGISTER;

typedef union _PCI_EXPRESS_ROOT_CONTROL_REGISTER {
    struct {
        USHORT CorrectableSerrEnable:1;
        USHORT NonFatalSerrEnable:1;
        USHORT FatalSerrEnable:1;
        USHORT PMEInterruptEnable:1;
        USHORT CRSSoftwareVisibilityEnable:1;
        USHORT Rsvd:11;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_ROOT_CONTROL_REGISTER, *PPCI_EXPRESS_ROOT_CONTROL_REGISTER;

typedef union _PCI_EXPRESS_ROOT_CAPABILITIES_REGISTER {
    struct {
        USHORT CRSSoftwareVisibility:1;
        USHORT Rsvd:15;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_ROOT_CAPABILITIES_REGISTER, *PPCI_EXPRESS_ROOT_CAPABILITIES_REGISTER;

typedef union _PCI_EXPRESS_ROOT_STATUS_REGISTER {
    struct {
        ULONG PMERequestorId:16;  // PCI_EXPRESS_REQUESTOR_ID
        ULONG PMEStatus:1;
        ULONG PMEPending:1;
        ULONG Rsvd:14;
    } DUMMYSTRUCTNAME;
    ULONG AsULONG;
} PCI_EXPRESS_ROOT_STATUS_REGISTER, *PPCI_EXPRESS_ROOT_STATUS_REGISTER;

typedef union _PCI_EXPRESS_DEVICE_CAPABILITIES_2_REGISTER {
    struct {
        ULONG CompletionTimeoutRangesSupported:4;
        ULONG CompletionTimeoutDisableSupported:1;
        ULONG AriForwardingSupported:1;
        ULONG AtomicOpRoutingSupported:1;
        ULONG AtomicOpCompleterSupported32Bit:1;
        ULONG AtomicOpCompleterSupported64Bit:1;
        ULONG CASCompleterSupported128Bit:1;
        ULONG NoROEnabledPRPRPassing:1;
        ULONG LTRMechanismSupported:1;
        ULONG TPHCompleterSupported:2;
        ULONG Rsvd:4;
        ULONG OBFFSupported:2;
        ULONG ExtendedFmtFieldSuported:1;
        ULONG EndEndTLPPrefixSupported:1;
        ULONG MaxEndEndTLPPrefixes:2;
        ULONG Rsvd2:8;
    } DUMMYSTRUCTNAME;
    ULONG AsULONG;
} PCI_EXPRESS_DEVICE_CAPABILITIES_2_REGISTER, *PPCI_EXPRESS_DEVICE_CAPABILITIES_2_REGISTER;

typedef union _PCI_EXPRESS_DEVICE_CONTROL_2_REGISTER {
    struct {
        USHORT CompletionTimeoutValue:4;
        USHORT CompletionTimeoutDisable:1;
        USHORT AriForwardingEnable:1;
        USHORT AtomicOpRequesterEnable:1;
        USHORT AtomicOpEgresBlocking:1;
        USHORT IDORequestEnable:1;
        USHORT IDOCompletionEnable:1;
        USHORT LTRMechanismEnable:1;
        USHORT Rsvd:2;
        USHORT OBFFEnable:2;
        USHORT EndEndTLPPrefixBlocking:1;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_DEVICE_CONTROL_2_REGISTER, *PPCI_EXPRESS_DEVICE_CONTROL_2_REGISTER;

typedef union _PCI_EXPRESS_DEVICE_STATUS_2_REGISTER {
    struct {
        USHORT Rsvd:16;
    } DUMMYSTRUCTNAME;
    USHORT AsUSHORT;
} PCI_EXPRESS_DEVICE_STATUS_2_REGISTER, *PPCI_EXPRESS_DEVICE_STATUS_2_REGISTER;

//
// PCI Express Capability
//

typedef struct _PCI_EXPRESS_CAPABILITY {
    PCI_CAPABILITIES_HEADER Header;
    PCI_EXPRESS_CAPABILITIES_REGISTER ExpressCapabilities;
    PCI_EXPRESS_DEVICE_CAPABILITIES_REGISTER DeviceCapabilities;
    PCI_EXPRESS_DEVICE_CONTROL_REGISTER DeviceControl;
    PCI_EXPRESS_DEVICE_STATUS_REGISTER DeviceStatus;
    PCI_EXPRESS_LINK_CAPABILITIES_REGISTER LinkCapabilities;
    PCI_EXPRESS_LINK_CONTROL_REGISTER LinkControl;
    PCI_EXPRESS_LINK_STATUS_REGISTER LinkStatus;
    PCI_EXPRESS_SLOT_CAPABILITIES_REGISTER SlotCapabilities;
    PCI_EXPRESS_SLOT_CONTROL_REGISTER SlotControl;
    PCI_EXPRESS_SLOT_STATUS_REGISTER SlotStatus;
    PCI_EXPRESS_ROOT_CONTROL_REGISTER RootControl;
    PCI_EXPRESS_ROOT_CAPABILITIES_REGISTER RootCapabilities;
    PCI_EXPRESS_ROOT_STATUS_REGISTER RootStatus;
    PCI_EXPRESS_DEVICE_CAPABILITIES_2_REGISTER DeviceCapabilities2;
    PCI_EXPRESS_DEVICE_CONTROL_2_REGISTER DeviceControl2;
    PCI_EXPRESS_DEVICE_STATUS_2_REGISTER DeviceStatus2;
} PCI_EXPRESS_CAPABILITY, *PPCI_EXPRESS_CAPABILITY;

// NOTE: For future expansion, capability registers can be copied in from NtDdk.h
//       which is included in the WDK.

#endif		// NTDDK

// pci parametrs

typedef struct PXI_SLOT_NUMBER {
    union {
        struct {
            ULONG   DeviceNumber:5;     // PCI Bus device address within the bus, low byte
            ULONG   FunctionNumber:3;   // device function , low byte
			ULONG	LegacySlot:8;       // 1-255 slot # used for PXI spec v 1.0, bits 8-15 
			ULONG	ChassisSlot:16;     // 0x203 is chassis 2 slot 3, bits 16-31
        } bits;
        ULONG   AsULONG;
    } u;
} PXI_SLOT_NUMBER;

typedef struct tagHWPCIDEVICE32
{
    ULONG							dwVendorId;				// input for HwPciGetNextDevice, out is in pcicfg
    ULONG							dwDeviceId;				// input for HwPciGetNextDevice, out is in pcicfg
    ULONG							dwBus;					// out, in start device bus number for HwPciGetNextDevice
	PXI_SLOT_NUMBER					pcisn;					// out, in start device/function numbers for HwPciGetNextDevice
	PCI_COMMON_CONFIG				pcicfg;					// out
	CM_PARTIAL_RESOURCE_DESCRIPTOR	aresdesc[PCI_MAX_RES];	// out
	HWMEMORYDESC32					amemdesc[PCI_MAX_RES];	// out
	CHAR							szId[256];				// out, registry key for device
} HWPCIDEVICE32, * PHWPCIDEVICE32;

typedef struct tagHWPCIDEVICE
{
    ULONG							dwVendorId;				// input for HwPciGetNextDevice, out is in pcicfg
    ULONG							dwDeviceId;				// input for HwPciGetNextDevice, out is in pcicfg
    ULONG							dwBus;					// out, in start device bus number for HwPciGetNextDevice
	PXI_SLOT_NUMBER					pcisn;					// out, in start device/function numbers for HwPciGetNextDevice
	PCI_COMMON_CONFIG				pcicfg;					// out
	CM_PARTIAL_RESOURCE_DESCRIPTOR	aresdesc[PCI_MAX_RES];	// out
	HWMEMORYDESC					amemdesc[PCI_MAX_RES];	// out
	CHAR							szId[256];				// out, registry key for device
} HWPCIDEVICE, * PHWPCIDEVICE;

// PXI chassis info
typedef struct PXIChassisInfo
{
	// version control
	DWORD			dwSize;                 // fill with sizeof (PXIChassisInfo) 

	// smart bridges info
	HWPCIDEVICE		apcidevBridge[4];		// array of smart bridges
	WORD			wNumberOfBridges;		// total number of smart bridges

	// chassis info
	WORD 			wChassisModel;			// 7010
	WORD 			wChassisRevision;		// A, B, C etc.
	DWORD			dwChassisSerial;		// S/N: GX7XXX-XXXX	
	CHAR			acUserDefinedData[64];	// User's defined data (e.g. Aeroflex S/N)

	// chassis slots info
	WORD			wNumberOfSlots;			// total number of slots
} PXIChassisInfo;

// pci interrupt
#ifndef WINAPI
#define WINAPI __stdcall
#endif

typedef LONG (WINAPI * HwInterruptCallback) (HANDLE hInterrupt, LPVOID pvUserData);

#define	HW_INTERRUPT_TMO_IMMEDIATE	           0
#define	HW_INTERRUPT_TMO_INFINITE		       0xFFFFFFFF

typedef enum enumHwRegAccessTypes
{
	enRegCompare=0,
	enRegRead=1,
	enRegWrite=2,
	enRegReadWrite=3,
	enRegNoAccess=4,
} enumHwRegAccessTypes;

typedef enum enumHwRegAccessWidths
{
	enReg8=0,
	enReg16=1,
	enReg32=2,
} enumHwRegAccessWidths;

typedef struct HW_INTERRUPT_REG_INFO
{
	union
	{	PHYSICAL_ADDRESS	PhysicalAddr;	// physical address
		PVOID				pVirtAddr;		// mapped kernel virtual address
	};
	ULONG				    dwAddrOffset;		// physical address offset
	ULONG				    dwAddrLength;		// virtual address map length
	enumHwRegAccessTypes	enAccessType;
	enumHwRegAccessWidths	enAccessWidth;
	ULONG				    dwMask;
	ULONG				    dwValue;
} HW_INTERRUPT_REG_INFO, *PHW_INTERRUPT_REG_INFO;

typedef struct HW_INTERRUPT_DEVICE_INFO
{
	// device address
	union
	{	ULONG			     		dwDevAddr;
		struct
		{	ULONG					dwFunction:8;
			ULONG					dwDevice:8;
			ULONG					dwBus:8;
			ULONG					dwUnused:8;
		};
	};
	// interrupt number or index
	union
	{	LONG				iIrq;				// line-based interrupt
		LONG				iMessageIdIndex;	// message-based interrupt (MSI)
	};
	// interrupt control registers
	HW_INTERRUPT_REG_INFO	InterruptDetection;
	HW_INTERRUPT_REG_INFO	InterruptRemoval;
	HW_INTERRUPT_REG_INFO	InterruptDisable;
	// reserved
	LONGLONG				llReserved;
	// interrupt callback info
	struct
	{	HwInterruptCallback pInterruptCallback;
		PVOID				pvInterruptCallbackData;
	};
	// reserved
	ULONG				    adwReserved[4];
} HW_INTERRUPT_DEVICE_INFO, *PHW_INTERRUPT_DEVICE_INFO;

// Visa slot helpers
#define HW_CHASSIS_MAX				31
#define HwMakePxiSlot(iChassis, iSlot, iFunction) (SHORT)(iSlot | (iChassis<<8) | (iFunction<<13))	
#define HwGetSlotChassis(nSlot)		((nSlot >> 8) & 0x1F)
#define HwGetSlotSlot(nSlot)		(nSlot & 0xFF)
#define HwGetSlotFunction(nSlot)	((nSlot >> 13) & 0x7)
#define	GT_BUS_VISA					0x80000000		// if ored with the bus number - use visa PXI[i]::b-d[.f]::INSTR ([interface=0], bus, device, [function=0])

// pci resources
BOOL HWCALL		HwPciResetDeviceList();								         // remove cashed device list and force enum next call to HwPciGetNextDevice
BOOL HWCALL		HwPciGetNextDevice(PHWPCIDEVICE ppcidev);			         // enum the next device, struct must be zero at first and optionaly set the vendor, device IDs
BOOL HWCALL		HwPciRegisterDevice(PHWPCIDEVICE ppcidev);			         // register and allocate device resources, required for your device under NT
INT  HWCALL		HwPciGetSlotDevice(INT iSlot, PHWPCIDEVICE ppcidev);         // returns the device in the specified slot
INT  HWCALL		HwPciAssignSlots(INT iVendorId, INT iDeviceId);		         // auto assign slot to specifc devices if not assigned, return the number of devices found
INT  HWCALL		HwPciGetChassisSlot(INT iBus, INT iDevice, INT iFunction);	 // returns the chassis slot mumber (chassis*256+slot)  for the specified device
INT  HWCALL		HwPciGetLegacySlot(INT iBus, INT iDevice, INT iFunction);	 // returns the legacy slot number for the specified device
INT  HWCALL		HwPciSetLegacySlot(INT iSlot, INT iBus, INT iDevice, INT iFunction);	// returns the legacy slot number for the specified device (1-255)
INT  HWCALL		HwPciGetSlotVisaResource(INT iSlot, PSTR pszResource);       // returns the VISA resource for the specified slot
INT  HWCALL		HwPciGetAliasVisaResource(PCSTR pszAlias, PSTR pszResource); // returns the VISA resource for the specified alias
INT  HWCALL		HwPciGetAliasChassisSlot(PCSTR pszAlias);                    // returns the chassis slot for the specified alias
INT  HWCALL		HwPciGetAliasLegacySlot(PCSTR pszAlias);                     // returns the legacy slot for the specified alias
INT  HWCALL		HwPciGetChassisInfo(INT iChassis, PXIChassisInfo * pChassisInfo);       // returns chassis info of the specified chassis number
BOOL HWCALL		HwPciReadConfigurationSpace(DWORD dwBus, DWORD dwDevice, DWORD dwFunction, DWORD dwOffset, BYTE * pucData, DWORD dwBytes);	// read from configuration space
BOOL HWCALL		HwPciWriteConfigurationSpace(DWORD dwBus, DWORD dwDevice, DWORD dwFunction, DWORD dwOffset, BYTE * pucData, DWORD dwBytes);	// write to configuration space
BOOL HWCALL		HwPciReadCapabilityRegister(DWORD dwBus, DWORD dwDevice, DWORD dwFunction, BYTE ucCapReg, BYTE * pucData, DWORD dwBytes); // scan the configuration space for a specific register
BOOL HWCALL		HwPciReadResourceList(DWORD dwBus, DWORD dwDevice, DWORD dwFunction, PCM_PARTIAL_RESOURCE_DESCRIPTOR pucData, DWORD dwBytes);	// read device resource list
// PCI interrupts
HANDLE HWCALL	HwPciEnableInterrupt(PHW_INTERRUPT_DEVICE_INFO pInterruptInfo);
BOOL HWCALL		HwPciDisableInterrupt(HANDLE h);
BOOL HWCALL		HwPciDiscardInterrupt(HANDLE h);
BOOL HWCALL		HwPciWaitForInterrupt(HANDLE h, DWORD dwTimeOut);

#ifdef HW_DEVICE_DESCRIPTION
BOOL HWCALL		HwPciGetDeviceDescription(INT iVendorID, INT iDeviceID, PSTR szDescription);	// returns device description
#endif

BOOL HWCALL		HwScanForHardwareChanges();							        // similar to device manager scan for hardware changes

// GetSlotDevice Errors
#define HW_OK				0
#define HW_ILLEGAL_SLOT		-1
#define HW_SLOT_NOT_CONFIG	-2
#define HW_NO_DEVICE		-3
#define HW_NOT_INSTALLED	-4


#endif		// WIN32

#ifdef __cplusplus
}
#endif		// C++

#pragma pack(pop)
#endif	// __HW_HEADER__

/****************************************************************************
    END - OF - FILE
 ****************************************************************************/
