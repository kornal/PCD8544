#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  Copyright 2016 Marcin Kornalewski <kornalek{a\t}interia{d/o/t}pl
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#

"""@package png2CarrayConverter
  Converts PNG image file in to C arrays
  and store in separated header files.
  Useful when we need to display an image
  using embedded LCD display like
  PCD8544 (Old Nokia display).
"""

import png
import os.path
import sys

class Image:
    """
    Image (picture) Abstract
    """

    def __init__(self):
        self.__sSourceFilePath=''
        self.__sDestFileName=""
        self.__sFileDir=''
        self.__sFileName=''
        self.__sFileNameBaseline=''
        self.__sFileExt=''
        self.__oImage=png.Reader
        self.__iImageSizeX=0
        self.__iImageSizeY=0
        self.__iImagePlanes=0
        self.__iPixelOffCode=0
        self.__aCarray=[]

    def __getCArraySize__(self):
        """ Get C Array Size
        
        Returns:
            int: C array size in bytes (=items).
        """
        iCarraySize=self.__iImageSizeY/8
        if ( self.__iImageSizeY%8 > 0 ):
            iCarraySize += 1
        iCarraySize *= self.__iImageSizeX
        return iCarraySize

    def importFromFile(self, filePath):
        """ Import image from the source file.
        
        Args:
            filePath (str): Name of the source image file (with extension).
        Returns:
            bool: The return value. Zero for success.
        """
        self.__sSourceFilePath=filePath
        self.__oImage=png.Reader(self.__sSourceFilePath)
        self.__iImageSizeX=self.__oImage.read()[0]
        self.__iImageSizeY=self.__oImage.read()[1]
        self.__iImagePlanes=self.__oImage.read()[3]['planes']
        self.__iPixelOffCode=( \
            (2**self.__oImage.read()[3]['bitdepth'])-1 )
        # This is how we get WHITE code.

        self.__sFileDir=os.path.dirname(filePath)
        self.__sFileName=os.path.basename(filePath)
        (self.__sFileNameBaseline, self.__sFileExt) = \
            os.path.splitext(filePath)
        return 0

    def processToCArray(self):
        """ Process To C Array
        
        This method will flush then update C array.
        
        Returns:
            bool: The return value. Zero for success.
        """

        iBitInByteWeight=0
        iCurrentRow=0

        self.__aCarray = [ 0 for i in range(self.__getCArraySize__())]

        for row in self.__oImage.read()[2]:
            # For each row in the bitmap:
            for column in range(self.__iImageSizeX):

                offset = column*self.__iImagePlanes
                # offset depends on what kind of date
                # (e.g. alpha channel) do we have in PNG file

                if (    row[offset] == 0 & \
                        row[offset+1] == 0 & \
                        row[offset+2] == 0):
                    # If pixel is BLACK:
                    self.__aCarray[ \
                        (((iCurrentRow)/8) * \
                        (self.__iImageSizeX)+column) \
                        ] += 2**iBitInByteWeight

                iEndLineTest=( \
                    (self.__iImageSizeX * self.__iImagePlanes) \
                    - self.__iImagePlanes)

                if ( offset == iEndLineTest ):
                    iBitInByteWeight += 1
                    # New line means also next bit in the byte.
                    if (iBitInByteWeight >= 8):
                        # In case our pointer to bit position in
                        # byte overflow we need to reset it.
                        iBitInByteWeight=0
            iCurrentRow += 1
        return 0

    def exportToFile(self, **kwarg):
        """ Export image to C header file.
        
        Consider main() for an example.
        
        Args:
            bCONST (bool): True if final header file will contain constant data.
            sType (str): Type of data in issued header file.
            bAvrProgMem (bool): True if we want to use AVR PROGMEM.
            sImageFullName (str): Brief description of the image file.
            aImageFullDescription : Full description of the image file (list of strings).
            sCopyRight (str): Copy Rights sentence.
            aLicense : License (list of strings).
        
        Returns:
            bool: The return value. Zero for success.
        """

        sDestFileExt="h"
        # Destination file extension will be h
        # In case you need something else (like hpp)
        # you can specify it here.

        if kwarg['bCONST']:
            self.__sDestFileName = ( self.__sFileNameBaseline.upper() + \
                                "." + sDestFileExt )
            sHeadLineConstPrefix = "const "
            sHeadLineName = self.__sFileNameBaseline.upper()
        else:
            self.__sDestFileName = ( self.__sFileNameBaseline + \
                                "." + sDestFileExt )
            sHeadLineConstPrefix = ""
            sHeadLineName = self.__sFileNameBaseline

        if kwarg['bAvrProgMem']:
            sHeadLineAvrProgMem = "PROGMEM "
        else:
            sHeadLineAvrProgMem = ""

        sDestFilePath=( os.path.join(self.__sFileDir, \
                                        "..//", \
                                        self.__sDestFileName) )

        sGuardName = ( self.__sFileNameBaseline.upper() + \
                        "_" + \
                        sDestFileExt.upper() )

        aDestFileHeader=[ \
            "/*\n", \
            " * File generated by png2CarrayConverter.py\n", \
            " *\n", \
            " * ", \
            kwarg['sCopyRight'], \
            " *\n", \
            " */\n" ]

