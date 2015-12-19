SASM    = SerenityASM.exe
SLINK   = SerenityLINK.exe
MV = copy
RM = del

ALL:FileSys.bit BankA.coe BankB.coe

BankA.coe BankB.coe: SerenityOS.bit
	python generateCoe.py SerenityOS.bit

SerenityOS.bit: bin\IntEntry.bit bin\IVT.bit bin\TVT.bit bin\KBInt.bit bin\test.bit bin\Hardgetchar.bit bin\HardKBInt.bit bin\TimerInt.bit
	$(SLINK) -o SerenityOS.bit \
		bin\IntEntry.bit \
		bin\IVT.bit \
		bin\TVT.bit \
		bin\HardKBInt.bit \
		bin\test.bit \
		bin\Hardgetchar.bit \
		bin\TimerInt.bit

FileSys.bit: bin\IntEntry.bit\
	bin\IVT.bit bin\TVT.bit\
	bin\boot.bit	\
	bin\KBInt.bit \
	bin\getchar.bit \
	bin\clear.bit

	$(SLINK) -o FileSys.bit \
		bin\IntEntry.bit \
		bin\IVT.bit \
		bin\TVT.bit \
		bin\KBInt.bit \
		bin\getchar.bit \
		bin\clear.bit \
		bin\boot.bit

bin\\%.bit: src\%.mips
	$(SASM) $<
	$(MV)   src\*.bit bin\*.bit
	$(RM)   src\*.bit

.PHONY: clean
clean:
	$(RM) bin\*.bit *.bit