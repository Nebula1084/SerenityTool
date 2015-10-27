SASM	= ..\SerenityTest\SerenityASM.exe
SLINK	= ..\SerenityTest\SerenityLINK.exe

SerenityOS.bit: bin\IntEntry.bit bin\SysEntry.bit bin\IVT.bit bin\TVT.bit bin\print.bit bin\printstr.bit bin\test.bit bin\putchar.bit
	$(SLINK) -o SerenityOS.bit \
		bin\IntEntry.bit \
		bin\SysEntry.bit \
		bin\IVT.bit \
		bin\TVT.bit \
		bin\print.bit \
		bin\printstr.bit \
		bin\test.bit \
		bin\putchar.bit

bin\IntEntry.bit: src\IntEntry.mips
	$(SASM) src\IntEntry.mips
	mv src\IntEntry.bit bin\IntEntry.bit
	
bin\SysEntry.bit: src\SysEntry.mips
	$(SASM) src\SysEntry.mips
	mv src\SysEntry.bit bin\SysEntry.bit

bin\IVT.bit: src\IVT.mips
	$(SASM) src\IVT.mips
	mv src\IVT.bit bin\IVT.bit

bin\TVT.bit: src\TVT.mips
	$(SASM) src\TVT.mips
	mv src\TVT.bit bin\TVT.bit
	
bin\print.bit: src\print.mips
	$(SASM) src\print.mips
	mv src\print.bit bin\print.bit
	
bin\printstr.bit: src\printstr.mips
	$(SASM) src\printstr.mips
	mv src\printstr.bit bin\printstr.bit
	
bin\putchar.bit: src\putchar.mips
	$(SASM) src\putchar.mips
	mv src\putchar.bit bin\putchar.bit
	
bin\test.bit: src\test.mips
	$(SASM) src\test.mips
	mv src\test.bit bin\test.bit
	
.PHONY: clean
clean:
	rm bin/*.bit *.bit