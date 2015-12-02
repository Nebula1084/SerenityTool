SASM	= SerenityASM.exe
SLINK	= SerenityLINK.exe
MV = copy
RM = del

BankA.coe BankB.coe:SerenityOS.bit
	python generateCoe.py SerenityOS.bit

SerenityOS.bit: bin\IntEntry.bit bin\SysEntry.bit bin\IVT.bit bin\TVT.bit bin\print.bit bin\printstr.bit  bin\putchar.bit bin\HardKBInt.bit bin\editor.bit bin\getchar.bit bin\Screen.bit
	$(SLINK) -o SerenityOS.bit \
		bin\IntEntry.bit \
		bin\SysEntry.bit \
		bin\IVT.bit \
		bin\TVT.bit \
		bin\print.bit \
		bin\printstr.bit \
		bin\putchar.bit \
		bin\HardKBInt.bit \
		bin\editor.bit \
		bin\getchar.bit \
		bin\Screen.bit

bin\\%.bit: src\%.mips
	$(SASM)	$<
	$(MV)	src\*.bit bin\*.bit
	$(RM)	src\*.bit

bin\Screen.bit: 
	python generateScreenMem.py
	$(MV)	Screen.bit bin\Screen.bit
	$(RM)	Screen.bit

.PHONY: clean
clean:
	$(RM) bin\*.bit *.bit