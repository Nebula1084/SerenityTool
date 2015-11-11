SASM	= ..\SerenityTest\SerenityASM.exe
SLINK	= ..\SerenityTest\SerenityLINK.exe
MV = copy
RM = del

SerenityOS.bit: bin\IntEntry.bit bin\SysEntry.bit bin\IVT.bit bin\TVT.bit bin\print.bit bin\printstr.bit bin\test.bit bin\putchar.bit bin\KBInt.bit
	$(SLINK) -o SerenityOS.bit \
		bin\IntEntry.bit \
		bin\SysEntry.bit \
		bin\IVT.bit \
		bin\TVT.bit \
		bin\print.bit \
		bin\printstr.bit \
		bin\test.bit \
		bin\putchar.bit \
		bin\KBInt.bit

bin\IntEntry.bit: src\IntEntry.mips
	$(SASM) src\IntEntry.mips
	$(MV) src\IntEntry.bit bin\IntEntry.bit
	$(RM) src\IntEntry.bit
	
bin\SysEntry.bit: src\SysEntry.mips
	$(SASM) src\SysEntry.mips
	$(MV) src\SysEntry.bit bin\SysEntry.bit
	$(RM) src\SysEntry.bit

bin\IVT.bit: src\IVT.mips
	$(SASM) src\IVT.mips
	$(MV) src\IVT.bit bin\IVT.bit
	$(MV) src\IVT.bit 

bin\TVT.bit: src\TVT.mips
	$(SASM) src\TVT.mips
	$(MV) src\TVT.bit bin\TVT.bit
	$(RM) src\TVT.bit 
	
bin\print.bit: src\print.mips
	$(SASM) src\print.mips
	$(MV) src\print.bit bin\print.bit
	$(RM) src\print.bit 
	
bin\printstr.bit: src\printstr.mips
	$(SASM) src\printstr.mips
	$(MV) src\printstr.bit bin\printstr.bit
	$(RM) src\printstr.bit 
	
bin\putchar.bit: src\putchar.mips
	$(SASM) src\putchar.mips
	$(MV) src\putchar.bit bin\putchar.bit
	$(RM) src\putchar.bit 
	
bin\test.bit: src\test.mips
	$(SASM) src\test.mips
	$(MV) src\test.bit bin\test.bit
	$(RM) src\test.bit 
	
bin\KBInt.bit: src\KBInt.mips
	$(SASM) src\KBInt.mips
	$(MV) src\KBInt.bit bin\KBInt.bit
	$(RM) src\KBInt.bit 
	
.PHONY: clean
clean:
	$(RM) bin/*.bit *.bit