#Imprime el arreglo.

.data
arreglo: .word 0, 1, 2, 3 

.text
main:
	li $t0, 0	# Inmediatos
	la $t1, arreglo	# Dirección
ciclo:
	li $v0, 1       # $system call code for print_int
	lw $a0, 0($t1)  # $integer to print

	syscall         # print it
	
	add $t0, 1	# Contador.

	add $t1, 4	# Otra entrada.

	bge $t0,4,salir # Lo hace 4 veces.

	j ciclo


salir:
	li $v0, 10	#Salir.
	syscall
