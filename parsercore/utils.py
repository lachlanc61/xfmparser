#common to all, should definitely be in a yaml or similar
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