#        aDefaultShortDescription=[ \
#            "//! Image Full Name\n", ]

#        aDefaultFullDescription=[ \
#            "/*! \n", \
#            " * Image Full Description\n", \
#            " */\n" ]

        aHeadline=[ \
            sHeadLineConstPrefix, \
            kwarg['sType'], \
            " ", \
            sHeadLineAvrProgMem, \
            sHeadLineName, \
            " [] = {", \
            "\n"]

        aFooter=["};\n"]

        aGuardBlockBegin=[ "\n", \
            "#ifndef ", \
            sGuardName, \
            "\n", \
            "#define ", \
            sGuardName, \
            " //!< Guard block.", \
            "\n", \
            "\n"]

        aGuardBlockEnd=[ "\n", \
            "#endif "
            "\n", \
            "\n"]

        fDestFile=open(sDestFilePath, 'w', 0)
        fDestFile.close()

        fDestFile=open(sDestFilePath, 'a')

        for i in aDestFileHeader:
            fDestFile.write(i)

        for i in kwarg['aLicense']:
            fDestFile.write(i)

        for i in aGuardBlockBegin:
            fDestFile.write(i)

        for i in aHeadline:
            fDestFile.write(i)

#        for i in aDefaultShortDescription:
#            fDestFile.write(i)
        if ( kwarg['sImageFullName'] ):
            fDestFile.write(kwarg['sImageFullName'])

#        for i in aDefaultFullDescription:
#            fDestFile.write(i)
        if ( kwarg['aImageFullDescription'] ):
            for i in kwarg['aImageFullDescription']:
                fDestFile.write(i)

        iNewLineIndicator=0
        for i in range(len(self.__aCarray)):
            # fDestFile.write(str(hex(self.__aCarray[i])))
            fDestFile.write( "{0:#0{1}x}".format(self.__aCarray[i], 4) )
            fDestFile.write(", ")

            iNewLineIndicator += 1
            if ( iNewLineIndicator == self.__iImageSizeX ):
                iNewLineIndicator=0
                fDestFile.write("\n")

        for i in aFooter:
            fDestFile.write(i)

        for i in aGuardBlockEnd:
            fDestFile.write(i)

        fDestFile.close()
        return 0

    def printInfo(self):
        """ Print some information on console.
        
        Returns:
            bool: The return value. Zero for success.
        """
        print("Working Dir: {0}" \
            .format(self.__sFileDir) )
        print("Source File Name: {0}" \
            .format(self.__sFileName) )

        print("Destination File Name: {0}" \
            .format(self.__sDestFileName) )

        print("IMG size is: {0}x{1}." \
            .format(self.__iImageSizeX, self.__iImageSizeY))

        return 0

def main():
    """ Example of how to use this module.
    """
    oImageUnderConversion = Image()
    for sourceFile in sys.argv[1:]:
        oImageUnderConversion.importFromFile(sourceFile)
        oImageUnderConversion.processToCArray()

        oImageUnderConversion.exportToFile( bCONST=True, \
                                            sType="unsigned char", \
                                            bAvrProgMem=True, \
                                            sImageFullName= \
        "//! Image in C array\n", \
                                            aImageFullDescription=[ \
        "/*\n", \
        " * Image file converted to C array.\n", \
        " *\n", \
        " */\n" \
        ], \
                                            sCopyRight= \
        "Copyright 2016 MY NAME, MY EMAIL", \
                                            aLicense=[ \
        "/*\n", \
        " * This program is free software; you can redistribute it and/or modify\n", \
        " * it under the terms of the GNU General Public License as published by\n", \
        " * the Free Software Foundation; either version 2 of the License, or\n", \
        " * (at your option) any later version.\n", \
        " *\n", \
        " * This program is distributed in the hope that it will be useful,\n", \
        " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n", \
        " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n", \
        " * GNU General Public License for more details.\n", \
        " *\n", \
        " * You should have received a copy of the GNU General Public License\n", \
        " * along with this program; if not, write to the Free Software\n", \
        " * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,\n", \
        " * MA 02110-1301, USA.\n", \
        " */\n" ] \
        )

        oImageUnderConversion.printInfo()

    return 0

if __name__ == '__main__':
    main()
