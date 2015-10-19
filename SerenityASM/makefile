MipsAssembler: main.o Assembler.o AssembleInfo.o Instructions.o global.o
	gcc -o Assembler main.o Assembler.o AssembleInfo.o Instruction.o PseudoInstruction.o FormatInstruction.o RegInstruction.o ImmInstruction.o JumpInstruction.o LoadSaveInstruction.o SpcInstruction.o global.o 
main.o:
	gcc -c src/main.cpp -Iinclude
Assembler.o:
	gcc -c src/Assembler.cpp -Iinclude
AssembleInfo.o:
	gcc -c src/AssembleInfo.cpp -Iinclude
Instructions.o:
	gcc -c src/Instruction.cpp -Iinclude
	gcc -c src/PseudoInstruction.cpp -Iinclude
	gcc -c src/FormatInstruction.cpp -Iinclude
	gcc -c src/RegInstruction.cpp -Iinclude
	gcc -c src/ImmInstruction.cpp -Iinclude
	gcc -c src/JumpInstruction.cpp -Iinclude
	gcc -c src/LoadSaveInstruction.cpp -Iinclude
	gcc -c src/SpcInstruction.cpp -Iinclude
global.o:
	gcc -c src/global.cpp -Iinclude
