//--------------------------------------------------------------------
// TUCN, Computer Science Department
// Input/Output Systems and Peripheral Devices
//--------------------------------------------------------------------
// http://users.utcluj.ro/~baruch/en/pages/teaching/inputoutput-systems/laboratory.php
//--------------------------------------------------------------------
// File:		PCI-e.h
// Date:		09.08.2015
// Changed:		06.03.2021
//--------------------------------------------------------------------
// Definitions for accessing the PCI/PCIe configuration space
//
// Sources of information:
// - PCI Code and ID Assignment Specification, Revision 1.11, 
//   PCI-SIG, Jan. 24, 2019.
// - The PCI ID Repository, http://pci-ids.ucw.cz/
//--------------------------------------------------------------------

#ifndef _PCI_H
#define _PCI_H

#define PCI_CONFIG_ADR		0x0CF8		// Address port for accessing the configuration space
#define PCI_CONFIG_DATA		0x0CFC		// Data port for accessing the configuration space

// Structures for the PCI/PCIe configuration registers

// Structure for header Type 0
typedef struct tPCI_CONFIG0 {
	WORD		VendorID;				// 0x00 Vendor ID
	WORD		DeviceID;				// 0x02 Device ID
	WORD		Command;				// 0x04 Command Register
	WORD		Status;					// 0x06 Status Register
	BYTE		RevisionID;				// 0x08 Revision ID
	BYTE		ProgInterface;			// 0x09 Programming Interface
	BYTE		SubClass;				// 0x0A Sub-Class Code
	BYTE		BaseClass;				// 0x0B Base Class-Code
	BYTE		CacheLineSize;			// 0x0C Cache Line Size
	BYTE		LatencyTimer;			// 0x0D Master Latency Timer (0x00 for PCIe)
	BYTE		HeaderType;				// 0x0E Header Type
	BYTE		BIST;					// 0x0F Built-In Self-Test
	DWORD		BaseAddress0;			// 0x10 Base Address 0
	DWORD		BaseAddress1;			// 0x14 Base Address 1
	DWORD		BaseAddress2;			// 0x18 Base Address 2
	DWORD		BaseAddress3;			// 0x1C Base Address 3
	DWORD		BaseAddress4;			// 0x20 Base Address 4
	DWORD		BaseAddress5;			// 0x24 Base Address 5
	DWORD		CardBusCIS;				// 0x28 CardBus CIS Pointer
	WORD		SubSystVendorID;		// 0x2C Subsystem Vendor ID
	WORD		SubSystID;				// 0x2E Subsystem ID
	DWORD		ROMBaseAddress;			// 0x30 Expansion ROM Base Address
	BYTE		CapabilitiesPtr;		// 0x34 Capabilities Pointer
	BYTE		Res0[3];				// 0x35-0x37 Reserved
	DWORD		Res1;					// 0x38 Reserved
	BYTE		InterruptLine;			// 0x3C Interrupt Line
	BYTE		InterruptPin;			// 0x3D Interrupt Pin
	BYTE		MinGrant;				// 0x3E Minimum Grant (0x00 for PCIe)
	BYTE		MaxLatency;				// 0x3F Maximum Latency (0x00 for PCIe)
	DWORD		DevSpecific[48];		// 0x40-0xFF Device-Specific Registers
} PCI_CONFIG0, *PPCI_CONFIG0;

