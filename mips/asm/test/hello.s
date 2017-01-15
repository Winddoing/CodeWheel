
#    text segment 
    .text        
    .globl main
main:        		# execution starts here
    la $a0,str    	# put string address into a0
    li $v0,4    	# system call to print
    syscall        	# out a string
    li $v0,10
    syscall        	# exit
#data segment
    .data
str:    .asciiz "hello world/n"

