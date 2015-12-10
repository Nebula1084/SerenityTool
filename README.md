
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
###Command m
	m address
would display memroy content from specified `address`.
###Command g
	g address
virtual machine would execute continuously until PC attains specified `address`.
###Command b
	b
In debug mode and execute continuously, the virtual machine would stop
when you hit key `b`.
##Display##
the Serenity VM support simple text mode, which support all `Zhe standard code`. The memory whose address is after `0x3000` is video memory, where we could write code and the screen would display correspondent character. By the way, the program could not exit but could be halted by `ctrl+c`.
##Virtual Disk
Now, Serenity VM support virtual disk functionality, which allow users access file.
###Memory Layout
~~~
50FC offset
50FD 
50FE command
50FF ready
5100 block buffer
51FF
~~~
###Command
Pass number of sector to 0x50FC which is the address of offset.
Send command to 0x50FE and the ready whose adress is 50FF would indicate the status of current
disk access
~~~
D_COMM_NONE 0
D_COMM_WRITE 1
D_COMM_READ 2

BUSY 0
READY 1
~~~

###Tips
use **sw** and **lw** instruction to access offset and use **sh** and **lh** access command. And
ready could only be accessed by **lh**.

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