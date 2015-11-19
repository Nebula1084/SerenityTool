b = bytearray(1)
with  open("Screen.bit","wb") as f:
    f.write(b'\x00\x00\x30\x00')
    for i in range(0x3000//4,(0x3000+1000)//4):
        str = '%04X'%(i*4)
        for i in range(4):
            f.write(b'\x00')
            b[0] = ord(str[i]);
            f.write(b)