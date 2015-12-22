SerenityASM.exe SHELL.M
copy SHELL.bit app\SHELL.BIT
del SHELL.bit

SerenityASM.exe TEST.M
copy TEST.BIT app\TEST.BIT
del TEST.BIT

SerenityASM.exe editor.mips
copy editor.bit app\EDIT.BIT
del editor.bit