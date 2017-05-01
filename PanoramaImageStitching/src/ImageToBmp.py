"""
This py is used to transform Working Directory files' Images into
bmp format and rename them with 1,2,3 ...
"""

import os, imghdr
from PIL import Image

count = 1
for item in os.listdir():
    if imghdr.what(item) != None:
        Image.open(item).save(str(count)+".bmp")
        count=count+1

