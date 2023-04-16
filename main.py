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

    print("---CHECKING ARRAY---")      
    indexlist=np.load("datasets/ts2_submap_indexlist.npy")
    pxlen=np.load("datasets/ts2_submap_pxlen.npy")
    data=np.load("./datasets/ts2_submap_data.npy")
    f="datasets/ts2_submap_buffer.bin"

    with open(f, mode='rb') as fi:
        stream = fi.read() 

    if len(indexlist) != len(pxlen):
        raise ValueError("mismatch between index and pixel lengths")

    npx=len(indexlist)

    print(indexlist.shape[0])

    parserout = np.zeros(npx*NDET*NCHAN, dtype=np.uint16)

    print("---READ IN DATA---")
    parserout = parsercore.readbuffer(indexlist, pxlen, stream, len(stream))

    print(parserout[1000,0,140:160])
    print(data[1000,0,140:160])

    print(f"DATA COORECT: {np.all(np.isclose(parserout,data))}")

    print("---END---")





if __name__ == "__main__":
    main()

    sys.exit()