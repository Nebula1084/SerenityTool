MipsSimulator : main.o MipsCPU.o MemoryManageUnit.o
	g++ main.o MipsCPU.o MemoryManageUnit.o -o SerenityVM
	del *.o
main.o:
	g++ src/main.cpp -c -Wall -Iinclude
MipsCPU.o: 
	g++ src/MipsCPU.cpp -c -Wall -Iinclude
MemoryManageUnit.o:
	g++ src/MemoryManageUnit.cpp -c -Wall -Iinclude
