/**
 * display.cpp
 *
 * This file is part of THGadget.
 *
 * Copyright (C) 2017,  Hernan Monserrat hemonserrat<at>gmail<dot>com
 * Based on the TFT Touch Shield 2.0 examples - tftbmp by Loove 2013-1-21
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*********************************************/
#include <SPI.h>
#include <SD.h>
#include <TFTv2.h>
#include "display.hpp"

#define BUFFPIXEL       60                      ///< must be a divisor of 240 
#define BUFFPIXEL_X3    180                     ///< BUFFPIXELx3


const int __Gnbmp_height = 320;                 ///< bmp hight
const int __Gnbmp_width  = 240;                 ///< bmp width
unsigned char __Gnbmp_image_offset  = 0;        ///< offset
char __Gsbmp_files[] = "night.bmp";

/**
 * Load a default image from the SD card named by #__Gsbmp_files
 */
void loadBackground(void)
{
  File bmpFile;
        bmpFile = SD.open(__Gsbmp_files);
        if (! bmpFile)
        {
            Serial.println("didnt find image");
            while (1);
        }
        if(! bmpReadHeader(bmpFile)) 
        {
            Serial.println("bad bmp");
            while(1);
        }

        bmpdraw(bmpFile, 0, 0);
        bmpFile.close();
}



/** 
 * This procedure reads a bitmap and draws it to the screen
 * its sped up by reading many pixels worth of data at a time
 * instead of just one pixel at a time. increading the buffer takes
 * more RAM but makes the drawing a little faster. 20 pixels' worth
 * is probably a good place
 */
void bmpdraw(File bmpFile, int x, int y)
{
    bmpFile.seek(__Gnbmp_image_offset);

    uint32_t time = millis();

    uint8_t sdbuffer[BUFFPIXEL_X3];                 // 3 * pixels to buffer

    for (int i=0; i< __Gnbmp_height; i++)
    {

        for(int j=0; j<(240/BUFFPIXEL); j++)
        {
            bmpFile.read(sdbuffer, BUFFPIXEL_X3);
            uint8_t buffidx = 0;
            int offset_x = j*BUFFPIXEL;
            
            unsigned int __color[BUFFPIXEL];
            
            for(int k=0; k<BUFFPIXEL; k++)
            {
                __color[k] = sdbuffer[buffidx+2]>>3;                        // read
                __color[k] = __color[k]<<6 | (sdbuffer[buffidx+1]>>2);      // green
                __color[k] = __color[k]<<5 | (sdbuffer[buffidx+0]>>3);      // blue
                
                buffidx += 3;
            }

            Tft.setCol(offset_x, offset_x+BUFFPIXEL);
            Tft.setPage(i, i);
            Tft.sendCMD(0x2c);                                                  
            
            TFT_DC_HIGH;
            TFT_CS_LOW;

            for(int m=0; m < BUFFPIXEL; m++)
            {
                SPI.transfer(__color[m]>>8);
                SPI.transfer(__color[m]);
            }

            TFT_CS_HIGH;
        }
        
    }
    
    Serial.print(millis() - time, DEC);
    Serial.println(" ms");
}

/**
 * Decode the BMP header information
 */
boolean bmpReadHeader(File f) 
{
    // read header
    uint32_t tmp;
    uint8_t bmpDepth;
    
    if (read16(f) != 0x4D42) {
        // magic bytes missing
        return false;
    }

    // read file size
    tmp = read32(f);
//    Serial.print("size 0x");
//    Serial.println(tmp, HEX);

    // read and ignore creator bytes
    read32(f);

    __Gnbmp_image_offset = read32(f);
 //   Serial.print("offset ");
 //   Serial.println(__Gnbmp_image_offset, DEC);

    // read DIB header
    tmp = read32(f);
 //   Serial.print("header size ");
 //   Serial.println(tmp, DEC);
    
    
    int bmp_width = read32(f);
    int bmp_height = read32(f);
    
    if(bmp_width != __Gnbmp_width || bmp_height != __Gnbmp_height)      // if image is not 320x240, return false
    {
        return false;
    }

    if (read16(f) != 1)
    return false;

    bmpDepth = read16(f);
 //   Serial.print("bitdepth ");
 //   Serial.println(bmpDepth, DEC);

    if (read32(f) != 0) {
        // compression not supported!
        return false;
    }

//    Serial.print("compression ");
//    Serial.println(tmp, DEC);

    return true;
}

/*********************************************/
// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f)
{
    uint16_t d;
    uint8_t b;
    b = f.read();
    d = f.read();
    d <<= 8;
    d |= b;
    return d;
}

// LITTLE ENDIAN!
uint32_t read32(File f)
{
    uint32_t d;
    uint16_t b;

    b = read16(f);
    d = read16(f);
    d <<= 16;
    d |= b;
    return d;
}

