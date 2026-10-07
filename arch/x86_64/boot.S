.section .text.entry

.globl _start
.type _start, @function

.extern kernel_main

_start:
    lea stack_top(%rip), %rsp

    andq $-16, %rsp

    call kernel_main

1:
    hlt
    jmp 1b

.section .bss

.align 16
stack_bottom:
    .skip 16384
stack_top: