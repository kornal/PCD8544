/*
 * PCD8544.cpp
 *
 * Copyright 2016 Marcin Kornalewski <kornalek{a\t}interia{d/o/t}pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "PCD8544.h"

PCD8544::PCD8544
(
    uint8_t & sce,
    uint8_t & rst,
    uint8_t & dc,
    uint8_t & backlight,
    SPIClass * pSPIClassObj
)

{
  pin_sce = sce;
  pin_rst = rst;
  pin_dc = dc;
  pin_backlight = backlight;
  backLightValue = 0;
  pSPI = pSPIClassObj;
}

PCD8544::~PCD8544() {
}

void PCD8544::TxRawData(uint8_t dataNotCommand, uint8_t rawData)
/*
 * This method let us transmit data or commands to LCD module.
 * method input:
 *          dataNotCommand
 *          rawData
 * where:
 *          dataNotCommand may be 0 for commands or 1 for data.
 *          rawData include exactly one byte of data (MSB goes first).
 * For more details about data and commands read PCD8544 spec.
 *
 * Before we send anything we need to have initiated following
 * pins:
 *  MOSI (DN) and
 *  SCE
 * we do that in Init() method.
 */
{
    digitalWrite(pin_dc, dataNotCommand);   //  Decide if we will tx command or data.
    digitalWrite(pin_sce, LOW);             //  Disable LCD chip on SPI interface.

    /*
     * shiftOut(pin_sdin, pin_sclk, MSBFIRST, rawData);
     *
     * Temporary we will skip line available above:
     * shiftOut
     * all because we want to try to use hardware SPI
     * mechanism provided by SPI.h
     * This one is available below.
     *
     */
    pSPI->setBitOrder(MSBFIRST);    // Setup bit order during SPI transmission.
    pSPI->transfer(rawData);        // Transmit data (one byte).

    digitalWrite(pin_sce, HIGH);    // Enable LCD chip to read data from SPI interface.
}

void PCD8544::ClearDisplayMap() {
    for (int i = 0; i < (LCD_X * LCD_Y / 8); i++) {
        displayMap[i] = 0x00;
    }
}

