/*
 * SSD1306.h
 *
 *  Created on: 18 set 2024
 *      
 */

#include "SSD1306.h"

uint8_t OledLineNum, OledCursorPos;

#define FONT_SIZE 5

const unsigned char OledFontTable[][FONT_SIZE] = {

    0x00, 0x00, 0x00, 0x00, 0x00,   // space
    0x00, 0x00, 0x2f, 0x00, 0x00,   // !
    0x00, 0x07, 0x00, 0x07, 0x00,   // "
    0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
    0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
    0x23, 0x13, 0x08, 0x64, 0x62,   // %
    0x36, 0x49, 0x55, 0x22, 0x50,   // &
    0x00, 0x05, 0x03, 0x00, 0x00,   // '
    0x00, 0x1c, 0x22, 0x41, 0x00,   // (
    0x00, 0x41, 0x22, 0x1c, 0x00,   // )
    0x14, 0x08, 0x3E, 0x08, 0x14,   // *
    0x08, 0x08, 0x3E, 0x08, 0x08,   // +
    0x00, 0x00, 0xA0, 0x60, 0x00,   // ,
    0x08, 0x08, 0x08, 0x08, 0x08,   // -
    0x00, 0x60, 0x60, 0x00, 0x00,   // .
    0x20, 0x10, 0x08, 0x04, 0x02,   // /

    0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
    0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
    0x42, 0x61, 0x51, 0x49, 0x46,   // 2
    0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
    0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
    0x27, 0x45, 0x45, 0x45, 0x39,   // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
    0x01, 0x71, 0x09, 0x05, 0x03,   // 7
    0x36, 0x49, 0x49, 0x49, 0x36,   // 8
    0x06, 0x49, 0x49, 0x29, 0x1E,   // 9

    0x00, 0x36, 0x36, 0x00, 0x00,   // :
    0x00, 0x56, 0x36, 0x00, 0x00,   // ;
    0x08, 0x14, 0x22, 0x41, 0x00,   // <
    0x14, 0x14, 0x14, 0x14, 0x14,   // =
    0x00, 0x41, 0x22, 0x14, 0x08,   // >
    0x02, 0x01, 0x51, 0x09, 0x06,   // ?
    0x32, 0x49, 0x59, 0x51, 0x3E,   // @

    0x7C, 0x12, 0x11, 0x12, 0x7C,   // A
    0x7F, 0x49, 0x49, 0x49, 0x36,   // B
    0x3E, 0x41, 0x41, 0x41, 0x22,   // C
    0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
    0x7F, 0x49, 0x49, 0x49, 0x41,   // E
    0x7F, 0x09, 0x09, 0x09, 0x01,   // F
    0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
    0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
    0x00, 0x41, 0x7F, 0x41, 0x00,   // I
    0x20, 0x40, 0x41, 0x3F, 0x01,   // J
    0x7F, 0x08, 0x14, 0x22, 0x41,   // K
    0x7F, 0x40, 0x40, 0x40, 0x40,   // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
    0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
    0x3E, 0x41, 0x41, 0x41, 0x3E,   // O
    0x7F, 0x09, 0x09, 0x09, 0x06,   // P
    0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
    0x7F, 0x09, 0x19, 0x29, 0x46,   // R
    0x46, 0x49, 0x49, 0x49, 0x31,   // S
    0x01, 0x01, 0x7F, 0x01, 0x01,   // T
    0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
    0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
    0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
    0x63, 0x14, 0x08, 0x14, 0x63,   // X
    0x07, 0x08, 0x70, 0x08, 0x07,   // Y
    0x61, 0x51, 0x49, 0x45, 0x43,   // Z

    0x00, 0x7F, 0x41, 0x41, 0x00,   // [
    0x55, 0xAA, 0x55, 0xAA, 0x55,   // Backslash (Checker pattern)
    0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
    0x04, 0x02, 0x01, 0x02, 0x04,   // ^
    0x40, 0x40, 0x40, 0x40, 0x40,   // _
    0x00, 0x03, 0x05, 0x00, 0x00,   // `

    0x20, 0x54, 0x54, 0x54, 0x78,   // a
    0x7F, 0x48, 0x44, 0x44, 0x38,   // b
    0x38, 0x44, 0x44, 0x44, 0x20,   // c
    0x38, 0x44, 0x44, 0x48, 0x7F,   // d
    0x38, 0x54, 0x54, 0x54, 0x18,   // e
    0x08, 0x7E, 0x09, 0x01, 0x02,   // f
    0x18, 0xA4, 0xA4, 0xA4, 0x7C,   // g
    0x7F, 0x08, 0x04, 0x04, 0x78,   // h
    0x00, 0x44, 0x7D, 0x40, 0x00,   // i
    0x40, 0x80, 0x84, 0x7D, 0x00,   // j
    0x7F, 0x10, 0x28, 0x44, 0x00,   // k
    0x00, 0x41, 0x7F, 0x40, 0x00,   // l
    0x7C, 0x04, 0x18, 0x04, 0x78,   // m
    0x7C, 0x08, 0x04, 0x04, 0x78,   // n
    0x38, 0x44, 0x44, 0x44, 0x38,   // o
    0xFC, 0x24, 0x24, 0x24, 0x18,   // p
    0x18, 0x24, 0x24, 0x18, 0xFC,   // q
    0x7C, 0x08, 0x04, 0x04, 0x08,   // r
    0x48, 0x54, 0x54, 0x54, 0x20,   // s
    0x04, 0x3F, 0x44, 0x40, 0x20,   // t
    0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
    0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
    0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
    0x44, 0x28, 0x10, 0x28, 0x44,   // x
    0x1C, 0xA0, 0xA0, 0xA0, 0x7C,   // y
    0x44, 0x64, 0x54, 0x4C, 0x44,   // z

    0x00, 0x10, 0x7C, 0x82, 0x00,   // {
    0x00, 0x00, 0xFF, 0x00, 0x00,   // |
    0x00, 0x82, 0x7C, 0x10, 0x00,   // }
    0x00, 0x06, 0x09, 0x09, 0x06    // ~ (Degrees)
};

