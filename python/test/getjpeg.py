__author__ = 'wqshao'


import urllib
import re

def gethtml(url):
    page = urllib.urlopen(url)
    html = page.read()
    return html

def getImg(html):
    reg = r'src="(.+?\.jpg)" size'
    imgre = re.compile(reg)
    imglist = re.findall(imgre,html)
    x = 0
    for imgurl in imglist:
        urllib.urlretrieve(imgurl,'%s.jpg' % x)
        x+=1
   # return imglist

html = gethtml("http://tieba.baidu.com/p/4485863414")

print (getImg(html))