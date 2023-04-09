#import build.xfmcparser as cparser

f="datasets/px14387_1_data.bin"

with open(f, mode='rb') as fi:
    stream = fi.read() 

print(type(stream))
print(stream[0:100])