SSD1306::SSD1306(u16 IIC_DEVICE_ID, u16 SCLK_RATE, u8 IIC_ADDR) {
  i2c_dev = new I2CDevice(IIC_DEVICE_ID, SCLK_RATE, IIC_ADDR);
}

bool SSD1306::OLED_Init(void)
{
	i2c_dev->begin();

	SendByteCommand(SSD1306_DISPLAY_OFF);
	Send2ByteCommand(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO, 0x80);
	Send2ByteCommand(SSD1306_SET_MULTIPLEX_RATIO, 0x3F);
	Send2ByteCommand(SSD1306_SET_DISPLAY_OFFSET, 0x0);
	SendByteCommand(SSD1306_SET_START_LINE | 0x0);

	Send2ByteCommand(SSD1306_CHARGE_PUMP, 0x14);
	Send2ByteCommand(SSD1306_MEMORY_ADDR_MODE, 0x00);

	SendByteCommand(SSD1306_SET_SEGMENT_REMAP | 0x1);
	SendByteCommand(SSD1306_COM_SCAN_DIR_DEC);
	Send2ByteCommand(SSD1306_SET_COM_PINS, 0x12);
	Send2ByteCommand(SSD1306_SET_CONTRAST_CONTROL, 0xCF);

	Send2ByteCommand(SSD1306_SET_PRECHARGE_PERIOD, 0xF1);
	Send2ByteCommand(SSD1306_SET_VCOM_DESELECT, 0x40);

	SendByteCommand(SSD1306_DISPLAY_ALL_ON_RESUME);
	SendByteCommand(SSD1306_NORMAL_DISPLAY);
	SendByteCommand(SSD1306_DISPLAY_ON);

	OLED_Clear();  

	xil_printf("Display Cleared");
	return true;
}

void SSD1306::OLED_Clear()
{
    Send3ByteCommand(SSD1306_SET_COLUMN_ADDR, 0x00, 0x7F);
    Send3ByteCommand(SSD1306_SET_PAGE_ADDR, 0x00, 0x07);

    SendClearCommand();

    Send3ByteCommand(SSD1306_SET_COLUMN_ADDR, 0x00, 0x7F);
	Send3ByteCommand(SSD1306_SET_PAGE_ADDR, 0x00, 0x07);

}

void SSD1306::OLED_DisplayChar(uint8_t ch)
{
    uint8_t dat,i=0;

    if(((OledCursorPos+FONT_SIZE)>=128) || (ch=='\n'))
    {
        
        OLED_GoToNextLine();
    }
    if(ch!='\n') 
    {
        ch = ch-0x20; 

        while(1)
        {
            dat= OledFontTable[ch][i]; 


            SendByteData(dat); 
            OledCursorPos++;

            i++;

            if(i==FONT_SIZE) 
            {
                SendByteData(0x00); 
                OledCursorPos++;
                break;
            }
        }
    }
}

#if(Enable_OLED_DisplayString==1)
void SSD1306::OLED_DisplayString(uint8_t *ptr)
{
    while(*ptr)
        OLED_DisplayChar(*ptr++);
}
#endif

