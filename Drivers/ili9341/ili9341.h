/*
 * ili9341.h
 *
 *  Created on: Jul 18, 2024
 *      Author: eyalk
 *
 *
 * This driver assumes using the ILI9341 display in "4-wire 8-bit data serial interface 2" data mode.
 * This means IM[3:0] = 0b1110 ( physically pulled ).
 * This also mean using Full-Duplex SPI mode.
 * A total of 6 pins are required:
 * 		 ----------------------------------------------------------------
 * 		|   SPI  |  PIN   |	Description 							 	 |
 * 		|----------------------------------------------------------------|
 * 		|        |  PA15  |    D/CX(DCX)                                 |
 * 		|        |-------------------------------------------------------|
 * 		|        |  PA15  |    CSX                                       |
 * 		| SPI1   |-------------------------------------------------------|
 * 		|        |  PA15  |    SCL                                       |
 * 		|        |-------------------------------------------------------|
 * 		|        |  PA15  |    SDO(MOSI)                                 |
 * 		|        |-------------------------------------------------------|
 * 		|        |  PA15  |    SDI(MISO)                                 |
 * 		|        |-------------------------------------------------------|
 * 		|        |  PA15  |    RST                                       |
 * 		|----------------------------------------------------------------|
 *
 * 		* Color depths options supported:
 * 			* 65k colors, RGB 5,6,5 - bits input
 * 			* 262k colors, RGB 6,6,6 - bits input
 */


#ifndef ILI9341_ILI9341_H_
#define ILI9341_ILI9341_H_

#ifdef STM32F746_NUCLEO
	#include "stm32f746xx.h"
#endif

/*
 * Command List
 * Note: Sending commands to the display requires D/CX -> LOW
 */

/*
 * Level 1 Commands
 */

// After a software-reset, 5ms delay must follow.
// If software-reset is applied during SLEEP-OUT mode, 120ms delay must follow.
#define ILI9341_CMD_SOFTWARE_RESET		0x01

// Read Display Identification Information
#define ILI9341_RDDIDIF					0x04

// Read Display Status
#define ILI9341_RDDST					0x09

// Read Display Power Mode
#define ILI9341_RDDPM					0x0A

// Read Display MADCTL
#define ILI9341_RDDMADCTL				0x0B

// Read Display Pixel Format
#define ILI9341_RDDCOLMOD				0x0C

// Read Display Image Mode
#define ILI9341_RDDIM					0x0D

// Read Display Signal Mode
#define ILI9341_RDDSM					0x0E

// Read Display Self-Diagnostic Result
#define ILI9341_RDDSDR					0x0F

// Enter Sleep Mode
#define ILI9341_SPLIN					0x10

// Sleep Out(Turn off sleep mode)
#define ILI9341_SLPOUT					0x11

// Partial Mode On
#define ILI9341_PTLON					0x12

// Normal Display Mode ON
#define ILI9341_NORON					0x13

// Display Inversion OFF
#define ILI9341_DINVOFF					0x20

// Display Inversion ON
#define ILI9341_DINVON					0x21

// Gamma Set
#define ILI9341_GAMSET					0x26

// Display OFF
#define ILI9341_DISPOFF					0x28

// Display ON
#define ILI9341_DISPON					0x29

// Column Address Set
#define ILI9341_CASET					0x2A

// Page Address Set
#define ILI9341_PASET					0x2B

// Memory Write
#define ILI9341_RAMWR					0x2C

// Color Set
#define ILI9341_RGBSET					0x2D

// Memory Read
#define ILI9341_RAMRD					0x2E

// Partial Area
#define ILI9341_PLTAR					0x30

// Vertical Scrolling Definition
#define ILI9341_VSCRDEF					0x33

// Tearing Effect Line OFF
#define ILI9341_TEOFF					0x34

// Tearing Effect Line ON
#define ILI9341_TEON					0x35

// Memory Access Control
#define ILI9341_MADCTL					0x36

// Vertical Scrolling Start Address
#define ILI9341_VSCRSADD				0x37

// IDLE Mode OFF
#define ILI9341_IDMOFF					0x38

// IDLE Mode On
#define ILI9341_IDMON					0x39

// COLMOD: Pixel Format Set
#define ILI9341_PIXSET					0x3A

// Write Memory Continue
#define ILI9341_WRITE_MEMORY_CONTINUE	0x3C

