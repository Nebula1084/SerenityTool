SerenityOS.bit: IntEntry.bit SysEntry.bit IVT.bit TVT.bit
	SerenityLINK.exe -o SerenityOS.bit \
		IntEntry.bit \
		SysEntry.bit \
		IVT.bit \
		TVT.bit

IntEntry.bit: IntEntry.mips
	SerenityASM.exe IntEntry.mips
	
SysEntry.bit: SysEntry.mips
	SerenityASM.exe SysEntry.mips

IVT.bit: IVT.mips
	SerenityASM.exe IVT.mips

TVT.bit: TVT.mips
	SerenityASM.exe TVT.mips
	