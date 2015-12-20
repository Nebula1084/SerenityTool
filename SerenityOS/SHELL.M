        push    $ra
        		
        la		$a0, hello_world
		li		$a1, 2
		li		$v0, 1
		syscall
        
        pop     $ra
        jr      $ra        
		
        la      $a0, open_name
		li		$v0, 7
		syscall

hello_world:
		.asciiz	"Hello World!"
open_name:
        .asciiz "EDIT    BIT"