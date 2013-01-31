outFile = open("testVectorProper.txt", 'w')
lines = open("testVector.txt", 'r').readlines()
outFile.write( '[' + ",  ".join([line.strip() for line in lines]) + ']')
outputFile.close()
