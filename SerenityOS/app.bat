SerenityASM.exe cmd.mips
copy cmd.bit app\SHELL.BIT /Y
del cmd.bit

SerenityASM.exe editor.mips
copy editor.bit app\EDIT.BIT /Y
del editor.bit