#if ((Enable_OLED_DisplayNumber == 1) || (Enable_OLED_DisplayFloatNumber == 1) || (Enable_OLED_Printf==1))
void SSD1306::OLED_DisplayNumber(uint8_t v_numericSystem_u8, uint32_t v_number_u32, uint8_t v_numOfDigitsToDisplay_u8)
{
    uint8_t i=0,a[10];

    if(C_BINARY_U8 == v_numericSystem_u8)
    {
        while(v_numOfDigitsToDisplay_u8!=0)
        {
            
            i = util_GetBitStatus(v_number_u32,(v_numOfDigitsToDisplay_u8-1));
            OLED_DisplayChar(util_Dec2Ascii(i));
            v_numOfDigitsToDisplay_u8--;
        }
    }
    else if(v_number_u32==0)
    {
        
        for(i=0;((i<v_numOfDigitsToDisplay_u8) && (i<C_MaxDigitsToDisplay_U8));i++)
            OLED_DisplayChar('0');
    }
    else
    {
        for(i=0;i<v_numOfDigitsToDisplay_u8;i++)
        {
            
            if(v_number_u32!=0)
            {
                
                a[i]=util_GetMod32(v_number_u32,v_numericSystem_u8);
                v_number_u32=v_number_u32/v_numericSystem_u8;
            }
            else if( (v_numOfDigitsToDisplay_u8 == C_DisplayDefaultDigits_U8) ||
                    (v_numOfDigitsToDisplay_u8 > C_MaxDigitsToDisplay_U8))
            {
                
                break;
            }
            else
            {
                
                a[i]=0;
            }
        }

        while(i!=0)
        {
            
            OLED_DisplayChar(util_Hex2Ascii(a[i-1]));
            i--;
        }
    }
}
#endif


#if ( Enable_OLED_Printf == 1 )
void SSD1306::OLED_Printf(const char *argList, ...)
{
    const char *ptr;
    va_list argp;
    sint16_t v_num_s16;
    sint32_t v_num_s32;
    uint16_t v_num_u16;
    uint32_t v_num_u32;
    char *str;
    char  ch;
    uint8_t v_numOfDigitsToDisp_u8;
#if (Enable_OLED_DisplayFloatNumber == 1)
    double v_floatNum_f32;
#endif

    va_start(argp, argList);

    
    for(ptr = argList; *ptr != '\0'; ptr++)
    {

        ch= *ptr;
        if(ch == '%')         
        {
            ptr++;
            ch = *ptr;
            if((ch>=0x30) && (ch<=0x39))
            {
                v_numOfDigitsToDisp_u8 = 0;
                while((ch>=0x30) && (ch<=0x39))
                {
                    v_numOfDigitsToDisp_u8 = (v_numOfDigitsToDisp_u8 * 10) + (ch-0x30);
                    ptr++;
                    ch = *ptr;
                }
            }
            else
            {
                v_numOfDigitsToDisp_u8 = C_MaxDigitsToDisplayUsingPrintf_U8;
            }


            switch(ch)       
            {
                case 'C':
                case 'c':     
                    ch = va_arg(argp, int);
                    OLED_DisplayChar(ch);
                    break;

                case 'd':    
                    v_num_s16 = va_arg(argp, int);
                    if(v_num_s16<0)
                    { 
                        v_num_s16 = -v_num_s16;
                        OLED_DisplayChar('-');
                    }
                    OLED_DisplayNumber(C_DECIMAL_U8,v_num_s16,v_numOfDigitsToDisp_u8);
                    break;

                case 'D':    
                    v_num_s32 = va_arg(argp, sint32_t);
                    if(v_num_s32<0)
                    { 
                        v_num_s32 = -v_num_s32;
                        OLED_DisplayChar('-');
                    }
                    OLED_DisplayNumber(C_DECIMAL_U8,v_num_s32,v_numOfDigitsToDisp_u8);
                    break;

                case 'u':    
                    v_num_u16 = va_arg(argp, int);
                    OLED_DisplayNumber(C_DECIMAL_U8,v_num_u16,v_numOfDigitsToDisp_u8);
                    break;

                case 'U':    
                    v_num_u32 = va_arg(argp, uint32_t);
                    OLED_DisplayNumber(C_DECIMAL_U8,v_num_u32,v_numOfDigitsToDisp_u8);
                    break;

                case 'x':  
                    v_num_u16 = va_arg(argp, int);
                    OLED_DisplayNumber(C_HEX_U8,v_num_u16,v_numOfDigitsToDisp_u8);
                    break;

                case 'X':  
                    v_num_u32 = va_arg(argp, uint32_t);
                    OLED_DisplayNumber(C_HEX_U8,v_num_u32,v_numOfDigitsToDisp_u8);
                    break;


                case 'b':  
                    v_num_u16 = va_arg(argp, int);
                    if(v_numOfDigitsToDisp_u8 == C_MaxDigitsToDisplayUsingPrintf_U8)
                        v_numOfDigitsToDisp_u8 = 16;
                    OLED_DisplayNumber(C_BINARY_U8,v_num_u16,v_numOfDigitsToDisp_u8);
                    break;

                case 'B':  
                    v_num_u32 = va_arg(argp, uint32_t);
                    if(v_numOfDigitsToDisp_u8 == C_MaxDigitsToDisplayUsingPrintf_U8)
                        v_numOfDigitsToDisp_u8 = 16;
                    OLED_DisplayNumber(C_BINARY_U8,v_num_u32,v_numOfDigitsToDisp_u8);
                    break;


                case 'F':
                case 'f': 
#if (Enable_OLED_DisplayFloatNumber == 1)
                    v_floatNum_f32 = va_arg(argp, double);
                    OLED_DisplayFloatNumber(v_floatNum_f32);
#endif
                    break;


                case 'S':
                case 's': 
                    str = va_arg(argp, char *);
                    OLED_DisplayString((uint8_t *)str);
                    break;

                case '%':
                    OLED_DisplayChar('%');
                    break;
            }
        }
        else
        {
            
            OLED_DisplayChar(ch);
        }
    }

    va_end(argp);
}
#endif


