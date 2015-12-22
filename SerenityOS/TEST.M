        push    $ra
        		
        la		$a0, hello_world
		li		$a1, 2
		li		$v0, 1
		syscall   #314
        
sus:    jd      sus             

hello_world:
		.asciiz	"He~~~2!"
open_name:
        .asciiz "TEST    BIT"
new_file:
        .asciiz "F12     TXT"
text:
        .asciiz "1234567890"
out:    .space 20