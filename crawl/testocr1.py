#!/usr/bin/python
# -*- coding: utf-8 -*- 
'''
@author: storm
@date: 2020-05-21
@description: 

sudo apt-get install python3-pil
sudo pip3 install pytesseract
'''

import sys
from PIL import Image
import pytesseract 

# 打开图片
#im = Image.open('/media/psf/Home/Desktop/upan/jpg/ocr.png')
im = Image.open('/media/psf/Home/Desktop/upan/qqrtx/QQ/challenge.jpg')

# 转为灰度图片
imgry = im.convert('L')
#imgry.show()

# 二值化
table = []
for i in range(256):
  if i < 140:
    table.append(0)
  else:
    table.append(1)

image_bi = imgry.point(table, '1')
# 识别验证码
verify_code = pytesseract.image_to_string(image_bi)
print (verify_code)
