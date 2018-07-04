#=median(A1:E5)
.data
	Array: 	.word 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5
	             #A1,A2,A3,A4,A5,B1,B2,...
.text
main:
#1. ordena vector: https://stackoverflow.com/questions/19212544/sorting-array-in-mips-assembly
    la  $t0, Array      # Copy the base address of your array into $t1
    add $t0, $t0, 100    # 4 bytes per int * 25 ints = 100 bytes                              
outterLoop:             # Used to determine when we are done iterating over the Array
    add $t1, $0, $0     # $t1 holds a flag to determine when the list is sorted
    la  $a0, Array      # Set $a0 to the base address of the Array
innerLoop:                  # The inner loop will iterate over the Array checking if a swap is needed
    lw  $t2, 0($a0)         # sets $t0 to the current element in array
    lw  $t3, 4($a0)         # sets $t1 to the next element in array
    slt $t5, $t2, $t3       # $t5 = 1 if $t0 < $t1
    beq $t5, $0, continue   # if $t5 = 1, then swap them
    add $t1, $0, 1          # if we need to swap, we need to check the list again
    sw  $t2, 4($a0)         # store the greater numbers contents in the higher position in array (swap)
    sw  $t3, 0($a0)         # store the lesser numbers contents in the lower position in array (swap)
continue:
    addi $a0, $a0, 4       # advance the array to start at the next location from last time
    bne  $a0, $t0, innerLoop    # If $a0 != the end of Array, jump back to innerLoop
    bne  $t1, $0, outterLoop    # $t1 = 1, another pass is needed, jump back to outterLoop
	
#2. obtiene valor del centro

	la 	$t0, Array
	add $t0, $t0, 48 #48 = 4*12. En la posición 13 está la mediana. 
	
#3. Imprime resultado en pantalla
	li 	$v0, 1
	lw 	$a0, ($t0)
	syscall

#4. Salir
	li 	$v0, 10
	syscall
	
