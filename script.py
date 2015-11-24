import random

f = open("file", "wb")
for i in xrange(0x2000):
	f.write(chr(random.randint(0, 255)))
f.close()