		move	$gp, $zero
		li		$sp, 0x2FFE
		la		$a0, hello_world
		li		$a1, 2
		li		$v0, 1
		syscall
		
exit:	beq		$zero, $zero, exit

hello_world:
		.asciiz	"Hello World!"