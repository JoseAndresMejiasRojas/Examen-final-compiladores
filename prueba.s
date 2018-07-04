# Esta prueba lo que hace es guardar un valor de la entrada estándar en val y lo imprime inmediatamente.

.data
val: .word 0

.text
main:

#Esto lee un int de la entrada estandar.  Se guarda en $t0.
li $v0,5
syscall
sw $v0, val	# Guardo lo que leí en val.


# Imprime lo que se guardó en val.
li $v0, 1       # $system call code for print_int
lw $a0, val    	# $integer to print
syscall         # print it

li 	$v0, 10	#Salir.
syscall
