import xfmcparser.xfmcparser as xfmcparser
import numpy as np

f="datasets/px14387_1_data.bin"


with open(f, mode='rb') as fi:
    stream = fi.read() 

indexes = np.sort(np.random.randint(0,len(stream),size=(10),dtype=np.uint64))
data = np.sort(np.random.randint(0,len(stream),size=(20),dtype=np.uint64))
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
result1=xfmcparser.doubleprint(a)
print("---")
print(f"Returned double: {result1}")

print("")
print("---np.uin64 function---")
result2=xfmcparser.indexret(indexes)
print("---")
print(f"Returned uint64: {result2}")

print("")
print("-----------------------")
print("---combined function---")
print("-----------------------")
byten1 = xfmcparser.indexbyte(indexes, stream, len(stream), indexes)

print("---python values---")
for i in indexes:
    print(f"[n] {[i]} {hex(stream[i])}")
print("---")
print(f"Returned byte: {hex(byten1[0])}")

print("---pointer test---")
xfmcparser.ptrtrial()


b = np.ones(10)*3
print(b.dtype)
xfmcparser.pbtest(b)

a=1