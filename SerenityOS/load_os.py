import struct

fdisk = open("SVD.vhd", "rb+")
fos = open("OS.bit", "rb")

fdisk.seek(130*512)

instruction = fos.read(4)

while True:	
	instruction = fos.read(4)
	if instruction=="":
		break	
	fdisk.write(instruction)

fos.close()
fdisk.close()