#!/usr/bin/python
from PIL import Image
import sys
print "Processing: " + sys.argv[1]
print "copy from here-------8<"

def rgb2hex(r, g, b):
    return '0x{:02x}{:02x}{:02x}'.format(r, g, b)

img = Image.open(sys.argv[1])

hexString = ""

if img.mode in ('RGBA', 'LA') or (img.mode == 'P' and 'transparency' in img.info):   
    pixels = img.convert('RGBA').load()
    width, height = img.size

    for x in range(width):
        hexString += '{'
        for y in range(height):
            r, g, b, a = pixels[x, y]
            hexString += rgb2hex(r, g, b)
            if y < height - 1:
                hexString += ","
        hexString += '},\n'

print hexString
print "\n------8<"
