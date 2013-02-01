import matplotlib.pyplot as plt
import sys

x = []
y = []

for line in open('realOutput.txt', 'r'):
	line = line.split(';')
	x.append( float(line[1].strip().replace(' ', '.')) )
	y.append( float(line[2].strip().replace(' ', '.')) )

filename = sys.argv[1]


plt.plot(x)
plt.title("Unfiltered")
plt.savefig(filename + "_unfiltered.png")

plt.clf()

plt.plot(y)
plt.title("Filtered")
plt.savefig(filename+ "_filtered.png")

