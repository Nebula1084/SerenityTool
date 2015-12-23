        push    $ra
        		
        la		$a0, hello_world
		li		$a1, 2
		li		$v0, 1
		syscall   #314
        la      $a0, open_name
        li      $v0, 9      #fopen
        syscall     # 322
        
        li      $v0, 16
        syscall
        
        move    $a0, $v0
        li      $a1, 1
        li      $a2, 1
        li      $v0, 4
        syscall
        

sus:    jd      sus             

hello_world:
		.asciiz	"He~~~!"
open_name:
        .asciiz "EDIT    BIT"
new_file:
        .asciiz "F12     TXT"
text:
        .asciiz "1234567890"
out:    .space 20