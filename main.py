import xfmcparser.xfmcparser as xfmcparser
import numpy as np

f="datasets/px14387_1_data.bin"


with open(f, mode='rb') as fi:
    stream = fi.read() 

indexes = np.sort(np.random.randint(0,len(stream),size=(10)))
print(f"input random indexes: {indexes}")

print("---individual functions---")
byte1 = xfmcparser.getbyte(stream, len(stream))
print(f"Returned byte: {byte1}")

a = np.array([1.1,2.2,3.3])
b = np.array([1,2,3],dtype=np.int8)

xfmcparser.mainline(a)

print("---combined function---")
xfmcparser.combine(a, stream, len(stream))

a=1