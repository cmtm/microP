import matplotlib.pyplot as plt

x = []
y = []

for line in open('realOutput.txt', 'r'):
	line = line.split(';')
	x.append( float(line[1].strip().replace(' ', '.')) )
	y.append( float(line[2].strip().replace(' ', '.')) )

plt.plot(x)
plt.title("Unfiltered")
plt.savefig("unfiltered.png")

plt.clf()

plt.plot(y)
plt.title("Filtered")
plt.savefig("filtered.png")

