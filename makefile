Final: SASM SVM
	mv "SerenityASM\SerenityASM.exe" "SerenityTest\SerenityASM.exe"
	mv "SerenityVM\SerenityVM.exe" "SerenityTest\SerenityVM.exe"
SASM:
	mingw32-make -C SerenityASM
SVM:
	mingw32-make -C SerenityVM