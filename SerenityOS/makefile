SASM	= SerenityASM.exe
SLINK	= SerenityLINK.exe
MV = copy
RM = del

SerenityOS.bit: IntEntry.bit SysEntry.bit IVT.bit TVT.bit print.bit printstr.bit  putchar.bit KBInt.bit editor.bit getchar.bit CPY DELETE
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

%.bit: src\%.mips
	$(SASM)	$<

CPY: 
	$(MV)	src\*.bit bin\*.bit

DELETE:
	$(RM)	src\*.bit

.PHONY: clean
clean:
	$(RM) bin/*.bit *.bit