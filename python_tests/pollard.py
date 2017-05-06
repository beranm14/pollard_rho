import random

def gcd(n, m):
	if m == 0:
		return n 
	else:
		#print hex(n % m), " \n"
		return gcd (m, n % m)

import time 

def pollardRho(N):
	if N%2==0:
		return 2
	x = 7
	y = ((x*x)%N+1)%N
	c = 1
	g = 1
	j = 0
	while g==1:             
		x = ((x*x)%N+c)%N
		y1 = ((y*y)%N+c)%N
		y = ((y1*y1)%N+c)%N
		g = gcd(abs(x-y),N)
		"""
		print(hex(x))
		print(hex(y))
		print(hex(N))
		print j 
		print "====================="
		"""
		j = j + 1
		if j%1000 == 0:
			print j
		#e.sleep(1)		
	print j
	return g

#print hex(0x7530eca9*0x200000000)
#print hex(gcd(0xF001099912345678, 0xF00000007530eca9))
#print gcd(0x1B69B4BACD05F15, 0xDB4DA5F7EF412B1)

#print hex((0x0000011110000009)%(0x100000002))
print hex(pollardRho(0x11110000009*0x11110000009))
#print hex(pollardRho(1238926361552897*93461639715357977769163558199606896584051237541638188580280321))

"""
print hex(pollardRho(1234567))
print hex(pollardRho(0x8ebaf1b9febaf1bafeba98767530eca9))
print hex(1238926361552897*93461639715357977769163558199606896584051237541638188580280321)
"""
#for i in range(2,1000):
#	print (str(i) + " " + str(pollardRho(i)) + " div " +str(i // pollardRho(i)))