void PCD8544::Init() {
    pinMode(pin_sce, OUTPUT);
    pinMode(pin_rst, OUTPUT);
    pinMode(pin_dc, OUTPUT);
    pinMode(pin_backlight, OUTPUT);

    analogWrite(pin_backlight, backLightValue);

    digitalWrite(pin_rst, LOW);
    digitalWrite(pin_rst, HIGH);

    TxRawData(LCD_C, 0x21 );  // LCD Extended Commands.
    TxRawData(LCD_C, 0xB1 );  // Set LCD Vop (Contrast).
    TxRawData(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
    TxRawData(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
    TxRawData(LCD_C, 0x20 );  // LCD Basic Commands
    TxRawData(LCD_C, 0x0C );  // LCD in normal mode.

    ClearDisplayMap();
}

void PCD8544::GotoXY(uint8_t x, uint8_t y) {
    TxRawData(LCD_C, 0x80 | x); // Column.
    TxRawData(LCD_C, 0x40 | y); // Row.
}

void PCD8544::Clear() {
    GotoXY(0, 0);
    for (int i = 0; i < (LCD_X * LCD_Y / 8); i++) {
        TxRawData(LCD_D, 0x00);
    }
}

void PCD8544::PrintChar(char oneChar) {
    for (int i = 0; i < ASCII_FONT_SIZE; i++)
    /*
     * Each character includes 7 pixels in column
     * plus one additional pixel as free space between two lines/rows.
     * Following that we can read one column in one byte.
     *
     * Each character includes constant number of columns:
     * called ASCII_FONT_SIZE.
     * Following that we need to repeat reading of byte
     * for each column.
     *
     * Summary: if we want to send a char to the display
     * we need to read whole column and send it.
     * We need to repeat it for each column.
     */
    {
        TxRawData(LCD_D, pgm_read_byte(&ASCII[oneChar - 0x20][i]));
        /*
         * We need to map our char to our ASCII map.
         * Each character is shifted: 0x20.
         * pgm_read_byte() has been used to read ASCII map directly
         * from program memory (and save RAM).
         */
    }
    TxRawData(LCD_D, 0x00);
    /*
     * Finally we will send one empty column.
     * This will be a separator between current and next character.
     */
}

void PCD8544::Print(const char * chars)
/*
 * Print each char in the string.
 * method input:
 *      string (const char *)
 */
{
    while ( * chars ) {
        PrintChar(*chars++);
    }
}


void PCD8544::HelloWorld()
/*
 * Simple "Hello World" like method.
 * Print on LCD header and source file names.
 *
 */
{
    Clear();            // Clear LCD display.
    GotoXY(10, 2);      // Go to column, row (in pixels).
    Print("PCD8544.h"); // Print string on LCD.
    GotoXY(10, 3);
    Print("PCD8544.cpp");
}

void PCD8544::AddToDisplayMap(const uint8_t * content) {
    for (int i=0; i < (LCD_X * LCD_Y / 8); i++) {
        displayMap[i] |= pgm_read_byte(content + i);
        /*
            In C when we use ARRAY then we copy it in to
            the RAM.
            ATMEGA has only 2k of RAM and 32k of
            FLASH dedicated for program.

            Function (macro) pgm_read_byte() became from:
            avr/pgmspace.h
            and read pointer value
            not from RAM but from FLASH (program area)
            This is how we save RAM space.
            Be aware that content shall include PROGMEM word.
            Example:
            const unsigned char PROGMEM BG_RIGHT_ALL [] = {24, 28};
        */
    }
}

void PCD8544::RemoveFromDisplayMap(const uint8_t * content) {
    for (int i=0; i < (LCD_X * LCD_Y / 8); i++) {
        displayMap[i] &= ~(pgm_read_byte(content + i));
        /*
            In C when we use ARRAY then we copy it in to
            the RAM.
            ATMEGA has only 2k of RAM and 32k of
            FLASH dedicated for program.

            Function (macro) pgm_read_byte() became from:
            avr/pgmspace.h
            and read pointer value
            not from RAM but from FLASH (program area).
            This is how we save RAM space.
            Be aware that content shall include PROGMEM word.
            Example:
            const unsigned char PROGMEM BG_RIGHT_ALL [] = {24, 28};
        */
    }
}

void PCD8544::PrintCharOnDisplayMap(
                                      char oneChar,
                                      uint8_t & iColumnX,
                                      uint8_t & iRowY
                                      )
{
    int i=((iRowY*LCD_X)+(iColumnX*(ASCII_FONT_SIZE+1)));
    while ( i < ((iRowY*LCD_X) + (iColumnX*(ASCII_FONT_SIZE+1)+ASCII_FONT_SIZE)) )
    {
        for (int j = 0; j < ASCII_FONT_SIZE; j++)
        {
            displayMap[i] = pgm_read_byte(&ASCII[oneChar - 0x20][j]);
            i++;
        }
    }

}

void PCD8544::PrintOnDisplayMap(
                                  const char * chars,
                                  uint8_t iColumnX,
                                  uint8_t iRowY
                                  )
{
    if ( (iColumnX <= LCD_X) & (iRowY <= LCD_Y) )
    {
        while ( * chars )
        {
            PrintCharOnDisplayMap(*chars++, iColumnX, iRowY);
            iColumnX++;
        }
    }
}

void PCD8544::UpdateDisplay() {
    GotoXY(0, 0);
    for (int i=0; i < (LCD_X * LCD_Y / 8); i++) {
        TxRawData(LCD_D, displayMap[i]);
    }
}

void PCD8544::SetBackLight(uint8_t blValue) {
    backLightValue = blValue;
    analogWrite(pin_backlight, backLightValue);
}

uint8_t PCD8544::GetBackLight() {
    return backLightValue;
}
