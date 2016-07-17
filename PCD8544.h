/*
 * PCD8544.h
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

#ifndef PCD8544_H_INCLUDED  // Guard block.
#define PCD8544_H_INCLUDED  //!< Guard block.
#define LCD_C     LOW       //!< Data / Command pin LOW for Command.
#define LCD_D     HIGH      //!< Data / Command pin HIGH for Data.
#define LCD_X     84        //!< Max horizontal resolution.
#define LCD_Y     48        //!< Max vertical resolution.

#include <Arduino.h>        // Required by Arduino environment.
#include <avr/pgmspace.h>   // Required by pgm_read_byte().
#include "ASCII.h"          // ASCII to font map.
#include <SPI.h>            // Required by class SPIClass.
/*
 * Related to #include <SPI.h> and some compilation errors:
 * Add following line:
 * #include <SPI.h>
 * also in sketch *.ino file.
 *
 */


//! PCD8544 Class (Nokia LCD Display (SPI module))
/*!
 *  PCD8544 Class
 *  supports Nokia LCD Display (SPI module).
 *
 *  \sa     ClearDisplayMap(), Init(), GotoXY(), Clear(), Print(), AddToDisplayMap(), RemoveFromDisplayMap(), PrintOnDisplayMap(), UpdateDisplay(), SetBackLight(), GetBackLight(), HelloWorld()
 *
 *          Example Pinout:
 *
 *              LCD                 Arduino
 *          1 - VCC	3.3V        3.3V only (not 5V!)
 *          2 - GND             GND
 *                                  10 uF Capacitor between VCC and GND
 *          3 - SCE	            5	Can be any digital pin.
 *          4 - RST	            12	Can be any digital pin.
 *          5 - D/C	            8	Can be any digital pin.
 *          6 - DN(MISO)	    11	Can't be moved.
 *          7 - SCLK            13	Can't be moved.
 *          8 - LED	            3	Can be any PWM pin.
 *
 *          Be aware that in between Arduino signal pins and LCD display
 *          some resistors shall be used.
 */
class PCD8544 {
public:
    //! PCD8544() Constructor Method.
    PCD8544
    (
        uint8_t & sce,          //!< SPI Chip Enable pin;
        uint8_t & rst,          //!< LCD Reset Pin;
        uint8_t & dc,           //!< LCD Data / Command Selection Pin;
        uint8_t & backlight,    //!< LCD Back Light Pin;
        SPIClass * pSPIClassObj //!< SPIClass object (SPI interface);
    );

    //! PCD8544() Destructor Method - Empty / Default.
    ~PCD8544();

    //! Clear display map (array in RAM, not a hardware display).
    /*!
     *  Clear display map (array in RAM, not a hardware display).
     *  \sa AddToDisplayMap(), PrintOnDisplayMap(), UpdateDisplay(), RemoveFromDisplayMap()
     */
    void ClearDisplayMap();

    //! Initiate PCD8544 Object
    void Init();

    //! Move caret to X,Y coordinates.
    /*!
     *  Method works on hardware display directly.
     *  Moves caret to X,Y coordinates.
     *  Useful before we write directly on hardware display.
     *  \param  x   uint8_t horizontal coordinate (column in pixels),
     *  \param  y   uint8_t vertical coordinate (row in pixel group of 8), min=0, max=4.
     *  \sa Print()
     *
     */
    void GotoXY(uint8_t x, uint8_t y);

    //! Flush hardware display.
    void Clear();

    //! Print the string on hardware display.
    /*!
     *  \param  chars   const char *    string
     *  \sa PrintOnDisplayMap(), GotoXY()
     */
    void Print(const char * chars);

    //! Add bit map to display map.
    /*!
     *  Add bit map (content) to display map (array in RAM, not a hardware display).
     *  \param  content const uint8_t * bit map
     *  \sa ClearDisplayMap(), PrintOnDisplayMap(), UpdateDisplay(), RemoveFromDisplayMap()
     */
    void AddToDisplayMap(const uint8_t *);

    //! Remove bit map from display map.
    /*!
     *  Remove bit map (content) from display map (array in RAM, not a hardware display).
     *  \param  content const uint8_t * bit map
     *  \sa ClearDisplayMap(), PrintOnDisplayMap(), UpdateDisplay(), AddToDisplayMap()
     *
     *      If there is no pixel on map
     *          then leave it as is.
     *      If there is an pixel on map and we want to flush it
     *          then flush it.
     *      If there is an pixel on map and we want to keep it
     *          then leave it as is.
     *      #########################
     *      a   =   a   &   ~b
     *      =========================
     *      0   =   0   &   0(~1)
     *      0   =   0   &   1(~0)
     *      0   =   1   &   0(~1)
     *      1   =   1   &   1(~0)
     */
    void RemoveFromDisplayMap(const uint8_t *);

    //! Print the string on display map.
    /*!
     *  Print the string on display map (array in RAM, not a hardware display).
     *  \param  chars   const char *  string,
     *  \param  iColumnX    uint8_t horizontal coordinate (column in pixels),
     *  \param  iRowY       uint8_t vertical coordinate (row in pixel group of 8), min=0, max=4.
     *  \sa ClearDisplayMap(), RemoveFromDisplayMap(), UpdateDisplay(), AddToDisplayMap(), Print()
     */
    void PrintOnDisplayMap(
                           const char * chars,
                           uint8_t iColumnX,
                           uint8_t iRowY
                           );

    //! Send display map to hardware display.
    /*!
     *  Copy whole display map (array in RAM) to hardware display
     *  using SPI interface.
     *  \sa ClearDisplayMap(), RemoveFromDisplayMap(), UpdateDisplay(), AddToDisplayMap()
     */
    void UpdateDisplay();

    //! Setup LCD Back Light.
    /*!
     *  Setup LCD Back Light.
     *  \param  blValue uint8_t value in range of <0,255> where 0 means that back light is off.
     *  \sa GetBackLight()
     */
    void SetBackLight(uint8_t blValue);

    //! Get LCD Back Light Value.
    /*!
     *  Get LCD Back Light Value.
     *  \return uint8_t value in range of <0,255> where 0 means that back light is off.
     *  \sa SetBackLight()
     */
    uint8_t GetBackLight();

    //! Simple "Hello World" like method.
    /*!
     *  Simple "Hello World" like method.
     *  Print on hardware display this particular class
     *  header and source file names.
     *  \sa Clear(), GotoXY(), Print()
     *
     */
    void HelloWorld();

 private:
    uint8_t pin_sce;        //!< SPI Chip Enable pin;
    uint8_t pin_rst;        //!< LCD Reset Pin;
    uint8_t pin_dc;         //!< LCD Data / Command Selection Pin;
    uint8_t pin_backlight;  //!< LCD Back Light Pin;
    uint8_t backLightValue; //!< LCD Back Light Value;

    //! Display Map
    /*!
     *  This map exist in RAM.
     *  It is dedicated to collect all display changes
     *  before sending them to hardware display.
     *  \sa UpdateDisplay()
     *
     */
    uint8_t displayMap[LCD_X * LCD_Y / 8];

    //!< SPIClass object (SPI interface);
    SPIClass * pSPI;

    //! Send data (or command).
    void TxRawData(uint8_t dataNotCommand, uint8_t rawData);

    //! Print one particular char on hardware display.
    void PrintChar(char oneChar);

    //! Print one particular char on display map.
    void PrintCharOnDisplayMap(
                               char oneChar,
                               uint8_t & iColumnX,
                               uint8_t & iRowY
                               );
};

#endif  // Guard block.
