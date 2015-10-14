main: assemble
	g++ -c src/main.cpp -Iinclude
	g++ AssembleInfo.o Assembler.o AssemblerLine.o main.o -o SerenityASM 
	del *.o

assemble:
	g++ -c src/AssembleInfo.cpp -Iinclude
	g++ -c src/Assembler.cpp -Iinclude
	g++ -c src/AssemblerLine.cpp -Iinclude