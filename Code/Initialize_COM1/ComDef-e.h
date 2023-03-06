//--------------------------------------------------------------------
// TUCN, Computer Science Department
// Input/Output Systems and Peripheral Devices
//--------------------------------------------------------------------
// http://users.utcluj.ro/~baruch/en/pages/teaching/inputoutput-systems/laboratory.php
//--------------------------------------------------------------------
// File:		ComDef-e.h
// Changed:		21.02.2021
//--------------------------------------------------------------------
// Definitions for the serial port
//--------------------------------------------------------------------

#ifndef ComDef_h
#define ComDef_h

//--------------------------------------------------------------------
// Base addresses for the serial ports
//--------------------------------------------------------------------

#define	COM1				0x3F8
#define	COM2				0x2F8

//--------------------------------------------------------------------
// Register offsets
//--------------------------------------------------------------------

#define	THR					0		// Transmitter Holding Register
#define	RBR					0		// Receiver Buffer Register
#define	DLR_LSB				0		// Divisor Latch Register LSB
#define	IER					1		// Interrupt Enable Register
#define	DLR_MSB				1		// Divisor Latch Register MSB
#define	IIR					2		// Interrupt Identification Register
#define	FCR					2		// FIFO Control Register
#define	LCR					3		// Line Control Register
#define	MCR					4		// Modem Control Register
#define	LSR					5		// Line Status Register
#define	MSR					6		// Modem Status Register

//--------------------------------------------------------------------
// Register addresses
//--------------------------------------------------------------------

// Registers for COM1 port

#define	COM1_THR			0x3F8	// Transmitter Holding Register
#define	COM1_RBR			0x3F8	// Receiver Buffer Register
#define	COM1_DLR_LSB		0x3F8	// Divisor Latch Register LSB
#define	COM1_IER			0x3F9	// Interrupt Enable Register
#define	COM1_DLR_MSB		0x3F9	// Divisor Latch Register MSB
#define	COM1_IIR			0x3FA	// Interrupt Identification Register
#define	COM1_FCR			0x3FA	// FIFO Control Register
#define	COM1_LCR			0x3FB	// Line Control Register
#define	COM1_MCR			0x3FC	// Modem Control Register
#define	COM1_LSR			0x3FD	// Line Status Register
#define	COM1_MSR			0x3FE	// Modem Status Register

// Registers for COM2 port

#define	COM2_THR			0x2F8	// Transmitter Holding Register
#define	COM2_RBR			0x2F8	// Receiver Buffer Register
#define	COM2_DLR_LSB		0x2F8	// Divisor Latch Register LSB
#define	COM2_IER			0x2F9	// Interrupt Enable Register
#define	COM2_DLR_MSB		0x2F9	// Divisor Latch Register MSB
#define	COM2_IIR			0x2FA	// Interrupt Identification Register
#define	COM2_FCR			0x2FA	// FIFO Control Register
#define	COM2_LCR			0x2FB	// Line Control Register
#define	COM2_MCR			0x2FC	// Modem Control Register
#define	COM2_LSR			0x2FD	// Line Status Register
#define	COM2_MSR			0x2FE	// Modem Status Register

//--------------------------------------------------------------------
// Masks for the IER register
//--------------------------------------------------------------------

#define	IER_RDA_INT			0x01	// Enable Received Data Available Interrupt
#define	IER_THRE_INT		0x02	// Enable THR Empty Interrupt
#define	IER_RLS_INT			0x04	// Enable Receiver Line Status Interrupt
#define	IER_MS_INT			0x08	// Enable Modem Status Interrupt

//--------------------------------------------------------------------
// Masks for the FCR register
//--------------------------------------------------------------------

#define	FCR_FIFO_EN			0x01	// FIFO Enable
#define	FCR_RXFIFO_RST		0x02	// Receiver FIFO Reset
#define	FCR_TXFIFO_RST		0x04	// Transmitter FIFO Reset
#define	FCR_DMA_MODE		0x08	// DMA Mode Select
#define	FCR_RX_TRIGGER		0xC0	// Receiver Trigger

//--------------------------------------------------------------------
// Masks for the LCR register
//--------------------------------------------------------------------

#define	LCR_WORD_LEN		0x03	// Word Length Select
#define	LCR_NO_STOP			0x04	// Number of Stop Bits
#define	LCR_PARITY_EN		0x08	// Parity Enable
#define	LCR_EVEN_PARITY		0x10	// Even Parity Select
#define	LCR_STICK_PARITY	0x20	// Stick Parity
#define	LCR_SET_BREAK		0x40	// Set Break
#define	LCR_DLAB			0x80	// Divisor Latch Access Bit

//--------------------------------------------------------------------
// Masks for the MCR register
//--------------------------------------------------------------------

#define	MCR_DTR				0x01	// Data Terminal Ready
#define	MCR_RTS				0x02	// Request To Send
#define	MCR_OUT1			0x04	// OUT1
#define	MCR_OUT2			0x08	// OUT2
#define	MCR_LOOP			0x10	// Loop

//--------------------------------------------------------------------
// Masks for the LSR register
//--------------------------------------------------------------------

#define	LSR_DATA_READY		0x01	// Data Ready
#define	LSR_OVERRUN_ERR		0x02	// Overrun Error
#define	LSR_PARITY_ERR		0x04	// Parity Error
#define	LSR_FRAMING_ERR		0x08	// Framing Error
#define	LSR_BREAK_INT		0x10	// Break Interrupt
#define	LSR_THR_EMPTY		0x20	// Transmitter Holding Register Empty
#define	LSR_TSR_EMPTY		0x40	// Transmitter Shift Register Empty
#define	LSR_ERR_RXFIFO		0x80	// Error in Receiver FIFO

//--------------------------------------------------------------------
// Masks for the MSR register
//--------------------------------------------------------------------

#define	MSR_DELTA_CTS		0x01	// Delta Clear To Send
#define	MSR_DELTA_DSR		0x02	// Delta Data Set Ready
#define	MSR_TRAIL_EDGE_RI	0x04	// Trailing Edge Ring Indicator
#define	MSR_DELTA_CD		0x08	// Delta Carrier Detect
#define	MSR_CTS				0x10	// Clear To Send
#define	MSR_DSR				0x20	// Data Set Ready
#define	MSR_RI				0x40	// Ring Indicator
#define	MSR_CD				0x80	// Carrier Detect

#endif