// Structure for header Type 1
typedef struct tPCI_CONFIG1 {
	WORD		VendorID;				// 0x00 Vendor ID
	WORD		DeviceID;				// 0x02 Device ID
	WORD		Command;				// 0x04 Command Register
	WORD		Status;					// 0x06 Status Register
	BYTE		RevisionID;				// 0x08 Revision ID
	BYTE		ProgInterface;			// 0x09 Programming Interface
	BYTE		SubClass;				// 0x0A Sub-Class Code
	BYTE		BaseClass;				// 0x0B Base Class-Code
	BYTE		CacheLineSize;			// 0x0C Cache Line Size
	BYTE		LatencyTimer;			// 0x0D Master Latency Timer (0x00 for PCIe)
	BYTE		HeaderType;				// 0x0E Header Type
	BYTE		BIST;					// 0x0F Built-In Self-Test
	DWORD		BaseAddress0;			// 0x10 Base Address 0
	DWORD		BaseAddress1;			// 0x14 Base Address 1
	BYTE		PrimaryBus;				// 0x18 Primary Bus Number
	BYTE		SecondaryBus;			// 0x19 Secondary Bus Number
	BYTE		SubordinateBus;			// 0x1A Subordinate Bus Number
	BYTE		SecondaryLatency;		// 0x1B Secondary Latency Timer
	BYTE		IOBase;					// 0x1C I/O Base
	BYTE		IOLimit;				// 0x1D I/O Limit
	WORD		SecondaryStatus;		// 0x1E Secondary Status Register
	WORD		MemoryBase;				// 0x20 Memory Base
	WORD		MemoryLimit;			// 0x22 Memory Limit
	WORD		PrefMemoryBase;			// 0x24 Prefetchable Memory Base
	WORD		PrefMemoryLimit;		// 0x26 Prefetchable Memory Limit
	DWORD		PrefBaseUpper32;		// 0x28 Prefetchable Base Upper 32 Bits
	DWORD		PrefLimitUpper32;		// 0x2C Prefetchable Limit Upper 32 Bits
	WORD		IOBaseUpper16;			// 0x30 I/O Base Upper 16 Bits
	WORD		IOLimitUpper16;			// 0x32 I/O Limit Upper 16 Bits
	BYTE		CapabilitiesPtr;		// 0x34 Capabilities Pointer
	BYTE		Res0[3];				// 0x35-0x37 Reserved
	DWORD		ROMBaseAddress;			// 0x38 Expansion ROM Base Address
	BYTE		InterruptLine;			// 0x3C Interrupt Line
	BYTE		InterruptPin;			// 0x3D Interrupt Pin
	WORD		BridgeControl;			// 0x3E Bridge Control
	DWORD		DevSpecific[48];		// 0x40-0xFF Device-Specific Registers
} PCI_CONFIG1, *PPCI_CONFIG1;

// Structure for the PCI/PCIe class code table
typedef struct tPCI_CLASS_TABLE
{
	BYTE		Class;					// class code
	BYTE		SubClass;				// sub-class code
	BYTE		ProgIf;					// programming interface
	CONST CHAR	*ClassDesc;				// class and sub-class descriptor
	CONST CHAR	*ProgIfDesc;			// programming interface descriptor
} PCI_CLASS_TABLE, *PPCI_CLASS_TABLE;

