SASM	= SerenityASM.exe
SLINK	= SerenityLINK.exe
MV = copy
RM = del

ALL: SerenityOS.bit FileSys.bit

SerenityOS.bit: bin\IntEntry.bit bin\SysEntry.bit bin\IVT.bit bin\TVT.bit bin\print.bit bin\printstr.bit  bin\putchar.bit bin\KBInt.bit bin\editor.bit bin\getchar.bit
	$(SLINK) -o SerenityOS.bit \
		bin\IntEntry.bit \
		bin\SysEntry.bit \
		bin\IVT.bit \
		bin\TVT.bit \
		bin\print.bit \
		bin\printstr.bit \
		bin\putchar.bit \
		bin\KBInt.bit \
		bin\editor.bit \
		bin\getchar.bit

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
	
.PHONY: clean
clean:
	$(RM) bin\*.bit *.bit