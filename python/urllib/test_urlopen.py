#!/bin/python

import urllib2

resp = urllib2.urlopen("http://www.ingenic.cn")

print resp.read()
