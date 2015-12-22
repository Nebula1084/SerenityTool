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
        li      $v0, 8      #dir 33e
        syscall
        
        la      $a0, new_file
        li      $v0, 9      #fopen
        syscall
        
        li      $a0, 10
        la      $a1, out
        li      $v0, 10     #fread 35a
        syscall
        
        la      $a0, out
        li      $a1, 0
        li      $v0, 1
        syscall
        
        li      $v0, 13     #fclose
        syscall                      
		
        li      $a0, 0x61
        sh      $a0, 0x3070($zero)
        
        la      $a0, open_name
		li		$v0, 7
		syscall #37c
        li      $a0, 0x62
        sh      $a0, 0x3051($zero)
        
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