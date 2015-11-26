EFILE		= SerenityVM.exe
RM			= del
GCC			= g++
SRC_DIR		= src
INCLUDE_DIR	= include
BIN_DIR		= bin
OBJS		= $(BIN_DIR)/main.o $(BIN_DIR)/MipsCPU.o $(BIN_DIR)/MemoryManageUnit.o \
		$(BIN_DIR)/SysPara.o $(BIN_DIR)/KeyCode.o $(BIN_DIR)/Log.o

$(EFILE): $(OBJS)
	$(GCC) -o $(EFILE) $(OBJS)

$(BIN_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/MipsCPU.h 
	$(GCC) -c -o $(BIN_DIR)/main.o $(SRC_DIR)/main.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/MipsCPU.o: $(SRC_DIR)/MipsCPU.cpp $(INCLUDE_DIR)/MipsCPU.h $(INCLUDE_DIR)/MemoryManageUnit.h $(INCLUDE_DIR)/SysPara.h
	$(GCC) -c -o $(BIN_DIR)/MipsCPU.o $(SRC_DIR)/MipsCPU.cpp -I$(INCLUDE_DIR)
	
$(BIN_DIR)/MemoryManageUnit.o: $(SRC_DIR)/MemoryManageUnit.cpp $(INCLUDE_DIR)/MemoryManageUnit.h
	$(GCC) -c -o $(BIN_DIR)/MemoryManageUnit.o $(SRC_DIR)/MemoryManageUnit.cpp -I$(INCLUDE_DIR)

$(BIN_DIR)/SysPara.o: $(SRC_DIR)/SysPara.cpp $(INCLUDE_DIR)/SysPara.h
	$(GCC) -c -o $(BIN_DIR)/SysPara.o $(SRC_DIR)/SysPara.cpp -I$(INCLUDE_DIR)
	
$(BIN_DIR)/KeyCode.o: $(SRC_DIR)/KeyCode.cpp $(INCLUDE_DIR)/KeyCode.h
	$(GCC) -c -o $(BIN_DIR)/KeyCode.o $(SRC_DIR)/KeyCode.cpp -I$(INCLUDE_DIR)
	
$(BIN_DIR)/Log.o: $(SRC_DIR)/Log.cpp $(INCLUDE_DIR)/Log.h
	$(GCC) -c -o $(BIN_DIR)/Log.o $(SRC_DIR)/Log.cpp -I$(INCLUDE_DIR)