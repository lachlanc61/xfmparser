import parsercore.parsercore_lib as parsercore
import numpy as np
import sys

BYTEORDER='little'
NCHAN=4096
NDET=2
PXHEADERLEN=16

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
        raise EnvironmentError("FATAL: System byteorder differs from data byteorder, C++ pixel extraction will fail")

    fpx="datasets/px14387_1_data.bin"
    fpxct="datasets/px14387_1_data_counts.npy"
    raw_fct="datasets/raw/px14387_1_data_counts.npy"
    raw_fchan="datasets/raw/px14387_1_data_chan.npy"

    raw_counts=np.load(raw_fct)
    raw_chan=np.load(raw_fchan)
    chan=np.arange(NCHAN)
    counts=np.load(fpxct)

    print("dtypes:")
    print(f"counts {counts.dtype}, {chan.dtype}:")
    
    raw_target_1D=np.ravel(np.vstack((chan, counts)),order='F')

    with open(fpx, mode='rb') as fi:
        stream = fi.read() 

    indexes = np.sort(np.random.randint(0,len(stream),size=(10),dtype=np.uint64))
    data = np.sort(np.random.randint(0,len(stream),size=(20),dtype=np.uint64))
    print(f"input random indexes: {indexes}")
    print(f"stream 0-50: {stream[0:50]}")
    print(f"stream length: {len(stream)}")
    print(f"converted stream: {raw_target_1D[0:50]}")


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

    parsercore.printpixel(stream, len(stream))
    raw_out_1D = parsercore.readpixel1D(stream, len(stream))
    print("---print converted 1D array from C++---")
    print(raw_out_1D[0:12])
    print(raw_out_1D.dtype)
    print(raw_out_1D.shape)
    print("---print converted 1D array from Python---")
    print(raw_target_1D[0:12])
    print(raw_target_1D.dtype)
    print(raw_target_1D.shape)


    out_1D = parsercore.readpixelcounts(stream, len(stream))
    print("---print converted 1D array from C++---")
    print(out_1D[144:155])
    print(out_1D.dtype)
    print(out_1D.shape)
    target_1D = counts
    print("---print converted 1D array from Python---")    
    print(target_1D[144:155])
    print(target_1D.dtype)
    print(target_1D.shape)
    print(f"FINAL TEST: {np.all(np.isclose(out_1D,target_1D))}")
    a=1

    print("---FULL CHANNEL ARRAY---")      
    indexlist=np.load("datasets/ts2_submap_indexlist.npy")
    pxlen=np.load("datasets/ts2_submap_pxlen.npy")
    data=np.load("./datasets/ts2_submap_data.npy")

    if len(indexlist) != len(pxlen):
        raise ValueError("mismatch between index and pixel lengths")

    indexlist_flat=indexlist.flatten()
    data_flat=data.flatten()

    f="datasets/ts2_submap_buffer.bin"

    with open(f, mode='rb') as fi:
        stream = fi.read() 

    recieve=np.zeros((len(indexlist),NDET,NCHAN),dtype=np.uint16)

    print(indexlist.shape[0])

    parserout = np.zeros(NCHAN, dtype=np.uint16)

    print("---READ BUFFER---")
    parserout = parsercore.readbuffer(indexlist, pxlen, stream, len(stream))

    #print(indexlist[0:10])
    #print(pxlen[0:10])

    print("---END---")













if __name__ == "__main__":
    main()

    sys.exit()