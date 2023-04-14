import parsercore.parsercore_lib as parsercore
import numpy as np
import sys

BYTEORDER='little'

def intfrombyte(byte):
    return int.from_bytes(byte, byteorder=BYTEORDER)

def binfrombyte(byte):
    binary = bin(int.from_bytes(byte, byteorder=BYTEORDER))
    padded = format(int.from_bytes(byte, byteorder=BYTEORDER), "#010b")
    hashless_padded = format(int.from_bytes(byte, byteorder=BYTEORDER), "010b")
    return padded

def hexfrombyte(byte):
    return hex(int.from_bytes(byte, byteorder=BYTEORDER))


def main():
    if BYTEORDER != sys.byteorder:
        print("WARNING: system byteorder differs from data byteorder")

    f="datasets/px14387_1_data.bin"
    fct="datasets/px14387_1_data_counts.npy"
    fchan="datasets/px14387_1_data_chan.npy"

    counts=np.load(fct)
    chan=np.load(fchan)
    correct_extract=np.ravel(np.vstack((chan, counts)),order='F')

    with open(f, mode='rb') as fi:
        stream = fi.read() 

    indexes = np.sort(np.random.randint(0,len(stream),size=(10),dtype=np.uint64))
    data = np.sort(np.random.randint(0,len(stream),size=(20),dtype=np.uint64))
    print(f"input random indexes: {indexes}")
    print(f"stream 0-50: {stream[0:50]}")
    print(f"stream length: {len(stream)}")
    print(f"converted stream: {correct_extract[0:50]}")


    print("---byte function---")
    print(type(stream))
    byteindex = int(indexes[0])
    byteindex = 0

    byte=stream[byteindex:byteindex+1]
    byte16=stream[byteindex:byteindex+1]
    print(f"Streamed byte: {(byte)}")
    print(f"int byte:      {(intfrombyte(byte))}")
    print(f"hex byte:      {(hexfrombyte(byte))}")
    print(f"binary byte:   {(binfrombyte(byte))}")
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
    print(c[0:6])
    print(c[6:12])
    print(c[12:18])
    print(c[18:24])
    print(c.dtype)
    print(type(c))

    """

    b = np.ones(10)*3
    print(b.dtype)
    parsercore.pbtest(b)
    """

    a=1

if __name__ == "__main__":
    main()

    sys.exit()