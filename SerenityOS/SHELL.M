        push    $ra
        		
        la		$a0, hello_world
		li		$a1, 2
		li		$v0, 1
		syscall   #314
        la      $a0, new_file
        li      $v0, 9      #fopen
        syscall     # 322
        
        li      $a0, 10
        la      $a1, text
        li      $v0, 11
        syscall             #fwrite 332
        
        li      $v0, 13     #fclose 338
        syscall 
        li      $a0, 0
        li      $v0, 8      #dir 33e
        syscall
        
        li      $a1, 10
        li      $a2, 10
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