PCI_CLASS_TABLE PciClassTable [] =
{
	0x00, 0x00, 0x00, "Non-VGA unclassified device", "",
	0x00, 0x01, 0x00, "VGA compatible unclassified device", "",
	0x00, 0x05, 0x00, "Image coprocessor", "",

	0x01, 0x00, 0x00, "SCSI controller ", "(vendor-specific interface)",
	0x01, 0x00, 0x11, "SCSI storage device - SCSI over PCI Express port ", "(PCI Express Queuing Interface)",
	0x01, 0x00, 0x12, "SCSI controller - SCSI over PCI Express port ", "(PCI Express Queuing Interface)",
	0x01, 0x00, 0x13, "SCSI storage device and SCSI controller - SCSI over PCI Express port ", "(PCI Express Queuing Interface)",
	0x01, 0x00, 0x21, "SCSI storage device - SCSI over PCI Express port ", "(NVM Express interface)",

	0x01, 0x01, 0x00, "IDE controller ", "(ISA compatibility mode-only controller)",
	0x01, 0x01, 0x05, "IDE controller ", "(PCI native mode-only controller)",
	0x01, 0x01, 0x0A, "IDE controller ", "(ISA compatibility mode controller)",
	0x01, 0x01, 0x0F, "IDE controller ", "(PCI native mode controller)",
	0x01, 0x01, 0x80, "IDE controller ", "(ISA compatibility mode-only controller with bus mastering)",
	0x01, 0x01, 0x85, "IDE controller ", "(PCI native mode-only controller with bus mastering)",
	0x01, 0x01, 0x8A, "IDE controller ", "(ISA compatibility mode controller with bus mastering)",
	0x01, 0x01, 0x8F, "IDE controller ", "(PCI native mode controller with bus mastering)",
	0x01, 0x02, 0x00, "Floppy disk controller ", "(vendor-specific interface)",
	0x01, 0x03, 0x00, "IPI (Intelligent Peripheral Interface) bus controller ", "(vendor-specific interface)",
	0x01, 0x04, 0x00, "RAID controller ", "(vendor-specific interface)",
	0x01, 0x05, 0x20, "ATA controller ", "(ADMA interface, single stepping)",
	0x01, 0x05, 0x30, "ATA controller ", "(ADMA interface, continuous operation)",
	0x01, 0x06, 0x00, "Serial ATA (SATA) controller ", "(vendor-specific interface)",
	0x01, 0x06, 0x01, "Serial ATA (SATA) controller ", "(AHCI interface)",
	0x01, 0x06, 0x02, "Serial ATA (SATA) controller ", "(Serial Storage Bus interface)",
	0x01, 0x07, 0x00, "Serial Attached SCSI (SAS) controller ", "(vendor-specific interface)",
	0x01, 0x07, 0x01, "Serial Attached SCSI (SAS) controller ", "(Serial Storage Bus interface)",
	0x01, 0x08, 0x00, "Non-volatile memory controller ", "(vendor-specific interface)",
	0x01, 0x08, 0x01, "Non-volatile memory controller ", "(NVHCMI interface)",
	0x01, 0x08, 0x02, "NVM Express (NVMe) I/O controller", "",
	0x01, 0x08, 0x03, "NVM Express (NVMe) administrative controller", "",
	0x01, 0x80, 0x00, "Other mass storage controller ", "(vendor-specific interface)",

	0x02, 0x00, 0x00, "Ethernet controller", "",
	0x02, 0x01, 0x00, "Token Ring controller", "",
	0x02, 0x02, 0x00, "FDDI controller", "",
	0x02, 0x03, 0x00, "ATM controller", "",
	0x02, 0x04, 0x00, "ISDN controller", "",
	0x02, 0x05, 0x00, "WorldFip controller", "",
	0x02, 0x06, 0x00, "PICMG 2.14 Multi Computing", "",
	0x02, 0x07, 0x00, "InfiniBand controller", "",
	0x02, 0x08, 0x00, "Host fabric controller ", "(vendor-specific interface)",
	0x02, 0x80, 0x00, "Other network controller", "",

	0x03, 0x00, 0x00, "VGA-compatible controller", "",
	0x03, 0x00, 0x01, "8514-compatible controller", "",
	0x03, 0x01, 0x00, "XGA-compatible controller", "",
	0x03, 0x02, 0x00, "3D controller", "",
	0x03, 0x80, 0x00, "Other display controller", "",

	0x04, 0x00, 0x00, "Video device ", "(vendor-specific interface)",
	0x04, 0x01, 0x00, "Audio device ", "(vendor-specific interface)",
	0x04, 0x02, 0x00, "Computer telephony device ", "(vendor-specific interface)",
	0x04, 0x03, 0x00, "High Definition Audio (HD-A) 1.0 compatible device", "",
	0x04, 0x03, 0x80, "High Definition Audio (HD-A) 1.0 compatible device ", "(additional vendor-specific extensions)",
	0x04, 0x80, 0x00, "Other multimedia device ", "(vendor-specific interface)",

	0x05, 0x00, 0x00, "RAM memory controller", "",
	0x05, 0x01, 0x00, "Flash memory controller", "",
	0x05, 0x80, 0x00, "Other memory controller", "",

	0x06, 0x00, 0x00, "Host bridge", "",
	0x06, 0x01, 0x00, "ISA bridge", "",
	0x06, 0x02, 0x00, "EISA bridge", "",
	0x06, 0x03, 0x00, "MicroChannel (MCA) bridge", "",
	0x06, 0x04, 0x00, "PCI-to-PCI bridge ", "(normal decode)",
	0x06, 0x04, 0x01, "PCI-to-PCI bridge ", "(subtractive decode)",
	0x06, 0x05, 0x00, "PCMCIA bridge", "",
	0x06, 0x06, 0x00, "NuBus bridge", "",
	0x06, 0x07, 0x00, "CardBus bridge", "",
	0x06, 0x08, 0x00, "RACEway bridge ", "(transparent mode)",
	0x06, 0x08, 0x01, "RACEway bridge ", "(endpoint mode)",
	0x06, 0x09, 0x40, "Semi-transparent PCI-to-PCI bridge ", "(primary PCI bus towards host CPU)",
	0x06, 0x09, 0x80, "Semi-transparent PCI-to-PCI bridge ", "(secondary PCI bus towards host CPU)",
	0x06, 0x0A, 0x00, "InfiniBand-to-PCI host bridge", "",
	0x06, 0x0B, 0x00, "Advanced Switching to PCI host bridge ", "(custom interface)",
	0x06, 0x0B, 0x01, "Advanced Switching to PCI host bridge ", "(ASI-SIG defined portal interface)",
	0x06, 0x80, 0x00, "Other bridge device", "",

	0x07, 0x00, 0x00, "Generic XT-compatible serial controller", "",
	0x07, 0x00, 0x01, "16450-compatible serial controller", "",
	0x07, 0x00, 0x02, "16550-compatible serial controller", "",
	0x07, 0x00, 0x03, "16650-compatible serial controller", "",
	0x07, 0x00, 0x04, "16750-compatible serial controller", "",
	0x07, 0x00, 0x05, "16850-compatible serial controller", "",
	0x07, 0x00, 0x06, "16950-compatible serial controller", "",
	0x07, 0x01, 0x00, "Parallel port ", "(SPP)",
	0x07, 0x01, 0x01, "Parallel port ", "(Bi-directional)",
	0x07, 0x01, 0x02, "Parallel port ", "(ECP 1.x-compliant)",
	0x07, 0x01, 0x03, "IEEE 1284 controller", "",
	0x07, 0x01, 0xFE, "IEEE 1284 target device", "",
	0x07, 0x02, 0x00, "Multiport serial controller", "",
	0x07, 0x03, 0x00, "Generic modem", "",
	0x07, 0x03, 0x01, "Hayes-compatible modem ", "(16450-compatible interface)",
	0x07, 0x03, 0x02, "Hayes-compatible modem ", "(16550-compatible interface)",
	0x07, 0x03, 0x03, "Hayes-compatible modem ", "(16650-compatible interface)",
	0x07, 0x03, 0x04, "Hayes-compatible modem ", "(16750-compatible interface)",
	0x07, 0x04, 0x00, "GPIB (IEEE 488.1/2) controller", "",
	0x07, 0x05, 0x00, "Smart Card controller", "",
	0x07, 0x80, 0x00, "Other communications controller", "",

	0x08, 0x00, 0x00, "Generic 8259 Programmable Interrupt Controller (PIC)", "",
	0x08, 0x00, 0x01, "ISA Programmable Interrupt Controller (PIC)", "",
	0x08, 0x00, 0x02, "EISA Programmable Interrupt Controller (PIC)", "",
	0x08, 0x00, 0x10, "I/O APIC interrupt controller", "",
	0x08, 0x00, 0x20, "I/O(x) APIC interrupt controller", "",
	0x08, 0x01, 0x00, "Generic 8237 DMA controller", "",
	0x08, 0x01, 0x01, "ISA DMA controller", "",
	0x08, 0x01, 0x02, "EISA DMA controller", "",
	0x08, 0x02, 0x00, "Generic 8254 system timer", "",
	0x08, 0x02, 0x01, "ISA system timer", "",
	0x08, 0x02, 0x02, "EISA system timers", "",
	0x08, 0x02, 0x03, "High Performance Event Timer (HPET)", "",
	0x08, 0x03, 0x00, "Generic Real-Time Clock (RTC) controller", "",
	0x08, 0x03, 0x01, "ISA Real-Time Clock (RTC) controller", "",
	0x08, 0x04, 0x00, "Generic PCI Hot-Plug controller", "",
	0x08, 0x05, 0x00, "SD host controller", "",
	0x08, 0x06, 0x00, "IOMMU controller", "",
	0x08, 0x07, 0x00, "Root Complex Event Collector", "",
	0x08, 0x80, 0x00, "Other system peripheral", "",

	0x09, 0x00, 0x00, "Keyboard controller", "",
	0x09, 0x01, 0x00, "Digitizer (pen)", "",
	0x09, 0x02, 0x00, "Mouse controller", "",
	0x09, 0x03, 0x00, "Scanner controller", "",
	0x09, 0x04, 0x00, "Gameport controller (generic)", "",
	0x09, 0x04, 0x10, "Gameport controller (extended)", "",
	0x09, 0x80, 0x00, "Other input device controller", "",

	0x0A, 0x00, 0x00, "Generic docking station", "",
	0x0A, 0x80, 0x00, "Other type of docking station", "",

	0x0B, 0x00, 0x00, "386 processor", "",
	0x0B, 0x01, 0x00, "486 processor", "",
	0x0B, 0x02, 0x00, "Pentium processor", "",
	0x0B, 0x10, 0x00, "Alpha processor", "",
	0x0B, 0x20, 0x00, "PowerPC processor", "",
	0x0B, 0x30, 0x00, "MIPS processor", "",
	0x0B, 0x40, 0x00, "Co-processor", "",
	0x0B, 0x80, 0x00, "Other processor", "",

	0x0C, 0x00, 0x00, "IEEE 1394 (FireWire) bus controller ", "(generic)",
	0x0C, 0x00, 0x10, "IEEE 1394 (FireWire) bus controller ", "(OpenHCI specification)",
	0x0C, 0x01, 0x00, "ACCESS.bus controller", "",
	0x0C, 0x02, 0x00, "Serial Storage Architecture (SSA) controller", "",
	0x0C, 0x03, 0x00, "Universal Serial Bus (USB) controller ", "(UHCI)",
	0x0C, 0x03, 0x10, "Universal Serial Bus (USB) controller ", "(OHCI)",
	0x0C, 0x03, 0x20, "Universal Serial Bus (USB) controller ", "(EHCI)",
	0x0C, 0x03, 0x30, "Universal Serial Bus (USB) controller ", "(xHCI)",
	0x0C, 0x03, 0x80, "Universal Serial Bus (USB) controller ", "(unspecified programming interface)",
	0x0C, 0x03, 0xFE, "Universal Serial Bus (USB) device", "",
	0x0C, 0x04, 0x00, "Fibre Channel controller", "",
	0x0C, 0x05, 0x00, "System Management Bus (SMBus) controller", "",
	0x0C, 0x06, 0x00, "InfiniBand bus controller", "",
	0x0C, 0x07, 0x00, "IPMI (Intelligent Platform Management Interface) SMIC (Server Management Interface Chip) interface", "",
	0x0C, 0x07, 0x01, "IPMI (Intelligent Platform Management Interface) Keyboard Controller Style interface", "",
	0x0C, 0x07, 0x02, "IPMI (Intelligent Platform Management Interface) Block Transfer interface", "",
	0x0C, 0x08, 0x00, "SERCOS (Serial Real-Time Communication System) interface (IEC 61491)", "",
	0x0C, 0x09, 0x00, "CANbus controller", "",
	0x0C, 0x0A, 0x00, "MIPI I3C Host Controller Interface", "",
	0x0C, 0x80, 0x00, "Other serial bus controller", "",

	0x0D, 0x00, 0x00, "IrDA compatible controller", "",
	0x0D, 0x01, 0x00, "Consumer IR controller", "",
	0x0D, 0x01, 0x10, "UWB radio controller", "",
	0x0D, 0x10, 0x00, "RF controller", "",
	0x0D, 0x11, 0x00, "Bluetooth controller", "",
	0x0D, 0x12, 0x00, "Broadband controller", "",
	0x0D, 0x20, 0x00, "Ethernet controller (802.1a - 5 GHz)", "",
	0x0D, 0x21, 0x00, "Ethernet controller (802.1b - 2.4 GHz)", "",
	0x0D, 0x40, 0x00, "Cellular controller/modem", "",
	0x0D, 0x41, 0x00, "Cellular controller/modem plus Ethernet (802.11)", "",
	0x0D, 0x80, 0x00, "Other type of wireless controller", "",

	0x0E, 0x00, 0x00, "I2O intelligent I/O controller", "",
	0x0E, 0x80, 0x00, "Intelligent I/O controller", "",

	0x0F, 0x01, 0x00, "Satellite TV controller", "",
	0x0F, 0x02, 0x00, "Satellite audio communication controller", "",
	0x0F, 0x03, 0x00, "Satellite voice communication controller", "",
	0x0F, 0x04, 0x00, "Satellite data communication controller", "",
	0x0F, 0x80, 0x00, "Other satellite communication controller", "",

	0x10, 0x00, 0x00, "Network and computing encryption and decryption controller", "",
	0x10, 0x10, 0x00, "Entertainment encryption and decryption controller", "",
	0x10, 0x80, 0x00, "Other encryption and decryption controller", "",

	0x11, 0x00, 0x00, "Digital Parallel In Out (DPIO) module", "",
	0x11, 0x01, 0x00, "Performance counters", "",
	0x11, 0x10, 0x00, "Communications synchronization plus time and frequency test/measurement controller", "",
	0x11, 0x20, 0x00, "Data acquisition/signal processing management controller", "",
	0x11, 0x80, 0x00, "Other data acquisition/signal processing controller", "",

	0x12, 0x00, 0x00, "Processing accelerator ", "(vendor-specific interface)",
	0x12, 0x01, 0x00, "AI interface accelerator", "",

	0x12, 0x01, 0x00, "Non-essential instrumentation function ", "(vendor-specific interface)",

	0xFF, 0x00, 0x00, "Device vith unassigned class/sub-class code", "",
};

// Number of table entries; can be used for searching in the table
#define	PCI_CLASS_TABLE_LEN	(sizeof(PciClassTable) / sizeof(PCI_CLASS_TABLE)) 

#endif
