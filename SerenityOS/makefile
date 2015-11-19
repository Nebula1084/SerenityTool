SASM	= SerenityASM.exe
SLINK	= SerenityLINK.exe
MV = copy
RM = del

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

bin\\%.bit: src\%.mips
	$(SASM)	$<
	$(MV)	src\*.bit bin\*.bit
	$(RM)	src\*.bit
	
.PHONY: clean
clean:
	$(RM) bin/*.bit *.bit