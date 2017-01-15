.text
.globl main
main:
# swap values $t0 and $t1 ... slow!
sw $t0, x
sw $t1, y
lw $t0, y
lw $t1, x
.data
x:
.word 0x10010010
y:
.word 0xABCDE080