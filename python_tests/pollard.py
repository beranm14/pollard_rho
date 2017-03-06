import random

def gcd(n, m):
	if m == 0:
		return n 
	else:
		#print hex(n % m), " \n"
		return gcd (m, n % m)

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
		j = j + 1
		print j 
	return g

#print hex(0x7530eca9*0x200000000)
#print hex(gcd(0xF001099912345678, 0xF00000007530eca9))
#print gcd(0x1B69B4BACD05F15, 0xDB4DA5F7EF412B1)

print pollardRho(162167)
# print pollardRho(1234567)

#for i in range(2,1000):
#	print (str(i) + " " + str(pollardRho(i)) + " div " +str(i // pollardRho(i)))

