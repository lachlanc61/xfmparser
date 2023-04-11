import xfmcparser.xfmcparser as xfmcparser
import numpy as np

f="datasets/px14387_1_data.bin"


with open(f, mode='rb') as fi:
    stream = fi.read() 

indexes = np.sort(np.random.randint(0,len(stream),size=(10),dtype=np.uint64))
print(f"input random indexes: {indexes}")
print(f"stream 0-50: {stream[0:50]}")


print("---byte function---")
byteindex = int(indexes[0])
print(f"Streamed byte: {hex(stream[byteindex])}")    
byte1 = xfmcparser.getbyte(stream, len(stream), byteindex)
print(f"Returned byte: {hex(byte1[0])}")

print("")
print("---np.float function---")
a = np.array([1.1,2.2,3.3])
b = np.array([1,2,3],dtype=np.int8)
result1=xfmcparser.mainline(a)
print(f"Returned double: {result1}")

print("")
print("---np.uin64 function---")
result2=xfmcparser.indexin(indexes)
print(f"Returned uint64: {result2}")



print("")
print("---combined function---")
print(f"Streamed byte: {hex(stream[byteindex])}")    
xfmcparser.combine(a, stream, len(stream), byteindex)


a=1