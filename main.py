import xfmcparser.xfmcparser as xfmcparser
import numpy as np

f="datasets/px14387_1_data.bin"


with open(f, mode='rb') as fi:
    stream = fi.read() 

indexes = np.sort(np.random.randint(0,len(stream),size=(10)))
print(indexes)

#print(type(stream))
#print(stream[0:100])
var = xfmcparser.getbyte(stream, len(stream))

print(var)

a = np.array([1.1,2.2,3.3])
b = np.array([1,2,3],dtype=np.int8)

xfmcparser.mainline(a,b)

a=1