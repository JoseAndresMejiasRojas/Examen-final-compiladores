.data
arreglo_variables:	.word 0, 0
.text
main:
li $t0, 0
la $t1, arreglo_variables
ciclo:
li $v0, 5
syscall
sw $v0, 0($t1)
add $t0, 1
add $t1, 4
bge $t0,2,salir_ciclo
j ciclo
salir_ciclo:
li $t1,0
mul $t0, $t1, 4
la $t2,arreglo_variables
add $t2, $t0, $t2
lw $a0, 0($t2)
li $v0, 1
syscall
li $t1,1
mul $t0, $t1, 4
la $t2,arreglo_variables
add $t2, $t0, $t2
lw $a0, 0($t2)
li $v0, 1
syscall
li $t1,1
mul $t0, $t1, 4
la $t2,arreglo_variables
add $t2, $t0, $t2
lw $a0, 0($t2)
li $v0, 1
syscall
li $t1,0
mul $t0, $t1, 4
la $t2,arreglo_variables
add $t2, $t0, $t2
lw $a0, 0($t2)
li $v0, 1
syscall
li $v0, 10
syscall
