import parsercore.parsercore_lib as parsercore
import parsercore.explore as explore
import numpy as np
import sys

#global flags
BYTEORDER='little'
NCHAN=4096
NDET=2
PXHEADERLEN=16

def parse( indexlist, pxlen, stream: bytes):
    """
    Primary wrapper for cpp parser, including basic safety checks
    """

    if BYTEORDER != sys.byteorder:
        raise EnvironmentError("FATAL: System byteorder differs from data byteorder, C++ pixel extraction will fail")

    if len(indexlist) != len(pxlen):
        raise ValueError("mismatch between index and pixel lengths")

    if indexlist.dtype != np.uint64 or pxlen.dtype != np.uint16:
        raise ValueError("invalid data types for index and/or pixel length ndarrays")

    #run the cpp parser
    result = parsercore.readstream(indexlist, pxlen, stream, len(stream))

    return result




def demo():
    """
    Demo for parser
        loads example data from ./datasets
        prints outputs
    """
    print("---LOADING DATA---")      
    indexlist=np.load("datasets/ts2_submap_indexlist.npy")
    pxlen=np.load("datasets/ts2_submap_pxlen.npy")
    data=np.load("./datasets/ts2_submap_data.npy")
    f="datasets/ts2_submap_buffer.bin"

    with open(f, mode='rb') as fi:
        stream = fi.read() 

    npx=len(indexlist)

    print(f"pixels loaded: {indexlist.shape[0]}")

    parserout = np.zeros(npx*NDET*NCHAN, dtype=np.uint16)

    print("---READ IN DATA---")
    parserout = parse(indexlist, pxlen, stream)

    print(f"expected:   {data[1000,0,140:160]}")
    print(f"result:     {parserout[1000,0,140:160]}")

    print(f"DATA COORECT: {np.all(np.isclose(parserout,data))}")

    print("---END---")


if __name__ == "__main__":
    demo()

    sys.exit()