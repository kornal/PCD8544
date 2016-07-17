/*
 * PCD8544_HelloWorld.ino
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

//! PCD8544 example #1: Hello World
/*!
 *  This is example of use PCD8544 library in Arduino sketch.
 *  This is only Hello World.
 *  It means that only printing simple string will be considered.
 */

#include <Arduino.h>

#include <SPI.h>
#include <PCD8544.h>

uint8_t LCD_BACKLIGHT = 3;      //!< LCD back light pin;
uint8_t LCD_DATA_COMMAND = 8;   //!< Transmit data or command pin;
uint8_t LCD_RESET = 12;         //!< LCD reset pin;
uint8_t LCD_SPI_SCE = 5;        //!< SPI chip select pin;

//! Object which represents our SPI interface.
/*!
 *  Refer to SPI.h for more details.
 */
SPIClass oSPIiface;
/*
 *  We need a pointer to this object,
 *  all because we will need to pass it
 *  to another object.
 */
SPIClass * pSPIiface = & oSPIiface;

//! Object which represents our Nokia LCD.
/*!
 *  Refer to PCD8544.h file for more details.
 */
PCD8544 oLCD
(
    LCD_SPI_SCE,
    LCD_RESET,
    LCD_DATA_COMMAND,
    LCD_BACKLIGHT,
    pSPIiface
);

void setup()
/*!<
 * This method will be executed only once
 * just after power on.
 */
{

    pSPIiface->begin();     //  Initiate SPI object.
    oLCD.Init();            //  Initiate LCD object.
    oLCD.SetBackLight(127); //  Setup LCD back light (max 255).
    oLCD.HelloWorld();      //  Use embedded Hello World method.
    /*
     * More or less here we shall be able to find
     * simple text message on our LCD display.
     */
}

void loop()
/*!<
 * This method will be executed in infinity loop.
 */
{
    for (int i=0; i<=5; i++)
    //< Our display has 6 lines and we will walk through them.
    {
        delay(1500);                // Wait 1.5 second.
        oLCD.Clear();               // Clear LCD display.
        oLCD.GotoXY(10, i);         // Go to i line.
        oLCD.Print("Hello World");  // Print Hello World message.;
    }
}
