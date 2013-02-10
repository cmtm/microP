import matplotlib.pyplot as plt
import numpy
from filter import filter_signal
import sys

unfilteredSig = []

for line in open('unfilteredTemp.txt', 'r'):
	line = line.strip()
	unfilteredSig.append( float(line) )




plt.plot(unfilteredSig)
plt.title("Unfiltered")
plt.savefig("unfiltered.png")

plt.clf()



spectrum = numpy.fft.fft(unfilteredSig, 1024)

plt.plot(spectrum)
plt.title("spectrum")
plt.savefig("spectrum.png")

plt.clf()



for i in range(1,51, 5):

	plt.plot(filter_signal(unfilteredSig, i))
	plt.title("Filtered with d = " + str(i))
	plt.savefig("d" + str(i).zfill(2) + "_filtered.png")

	plt.clf()

