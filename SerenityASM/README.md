#版本信息
指令格式、功能等信息详见[ZPC之MIPS指令集.pdf](ZPC之MIPS指令集.pdf)（口令：lou）

###Version 2.7
1. 新增了格式指令equ，equ不一定要在汇编文件开头，且该行不会出现在bit文件中

###Version 2.6.1
1. 去除了错误的协寄存器名$c32
2. 解决了li、la伪指令当立即数或地址能用16位表示时，新增的空指令出现的BUG
3. 输出bit文件改为二进制写

###Version 2.6
1. 行数修正，起始编号从0开始，而不是1，“.origin”不占行
2. “.asciiz”指令后的每个字符由32位变为16位
3. “li”和“la”变成稳定转换成两条标准指令，当立即数或绝对地址能用16位表示时，原指令后增加一条空指令add $s0,$s0,$zero

###Version 2.5
1. 相对寻址BUG修正

###Version 2.4
1. 解决了一些BUG
2. ".origin"后现在只能跟偶数地址，否则报错

###Version 2.3
1. 绝对寻址BUG修正

###Version 2.2
1. 修正了一些BUG

###Version 2.1
1. jump指令更正
2. 立即数相关问题的解决
3. 新格式指令".space"加入

###Version 2.0
1. 在Version1.0的基础上进行了大量优化和更正
2. 支持的指令

		2.1 ImmIns
		 	支持全8条指令
		 	addi、addiu、slti、sltiu、andi、ori、xori、lui
		 	addi和stliu中imme当做无符号16位处理
		2.2 SpcIns
		 	支持eret、mul、mtc0、mfc0，不支持mtc1、mfc1
		 	mtc0、mfc0的opcode为010000
		 	32个协寄存器依次命名位$c0-$c31
		2.3 LoadSaveIns
		 	支持全8条指令			lb、lh、lw、lbu、lhu、sb、sh、sw
		2.4 JumpIns
		 	支持全10条指令
		 	bge、bgezal、bltz、j、jal、
		 	beq、bne、blez、blezal、bgtz
		2.5 RegIns
		 	支持全27条指令
		 	syscall、jr、mthi、mtlo、mfhi、mflo、jalr、			mult、multu、div、divu、sra、sll、sllv、srav、			srl、srlv、add、addu、sub、subu、slt、sltu、			and、nor、or、xor
		2.6 Pseudo
		 	支持全15条指令
		 	move、not、neg、push、pop、blt、bgt、ble、bge、			abs、swap、sne、seq、li、la、			但push和pop的参数只能有一个
		2.7 FormatIns
			格式指令指令.origin、.2byte、.asciiz
			- .origin必须在汇编文件开头，若不加.origin，默认为
			0，.orgin后的地址会在bit文件第一行出现
			- .2byte和.asciiz后的字符串只由双引号配对标识，字符
			支持的反斜杠转义包括\n，\t，\r，\”，\\，此外转义符后
			加其他字符的情况，自动忽略该转义符
			- .2byte后字符和字符串军用双引号而不用单引号
3. 报错功能

	支持报错功能，报错时以行为单位，每次只能报一行的一种错误。<br>	报错时输出如下：<br>	Line "lineNumber" : "errorInfo" <br>	lineNumber为出错的汇编码行号（伪指令展开前，空行也算行） <br>	errorInfo为错误信息，有以下几种 <br>
			1)	No error 没有错误，前面不会有行号		2)	Wrong formation 各种格式错误，如除eret和syscall外的指令只有指令名而无操作数；指令名和操作数间出现逗号,如”add,$s1,$s2,$s3”；操作数与操作数间无分隔符”,“或括号，如”add $s1 $s2 $s3”；重复的分隔符，如”add $s1,,$s2,$s3”；逗号出现在括号间，如”lw $s1,0(,$s2)”；	括号不匹配或出现多于一个括号”lw$s1,0(($s2)”；括号间无操作数，如”lw $s1,0,()”；逗号后无操作数,如“add $s1,$s2,$s3,”……		3)	Illegal characters in label label中有非法字符（合法字符为大小写字母、数字和下划线）		4)	Label duplication 连续的label，即两个label连在一起，如：		Line 0：RR		Line 1：RRR syscall		报错为：		Line 1：RRR syscall		Label duplication		5)	The amount of operand is wrong 操作数的数量错误，如add指令本应恰有3个操作数，因此add $s1,$s2，操作数的数量错误		6)	No such register 没有这样的寄存器，如某行若有非法寄存器名“$r1”，改行则会报此类错误		7)	No such instruction or pseudo instruction 没有这样的指令或伪指令，即指令名不合法或不存在		8)	Redefined label 重定义的label，当同一个label出现第二次时在第二次那行报错，如		Line 0: RR: add $s0,$s1,$s2		……		Line 3: RR: sw $s0,0($s2)		报错为：		Line 3: RR: sw $s0,0($s2)		No such register		9)	Wrong immediate number or offset 错误（超过位数或出现非法字符）的立即数或偏移（目前仅在li指令中报此类错误）		10)	No such label 没有这样的label，即找不到在跳转类指令中的label		11)	Illegal origin address  origin后的地址不合法		12)	reduplicated .origin 重复的.origin，规定一个汇编文件只能有一个.orgin		13)	Can not assemble 其余已被发现的错误		14)	Unknown error 未知错误

4. 注释 <br>
	支持注释，以“#”为关键字，允许一行只有注释
5. 进制 <br>
	支持十进制、二进制和十六进制 <br>	十进制以”#”开头 <br>	二进制以”B”开头 <br>	十六进制以“0x”开头 <br>	默认为十进制 <br>
6. 大小写 <br>
    整个汇编器不区分大小写
7. 表达式 <br>
	不支持表达式	 	 	

###Version 1.0
