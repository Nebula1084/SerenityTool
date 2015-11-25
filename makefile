MV = copy
RM = del

Final: SASM SVM SLINK
	$(MV) "SerenityASM\SerenityASM.exe" "SerenityOS\SerenityASM.exe"
	$(MV) "SerenityVM\SerenityVM.exe" "SerenityOS\SerenityVM.exe"
	$(MV) "SerenityVM\SVD.vhd" "SerenityOS\SVD.vhd"
	$(MV) "SerenityLINK\SerenityLINK.exe" "SerenityOS\SerenityLINK.exe"
	$(MV) "SerenityASM\SerenityASM.exe" "SerenityTest\SerenityASM.exe"
	$(MV) "SerenityVM\SerenityVM.exe" "SerenityTest\SerenityVM.exe"
	$(MV) "SerenityVM\SVD.vhd" "SerenityTest\SVD.vhd"
	$(MV) "SerenityLINK\SerenityLINK.exe" "SerenityTest\SerenityLINK.exe"
SASM:
	mingw32-make -C SerenityASM
SVM:
	mingw32-make -C SerenityVM
SLINK:
	mingw32-make -C SerenityLINK