#if (Enable_OLED_DisplayFloatNumber == 1)
void SSD1306::OLED_DisplayFloatNumber(double v_floatNum_f32)
{
    uint32_t v_temp_u32;
    

    v_temp_u32 = (uint32_t) v_floatNum_f32;
    OLED_DisplayNumber(C_DECIMAL_U8,v_temp_u32,C_DisplayDefaultDigits_U8);

    OLED_DisplayChar('.');

    v_floatNum_f32 = v_floatNum_f32 - v_temp_u32;
    v_temp_u32 = v_floatNum_f32 * 1000000;
    OLED_DisplayNumber(C_DECIMAL_U8,v_temp_u32,C_DisplayDefaultDigits_U8);
}
#endif



#if (Enable_OLED_GoToLine == 1)
void  SSD1306::OLED_GoToLine(uint8_t lineNumber)
{
    if(lineNumber<8)
    {   
        OledLineNum = lineNumber;
        OLED_SetCursor(OledLineNum,0);
    }
}
#endif


void  SSD1306::OLED_GoToNextLine()
{
    
    OledLineNum++;
    OledLineNum = OledLineNum&0x07;
    OLED_SetCursor(OledLineNum,0); 
}


void SSD1306::OLED_SetCursor(uint8_t lineNumber,uint8_t cursorPosition)
{
    
    if((lineNumber <= C_OledLastLine_U8) && (cursorPosition <= 127))
    {
        OledLineNum=lineNumber;   
        OledCursorPos=cursorPosition; 

        Send3ByteCommand(SSD1306_SET_COLUMN_ADDR, cursorPosition, 0x7F);

        Send3ByteCommand(SSD1306_SET_PAGE_ADDR, lineNumber, 0x07);

    }
}

void SSD1306::SendByteCommand(uint8_t cmd) {
	uint8_t buffer[2] = {(uint8_t)SSD1306_COMMAND, (uint8_t)cmd};
	i2c_dev->write(buffer, 2);
	return;
}

void SSD1306::Send2ByteCommand(uint8_t cmd, uint8_t val) {
	uint8_t buffer[3] = {(uint8_t)SSD1306_COMMAND, (uint8_t)cmd, (uint8_t)val};
	i2c_dev->write(buffer, 3);
	return;
}

void SSD1306::Send3ByteCommand(uint8_t cmd, uint8_t val_1, uint8_t val_2) {
	uint8_t buffer[4] = {(uint8_t)SSD1306_COMMAND, (uint8_t)cmd, (uint8_t)val_1, (uint8_t)val_2};
	i2c_dev->write(buffer, 4);
	return;
}

void SSD1306::SendClearCommand() {
	uint8_t buffer[1026];
	int i;
	buffer[0] = (uint8_t)SSD1306_DATA_CONTINUE;
	for (i=1; i<1025; i++) {
		buffer[i] = 0;
	}
	i2c_dev->write(buffer, 1025);
}

void SSD1306::SendByteData(uint8_t val) {
	uint8_t buffer[2] = {(uint8_t)SSD1306_DATA_CONTINUE, (uint8_t)val};
	i2c_dev->write(buffer, 2);
	return;
}