// Read Memory Continue
#define ILI9341_READ_MEMORY_CONTINUE	0x3E

// Set Tear Scanline
#define ILI9341_SET_TEAR_SCANLINE		0x44

// Get Scanline
#define ILI9341_GET_SCANLINE			0x45

// Write Display Brightness
#define ILI9341_WRDISBV					0x51

// Read Display Brightness
#define ILI9341_RDDISBV					0x52

// Write CTRL Display
#define ILI9341_WRCTRLD					0x53

// Read CTRL Display
#define ILI9341_RDCTRLD					0x54

// Write Content Adaptive Brightness Control
#define ILI9341_WRCABC					0x55

// Read Content Adaptive Brightness Control
#define ILI9341_RDCABC					0x56

// Write CABC Minimum Brightness
#define ILI9341_WBACKLIGHT_CONTROL1 	0x5E

// Read CABC Minimum Brightness
#define ILI9341_RBACKLIGHT_CONTROL1		0x5F

// Read ID1
#define ILI9341_RDID1					0xDA

// Read ID2
#define ILI9341_RDID2					0xDB

// Read ID3
#define ILI9341_RDID3					0xDC


/*
 * Level 2 Commands
 */

// RGB Interface Signal Control
#define ILI9341_IFMODE					0xB0

// Frame Rate Control(In Normal Mode/Full Colors)
#define ILI9341_FRMCTR1					0xB1

// Frame Rate Control(In Idle Mode/8 colors)
#define ILI9341_FRMCTR2					0xB2

// Frame Rate Control(In Partial Mode/Full Colors)
#define ILI9341_FRMCTR3					0xB3

// Display Inversion Control
#define ILI9341_INVTR					0xB4

// Blanking Porch Control
#define ILI9341_PRCTR					0xB5

// Display Function Control
#define ILI9341_DISCTRL					0xB6

// Entry Mode Set
#define ILI9341_ETMOD					0xB7

// Backlight Control1
#define ILI9341_BACKLIGHT_CONTROL1		0xB8

// Backlight Control2
#define ILI9341_BACKLIGHT_CONTROL2		0xB9

// Backlight Control3
#define ILI9341_BACKLIGHT_CONTROL3		0xBA

// Backlight Control4
#define ILI9341_BACKLIGHT_CONTROL4		0xBB

// Backlight Control5
#define ILI9341_BACKLIGHT_CONTROL5		0xBC

// Backlight Control7
#define ILI9341_BACKLIGHT_CONTROL7		0xBE

// Backlight Control8
#define ILI9341_BACKLIGHT_CONTROL8		0xBF

// Power Control
#define ILI9341_POWER_CONTROL1			0xC0

// Power Control2
#define ILI9341_POWER_CONTROL2			0xC1

// VCOM Control1
#define ILI9341_VMCTRL1					0xC5

// VCOM Control2
#define ILI9341_VMCTRL2					0xC7

// NV Memory Write
#define ILI9341_NVMWR					0xD0

// NV Memory Protection Key
#define ILI9341_NVMPKEY					0xD1

// NV Memory Status Read
#define ILI9341_RDNVM					0xD2

// Read ID4
#define ILI9341_RDID4					0xD3

// Positive Gamma Correction
#define ILI9341_PGAMCTRL				0xE0

// Negative Gamma Correction
#define ILI9341_NGAMCTRL				0xE1

// Digital Gamma Control1
#define ILI9341_DGAMCTRL1				0xE2

// Digital Gamma Control2
#define ILI9341_DGAMCTRL2				0xE3

// Interface Control
#define ILI9341_IFCTL					0xF6

/*
 * Description of Extend Register Command
 */

// Power Control A
#define ILI9341_POWER_CONTROLA 			0xCB

// Power Control B
#define ILI9341_POWER_CONTROLB			0xCF

// Driver Timing Control 1
#define ILI9341_DRIVING_TIM_CTRLA		0xE8

// Driver Timing Control 2
#define ILI9341_DRIVER_TIM_CTRLB		0xEA

// Power On Sequence Control
#define ILI9341_POWER_ON_SEQ_CTRL		0xED

// Enable 3G
#define ILI9341_ENABLE_3G				0xF2

// Pump Ratio Control
#define ILI9341_PUMP_RATIO_CTRL			0xF7









#endif /* ILI9341_ILI9341_H_ */
