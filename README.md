
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
`BNE $rs,$rt,data`<br>
`J address`<br>
`JAL address`<br>
`ADDI $rt,$rs,data`<br>

~~~
up 172
down 180
left 175
right 177
~~~ 

##Error Hint
When virtual machine execute a instruction which is illegal, the virtual machine would pause and prompt the code that would be executed.

##Usage##
The Virtual machine could be used in following way.<br>
`SerenityVM.exe file`<br>
For instance, insrtuction `SerenityVM.exe boot.bit` would execute the binary file `boot.bit`. And the using option `-d` could specify the debug mode, like `SerenityVM.exe -d boot.bit`.
##Display##
the Serenity VM support simple text mode, which support all `Zhe standard code`. The memory whose address is after `0x3000` is video memory, where we could write code and the screen would display correspondent character. By the way, the program could not exit but could be halted by `ctrl+c`.
##Virtual Disk
Now, Serenity VM support virtual disk functionality, which allow users access file.
~~~
Read:
Pass number of sector to 0x5001
Send command to 0x5002
Once the transfer is finished, the 0x5003 would be 1. 
And the content of specified sector would be in 0x5100.

Write:
Pass number of sector to 0x5001
Send command to 0x5002
The content in 0x5100 would be flushed into specified sector

Command
D_COMM_NONE 0
D_COMM_WRITE 1
D_COMM_READ 2
~~~

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
After build, `SerenityASM.exe` and `SerenityVM.exe` would be under the directory `SerenityTool/SerrentiyTest`. And user could them it to develop `MIPS` project.1