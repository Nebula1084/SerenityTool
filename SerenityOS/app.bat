SerenityASM.exe SHELL.M
copy SHELL.bit app\SHELL.BIT /Y
del SHELL.bit

SerenityASM.exe editor.mips
copy editor.bit app\EDIT.BIT /Y
del editor.bit