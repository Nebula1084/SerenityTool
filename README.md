# SerenityVM #
##Build##
windows:<br>
`build.bat`<br>
##Instruction##
currently, our VM only could execute following instructions:<br>
`ADD $rd,$rs,$rt`<br>
`SUB $rd,$rs,$rt`<br>
`SLL $rd,$rs,$rt`<br>
`SRL $rd,$rs,$rt`<br>
`SRA $rd,$rs,$rt`<br>
`JR $rs`<br>
`JALR $rs,$rd`<br>
`LW $rt,data($rs)`<br>
`SW $rt,data($rs)`<br>
`LH $rt,data($rs)`<br>
`SH $rt,data($rs)`<br>
`BEQ $rs,$rt,data`<br>
`J address`<br>
`JAL address`<br>
`ADDI $rt,$rs,data`<br>

##Usage##
The Virtual machine could be used in following way.<br>
`SerenityVM.exe file`<br>
For instance, insrtuction `SerenityVM.exe boot.bit` would execute the binary file `boot.bit`. And the using option `-d` could specify the debug mode, like `SerenityVM.exe -d boot.bit`.
##Display##
the Serenity VM support simple text mode, which support all `Zhe standard code`. The memory whose address is after `0x3000` is video memory, where we could write code and the screen would display correspondent character. By the way, the program could not exit but could be halted by `ctrl+c`.


# SerenityASM #
##Usage##
The Assembler could be used in following way.<br>
`SerenityASM.exe file`<br>
For instance, insrtuction `SerenityASM.exe boot.txt` would compile codes in text file `boot.txt` into binary format `boot.bit`.
##Instruction##
Most standard instruction is supported currently, including `add, addu, and, jalr, jr, mfhi, mflo, nor, or, sll, sllv, slt, sltu, sra, srav, xor, bne, beq, andi, ori, xori, mfc0, mtc0, lui`
<br>
##Pseudo instruction##
Currently, we support a little bit pseudo instruction, which are `move`, `push`, `pop`


# SerenityTest
After build, `SerenityASM.exe` and `SerenityVM.exe` would be under the directory `SerenityTool/SerrentiyTest`. And user could them it to develop `MIPS` project. 