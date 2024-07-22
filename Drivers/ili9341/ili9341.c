/*
 * ili9341.c
 *
 *  Created on: Jul 19, 2024
 *      Author: eyalk
 */

#include "ili9341.h"

volatile uint16_t LCD_HEIGHT = 240;
volatile uint16_t LCD_WIDTH	 = 320;

static const uint8_t initCmd[] = {
		0xEF, 3, 0x03, 0x80, 0x02,
		0xCF, 3, 0x00, 0xC1, 0x30,
		0xED, 4, 0x64, 0x03, 0x12, 0x81,
		0xE8, 3, 0x85, 0x00, 0x78,
		0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
		0xF7, 1, 0x20,
		0xEA, 2, 0x00, 0x00,
		ILI9341_POWER_CONTROL1, 1, 0x23,
		ILI9341_POWER_CONTROL2, 1, 0x10,
		ILI9341_VMCTRL1, 2, 0x3e, 0x28,
		ILI9341_VMCTRL2, 1, 0x86,
		ILI9341_MADCTL, 1, 0x48,
		ILI9341_VSCRSADD, 1, 0x00,
		ILI9341_PIXSET, 1, 0x55,
		ILI9341_FRMCTR1, 1, 0x55,
		ILI9341_DISCTRL, 3, 0x08, 0x082, 0x27,
		ILI9341_ENABLE_3G, 1, 0x00,
		ILI9341_GAMSET, 1, 0x01,
		ILI9341_PGAMCTRL, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
	    	0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
		ILI9341_NGAMCTRL, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
	    	0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
		ILI9341_SLPOUT, 0x80,
		ILI9341_DISPON, 0x80,
		0x00

};


void ILI9341_SPI_Send(unsigned char SPI_Data) {
	HAL_SPI_Transmit(hSPI_LCD, &SPI_Data, sizeof(uint8_t), LCD_MAX_DELAY);
}


void ILI9341_Write_Command(uint8_t cmd) {
	HAL_GPIO_WritePin(ILI9341_CSX_PORT, ILI9341_CSX_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ILI9341_DCX_PORT, ILI9341_DCX_PIN, GPIO_PIN_RESET);
	ILI9341_SPI_Send(cmd);
	HAL_GPIO_WritePin(ILI9341_CSX_PORT, ILI9341_CSX_PIN, GPIO_PIN_SET);
}


void ILI9341_Write_Data(uint8_t data) {
	HAL_GPIO_WritePin(ILI9341_CSX_PORT, ILI9341_CSX_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ILI9341_DCX_PORT, ILI9341_DCX_PIN, GPIO_PIN_SET);
	ILI9341_SPI_Send(cmd);
	HAL_GPIO_WritePin(ILI9341_CSX_PORT, ILI9341_CSX_PIN, GPIO_PIN_SET);
}


void ILI9341_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2) {
	ILI9341_Write_Command(ILI9341_CASET);
	ILI9341_Write_Data(X1 >> 8);
	ILI9341_Write_Data(X1);
	ILI9341_Write_Data(X2 >> 8);
	ILI9341_Write_Data(X2);

	ILI9341_Write_Command(ILI9341_PASET);
	ILI9341_Write_Data(Y1 >> 8);
	ILI9341_Write_Data(Y1);
	ILI9341_Write_Data(Y2 >> 8);
	ILI9341_Write_Data(Y2);

	ILI9341_Write_Command(ILI9341_RAMWR);
}


void ILI9341_Reset(void) {

}


void ILI9341_Set_Rotation(uint8_t rotation) {
	uint8_t screen_rotation = rotation;

	ILI9341_Write_Command(ILI9341_MADCTL);
#ifdef USE_FREERTOS
	vTaskDelay(1);
#else
	HAL_Delay(1);
#endif

	switch(screen_rotation) {
		case SCREEN_VERTICAL_1:
			ILI9341_Write_Data(0x40|0x08);
			LCD_WIDTH = 240;
			LCD_HEIGHT = 320;
			break;
		case SCREEN_HORIZONTAL_1:
			ILI9341_Write_Data(0x20|0x08);
			LCD_WIDTH = 320;
			LCD_HEIGHT = 240;
			break;
		case SCREEN_VERTICAL_1:
			ILI9341_Write_Data(0x80|0x08);
			LCD_WIDTH = 240;
			LCD_HEIGHT = 320;
			break;
		case SCREEN_VERTICAL_2:
			ILI9341_Write_Data(0x40|0x80|0x20|0x08);
			LCD_WIDTH = 320;
			LCD_HEIGHT = 240;
			break;
		default:
			break;
	}
}


void ILI9341_Enable(void) {
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
}


int ILI9341_Init(uint32_t orientation) {

}


void ILI9341_Fill_Screen(uint16_t color) {

}


void ILI9341_Draw_Color(uint16_t color) {

}


void ILI9341_Draw_Pixel(uint16_t X, uint16_t Y, uint16_t color) {

}


void ILI9341_Draw_Color_Burst(uint16_t color, uint32_t size) {

}


void ILI9341_Draw_Rectangle(uint16_t X, uint16_t Y, uint16_t width, uint16_t height, uint16_t color) {

}


void ILI9341_Draw_Horizontal_Line(uint16_t X, uint16_t Y, uint16_t width, uint16_t color) {

}


void ILI9341_Draw_Vertical_Line(uint16_t X, uint16_t Y, uint16_t height, uint16_t color) {

}


void ILI9341_Draw_Image(const char* Image_Array, uint8_t orientation) {

}
