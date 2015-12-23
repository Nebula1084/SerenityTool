SerenityASM.exe src\cmd.mips
move src\cmd.bit F:\SHELL.BIT \y
SerenityASM.exe src\editor.mips
move src\editor.bit F:\EDIT.BIT \y
cd ..
mingw32-make 
cd SerenityOS
soft-OS.bat
test.bat
