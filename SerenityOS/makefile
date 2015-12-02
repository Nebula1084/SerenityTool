SASM	= SerenityASM.exe
SLINK	= SerenityLINK.exe
MV = copy
RM = del

ALL:FileSys.bit BankA.coe BankB.coe

BankA.coe BankB.coe:SerenityOS.bit
	python generateCoe.py SerenityOS.bit

SerenityOS.bit: bin\IntEntry.bit bin\SysEntry.bit bin\IVT.bit bin\TVT.bit bin\print.bit bin\printstr.bit  bin\putchar.bit bin\KBInt.bit bin\editor.bit bin\getchar.bit
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

FileSys.bit: bin\IntEntry.bit bin\SysEntry.bit \
	bin\IVT.bit bin\TVT.bit bin\print.bit bin\printstr.bit \
	bin\putchar.bit bin\KBInt.bit bin\filetest.bit bin\getchar.bit \
	bin\printint.bit bin\fopen.bit
	$(SLINK) -o FileSys.bit \
		bin\IntEntry.bit \
		bin\SysEntry.bit \
		bin\IVT.bit \
		bin\TVT.bit \
		bin\print.bit \
		bin\printstr.bit \
		bin\putchar.bit \
		bin\KBInt.bit \
		bin\printint.bit \
		bin\filetest.bit \
		bin\getchar.bit \
		bin\fopen.bit

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