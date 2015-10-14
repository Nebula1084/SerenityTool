Final: SASM SVM ST
	Copy "SerenityASM\SerenityASM.exe" "SerenityTest\SerenityASM.exe"
	Copy "SerenityVM\SerenityVM.exe" "SerenityTest\SerenityVM.exe"
SASM:
	mingw32-make -C SerenityASM
SVM:
	mingw32-make -C SerenityVM
ST:
	mingw32-make -C SerenityTest