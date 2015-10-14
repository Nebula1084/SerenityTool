# SerenityVM #
##Build##
windows:<br>
`build.bat`<br>
##Instruction##
currently, our VM only could execute following instructions:<br>
`add,sub,lw,sw,lh,sh,beq,j`
##Running##
executable will prompt opening the binary file, chosen by user, which would be executed by VM subsequently. `boot.txt` is MIPS assembly code and `boot.bit` is executable which has been assembled, besides, could be used as test file. 	
##Display##
the Serenity VM support simple text mode, which support all `Zhe standard code`. The memory whose address is after `0xc00` is video memory, where we could write code and the screen would display correspondent character. By the way, the program could not exit but could be halted by `ctrl+c`.


# SerenityASM #
##Build##
for windows user<br>
  `build.bat`<br>
for Unix user<br>
  `./build.sh`<br>
##Instruction##
Most standard instruction is supported currently, including `add, addu, and, jalr, jr, mfhi, mflo, nor, or, sll, sllv, slt, sltu, sra, srav, xor, bne, beq, andi, ori, xori, mfc0, mtc0, lui`
<br>
##Pseudo instruction##
Currently, we support a little bit pseudo instruction, which are `move`, `push`, `pop`
<br>
We do support instruction heading by `.` and `syscall` temporarily.

# SerenityTool
After build, `SerenityASM.exe` and `SerenityVM.exe` would be under the directory `SerenityTool`. And user could them it to develop `MIPS` project. 