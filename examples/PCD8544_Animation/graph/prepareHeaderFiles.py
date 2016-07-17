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

"""@package prepareHeaderFiles.py
  Script prepare header files
  with  arrays dedicated to particular image.
"""

from os import path as ospath
from sys import path as syspath

utilitiesPath = ospath.abspath("../../../utilities/")
if utilitiesPath not in syspath:
    """
    We need to import png2CarrayConverter module
    however its location is unknown for python (true for default environment).
    In other words issued module is not covered by PYTHONPATH
    If it is true then we will add it.
    """
    syspath.insert(0, utilitiesPath)

import png2CarrayConverter

def doConversion(sourceFile, fullName, description):
    """ This function will do conversion for one particular image.
    
    Import png2CarrayConverter module before it!
    
    Args:
        sourceFile (str): Name of the source image file (with extension).
        fullName (str): Brief description of the image file.
        description : Full description of the image file (list of strings).
    Returns:
        bool: The return value. Zero for success.
    """

    oImageUnderConversion = png2CarrayConverter.Image()
    oImageUnderConversion.importFromFile(sourceFile)
    oImageUnderConversion.processToCArray()

    oImageUnderConversion.exportToFile( bCONST=True, \
                                        sType="unsigned char", \
                                        bAvrProgMem=True, \
                                        sImageFullName= fullName, \
                                        aImageFullDescription=description, \
                                        sCopyRight= \
    "Copyright 2016 Marcin Kornalewski, <kornalek{a/t}interia{d/o/t}pl", \
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

def main():
    
    doConversion( \
        "she.png", \
        "//! Pictograph of a woman. \n", \
        [ \
        "/*\n", \
        " * Pictograph represents a woman. \n", \
        " * Image file converted to C array. \n", \
        " *\n", \
        " */\n" \
        ]\
    )
    
    doConversion( \
        "he0.png", \
        "//! Pictograph of a man during jump. \n", \
        [ \
        "/*\n", \
        " * Pictograph represents a man during a jump. \n", \
        " * Jump state: 0 (steady). \n", \
        " * Image file converted to C array. \n", \
        " *\n", \
        " */\n" \
        ]\
    )
    
    doConversion( \
        "he1.png", \
        "//! Pictograph of a man during jump. \n", \
        [ \
        "/*\n", \
        " * Pictograph represents a man during a jump. \n", \
        " * Jump state: 1. \n", \
        " * Image file converted to C array. \n", \
        " *\n", \
        " */\n" \
        ]\
    )
    
    doConversion( \
        "he2.png", \
        "//! Pictograph of a man during a jump. \n", \
        [ \
        "/*\n", \
        " * Pictograph represents a man during a jump. \n", \
        " * Jump state: 2. \n", \
        " * Image file converted to C array. \n", \
        " *\n", \
        " */\n" \
        ]\
    )
    
    doConversion( \
        "he3.png", \
        "//! Pictograph of a man during jump. \n", \
        [ \
        "/*\n", \
        " * Pictograph represents a man during a jump. \n", \
        " * Jump state: 3. \n", \
        " * Image file converted to C array. \n", \
        " *\n", \
        " */\n" \
        ]\
    )
    
    doConversion( \
        "he4.png", \
        "//! Pictograph of a man during jump. \n", \
        [ \
        "/*\n", \
        " * Pictograph represents a man during a jump. \n", \
        " * Jump state: 4. \n", \
        " * Image file converted to C array. \n", \
        " *\n", \
        " */\n" \
        ]\
    )
    
    doConversion( \
        "he5.png", \
        "//! Pictograph of a man during jump. \n", \
        [ \
        "/*\n", \
        " * Pictograph represents a man during a jump. \n", \
        " * Jump state: 5. \n", \
        " * Image file converted to C array. \n", \
        " *\n", \
        " */\n" \
        ]\
    )
    
    return 0

if __name__ == '__main__':
    main()
