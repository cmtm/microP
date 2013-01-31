import matplotlib.pyplot as plt
import sys

signal = [float(num.strip()) for num in sys.argv[1][1:-1].split(',')]
plt.plot(signal)
plt.ylabel('flat signal')
plt.savefig("rough.png")
