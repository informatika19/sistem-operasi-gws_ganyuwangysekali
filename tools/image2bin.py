#!/usr/bin/env python

from PIL import Image
from palette import palette

import sys

if len(sys.argv) < 3:
  print(f'Usage: {sys.argv[0]} <input> <output>')
  exit(1)
infile = sys.argv[1]
outfile = sys.argv[2]

img = Image.open(infile).convert()

# resize to fit small size
# small image = small size
width = img.size[0]
height = img.size[1]
if(height > width):
  height = 60
  width = int(img.size[0]*height/img.size[1])
else:
  width = 100
  height = int(img.size[1]*width/img.size[0])

resized = img.resize((width, height))

pixels = resized.convert('RGB').load()

binaryList = []

binFile = open(outfile, 'wb')
binFile.write(bytearray([width, height]))
for y in range(height):
  for x in range(width):
    smallestDiff = 0xFFFFFF
    smallestDiffIdx = 0
    idx = 0
    for c in palette:
      dif = sum([(pixels[x, y][i] - c[i])**2 for i in range(3)])
      if dif < smallestDiff:
        smallestDiff = dif
        smallestDiffIdx = idx
      idx+=1
    binFile.write(bytearray([smallestDiffIdx]))


binFile.close()