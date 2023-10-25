#!/usr/bin/env python
import os

i = 1

while i < 2000 :
	print('\nPage num ' + str(i))
	val = os.system('./tlb ' + str(i) + ' ' + str(100000))
	i = i * 2
 
