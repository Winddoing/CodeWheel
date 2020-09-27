.globl _start
.code16
_start:
    mov $0x3f8, %dx
    add %bl, %al
    add $'0', %al
    out %al, (%dx)
    mov $'\n', %al
    out %al, (%dx)
    hlt
