.global _start
_start:
        mov x1,0x80000
        mov x3,16

        ldr x0,[x1]
        ldr x2, [x1,#8]
        ldr x4, [x1,x3]
        ldr x5, [x1,x3,lsl #3]
