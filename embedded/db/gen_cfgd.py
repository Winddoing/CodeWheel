#!/bin/python

import sqlite3
import os
import re

DB_FILE='cfgd.db'
DB_ORIG_FILE='cfgd.conf'
DB_TABLE_NAME='configTbl'

if os.path.exists(DB_FILE):
	os.remove(DB_FILE);

dbconn=sqlite3.connect(DB_FILE);
cur=dbconn.cursor();
cur.execute('create table {0}(name varchar(32) PRIMARY KEY,value varchar(128))'.format(DB_TABLE_NAME));

line_number = 0
with open(DB_ORIG_FILE,encoding='utf-8') as f:
	for aline in f:
		line_number = line_number + 1
		print('{:>4} {}'.format(line_number,aline))
		(name,value)=re.split(r'[\s=]+',aline.rstrip())
		print('{0}={1}'.format(name,value))
		cur.execute('insert into {0} values("{1}","{2}")'.format(DB_TABLE_NAME,name,value))

cur.close()
dbconn.commit()
dbconn.close()

