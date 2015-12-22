SerenityASM.exe src\cmd.mips
move src\cmd.bit H:\SHELL.BIT
cd ..
mingw32-make 
cd SerenityOS
soft-OS.bat
test.bat
