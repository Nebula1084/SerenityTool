EFILE		= SerenityASM.exe
RM			= del
GCC			= g++
SRC_DIR 	= src
INCLUDE_DIR = include
BIN_DIR 	= bin
OBJS		= $(BIN_DIR)/main.o $(BIN_DIR)/Assembler.o $(BIN_DIR)/AssembleInfo.o $(BIN_DIR)/Instruction.o $(BIN_DIR)/PseudoInstruction.o $(BIN_DIR)/FormatInstruction.o $(BIN_DIR)/RegInstruction.o $(BIN_DIR)/ImmInstruction.o $(BIN_DIR)/JumpInstruction.o $(BIN_DIR)/LoadSaveInstruction.o $(BIN_DIR)/SpcInstruction.o $(BIN_DIR)/global.o

$(EFILE): $(OBJS)
	$(GCC) -o $(EFILE) $(OBJS)
$(BIN_DIR)/main.o: 	$(INCLUDE_DIR)/Assembler.hpp $(SRC_DIR)/main.cpp
	$(GCC) -c -o $(BIN_DIR)/main.o $(SRC_DIR)/main.cpp -I$(INCLUDE_DIR)
$(BIN_DIR)/Assembler.o:	$(INCLUDE_DIR)/Assembler.hpp \
	$(INCLUDE_DIR)/RegInstruction.hpp \
	$(INCLUDE_DIR)/JumpInstruction.hpp \
	$(INCLUDE_DIR)/ImmInstruction.hpp \
	$(INCLUDE_DIR)/SpcInstruction.hpp \
	$(INCLUDE_DIR)/LoadSaveInstruction.hpp \
	$(INCLUDE_DIR)/PseudoInstruction.hpp \
	$(INCLUDE_DIR)/FormatInstruction.hpp \
	$(INCLUDE_DIR)/global.hpp \
	$(SRC_DIR)/Assembler.cpp
	$(GCC) -c -o $(BIN_DIR)/Assembler.o $(SRC_DIR)/Assembler.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/AssembleInfo.o: 	$(INCLUDE_DIR)/AssembleInfo.hpp $(SRC_DIR)/AssembleInfo.cpp
	$(GCC) -c -o $(BIN_DIR)/AssembleInfo.o $(SRC_DIR)/AssembleInfo.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/Instruction.o: 	$(INCLUDE_DIR)/Instruction.hpp $(INCLUDE_DIR)/global.hpp \
	$(INCLUDE_DIR)/AssembleInfo.hpp \
	$(SRC_DIR)/Instruction.cpp
	$(GCC) -c -o $(BIN_DIR)/Instruction.o $(SRC_DIR)/Instruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/PseudoInstruction.o:	$(INCLUDE_DIR)/PseudoInstruction.hpp \
	$(INCLUDE_DIR)/RegInstruction.hpp \
	$(INCLUDE_DIR)/ImmInstruction.hpp \
	$(INCLUDE_DIR)/LoadSaveInstruction.hpp \
	$(INCLUDE_DIR)/JumpInstruction.hpp \
	$(INCLUDE_DIR)/global.hpp \
	$(SRC_DIR)/PseudoInstruction.cpp
	$(GCC) -c -o $(BIN_DIR)/PseudoInstruction.o $(SRC_DIR)/PseudoInstruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/FormatInstruction.o: $(INCLUDE_DIR)/FormatInstruction.hpp \
	$(SRC_DIR)/FormatInstruction.cpp
	$(GCC) -c -o $(BIN_DIR)/FormatInstruction.o $(SRC_DIR)/FormatInstruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/RegInstruction.o: 	$(INCLUDE_DIR)/RegInstruction.hpp  $(SRC_DIR)/RegInstruction.cpp
	$(GCC) -c -o $(BIN_DIR)/RegInstruction.o $(SRC_DIR)/RegInstruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/ImmInstruction.o: 	$(INCLUDE_DIR)/ImmInstruction.hpp \
	$(SRC_DIR)/ImmInstruction.cpp
	$(GCC) -c -o $(BIN_DIR)/ImmInstruction.o $(SRC_DIR)/ImmInstruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/JumpInstruction.o:	$(INCLUDE_DIR)/JumpInstruction.hpp \
	$(INCLUDE_DIR)/global.hpp \
	$(SRC_DIR)/JumpInstruction.cpp
	$(GCC) -c -o $(BIN_DIR)/JumpInstruction.o $(SRC_DIR)/JumpInstruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/LoadSaveInstruction.o: 	$(INCLUDE_DIR)/LoadSaveInstruction.hpp \
	$(SRC_DIR)/LoadSaveInstruction.cpp
	$(GCC) -c -o $(BIN_DIR)/LoadSaveInstruction.o $(SRC_DIR)/LoadSaveInstruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/SpcInstruction.o:	$(INCLUDE_DIR)/SpcInstruction.hpp \
	$(INCLUDE_DIR)/AssembleInfo.hpp \
	$(SRC_DIR)/SpcInstruction.cpp
	$(GCC) -c -o $(BIN_DIR)/SpcInstruction.o $(SRC_DIR)/SpcInstruction.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/global.o:	$(INCLUDE_DIR)/global.hpp \
	$(SRC_DIR)/global.cpp
	$(GCC) -c -o $(BIN_DIR)/global.o $(SRC_DIR)/global.cpp -I$(INCLUDE_DIR)



.PHONY: clean
clean:
	$(RM) $(BIN_DIR)\*.o $(EFILE)
