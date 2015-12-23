SerenityASM.exe src\cmd.mips
SerenityASM.exe src\editor.mips
copy src\cmd.bit H:\SHELL.BIT /Y
copy src\editor.bit H:\EDIT.BIT /Y
cd ..
mingw32-make 
cd SerenityOS
soft-OS.bat