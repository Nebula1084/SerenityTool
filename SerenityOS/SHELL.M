		la		$a0, hello_world
		li		$a1, 2
		li		$v0, 1
		syscall
        
loop:   jd      loop

hello_world:
		.asciiz	"Hello World!"