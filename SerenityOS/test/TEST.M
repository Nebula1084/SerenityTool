        push    $ra
        		
        move    $a0, $v1        
		li		$a1, 3
        li      $a2, 3
		li		$v0, 4
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