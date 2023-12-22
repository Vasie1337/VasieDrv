; junkcode.asm
.code

junkcode proc
    ; Some random instructions
    mov rax, rbx
    add rcx, rdx
    sub rsi, rdi
    imul r8, r9, 42
    xor r10, r11
    shl r12, 3
    shr r13, 2
    lea r14, [r15 + 16]

    ; Pointless jumps
    jmp short label1
    label1:
    jmp short label2
    label2:

    ; Redundant calculations
    imul rax, rax, 1
    add rbx, 0
    sub rcx, 0
    xor rdx, rdx

    ; Useless comparisons
    cmp rsi, rsi
    je short label3
    label3:
    cmp r8, r8
    jne short label4
    label4:

    ; Some more pointless instructions
    nop
    nop
    nop
    nop
    nop
    nop

junkcode endp
end
