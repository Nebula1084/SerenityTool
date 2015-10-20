MipsAssembler: main.o Assembler.o AssembleInfo.o Instructions.o global.o
	g++ -o Assembler main.o Assembler.o AssembleInfo.o Instruction.o PseudoInstruction.o FormatInstruction.o RegInstruction.o ImmInstruction.o JumpInstruction.o LoadSaveInstruction.o SpcInstruction.o global.o
main.o:
	g++ -c src/main.cpp -Iinclude
Assembler.o:
	g++ -c src/Assembler.cpp -Iinclude
AssembleInfo.o:
	g++ -c src/AssembleInfo.cpp -Iinclude
Instructions.o:
	g++ -c src/Instruction.cpp -Iinclude
	g++ -c src/PseudoInstruction.cpp -Iinclude
	g++ -c src/FormatInstruction.cpp -Iinclude
	g++ -c src/RegInstruction.cpp -Iinclude
	g++ -c src/ImmInstruction.cpp -Iinclude
	g++ -c src/JumpInstruction.cpp -Iinclude
	g++ -c src/LoadSaveInstruction.cpp -Iinclude
	g++ -c src/SpcInstruction.cpp -Iinclude
global.o:
	g++ -c src/global.cpp -Iinclude
