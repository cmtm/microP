import math

def filter_signal(signal, d):
	return [sum(signal[max(0, i - d + 1) : i + 1])/d for i in range(len(signal))]	

