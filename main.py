import parsercore.parsercore_lib as parsercore
import numpy as np

f="datasets/px14387_1_data.bin"


with open(f, mode='rb') as fi:
    stream = fi.read() 

indexes = np.sort(np.random.randint(0,len(stream),size=(10),dtype=np.uint64))
data = np.sort(np.random.randint(0,len(stream),size=(20),dtype=np.uint64))
print(f"input random indexes: {indexes}")
print(f"stream 0-50: {stream[0:50]}")
print(f"stream length: {len(stream)}")

print("---byte function---")
byteindex = int(indexes[0])
print(f"Streamed byte: {hex(stream[byteindex])}")    
parsercore.printbyte(stream, len(stream), byteindex)
#print(byte1[0], type(byte1[0]))
#print(f"Returned byte: {hex(byte1[0])}")

print("")
print("---np.float function---")
a = np.array([1.1,2.2,3.3])
b = np.array([1,2,3],dtype=np.int8)
result1=parsercore.doubleprint(a)
print("---")
print(f"Returned double: {result1}")

print("")
print("---np.uin64 function---")
result2=parsercore.indexret(indexes)
print("---")
print(f"Returned uint64: {result2}")

print("")
print("-----------------------")
print("---combined function---")
print("-----------------------")
parsercore.indexbyte(indexes, stream, len(stream))

d = parsercore.returnchanarray(indexes, stream, len(stream))
print("---print a---")
print(d)
print(d.dtype)
print(type(d))

c = parsercore.readpixel(stream, len(stream))
print("---print a---")
print(c)
print(c.dtype)
print(type(c))

"""

b = np.ones(10)*3
print(b.dtype)
parsercore.pbtest(b)
"""

a=1