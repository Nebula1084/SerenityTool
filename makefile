Final: SASM SVM SLINK
	mv "SerenityASM\SerenityASM.exe" "SerenityTest\SerenityASM.exe"
	mv "SerenityVM\SerenityVM.exe" "SerenityTest\SerenityVM.exe"
	mv "SerenityLINK\SerenityLINK.exe" "SerenityTest\SerenityLINK.exe"
SASM:
	mingw32-make -C SerenityASM
SVM:
	mingw32-make -C SerenityVM
SLINK:
	mingw32-make -C SerenityLINK