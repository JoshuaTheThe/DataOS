        bits 64
        global SSEIsAvailable
        global SSEEnable
SSEIsAvailable:
        push rbp
        mov rbp, rsp
        ; FPU && SSE2 && SSE && FXSR
        mov rax, 0x01
        cpuid
        test rdx, (7<<24) | 1
        jz .noSSE
        jmp .return
.noSSE:
        mov rax, 0x00
.return:
        mov rsp, rbp
        pop rbp
        ret
SSEEnable:
        push rbp
        mov rbp, rsp
        mov rax, cr0
        and ax, 0xFFFB
        or ax, 0x2
        mov cr0, rax
        mov rax, cr4
        or ax, 3 << 9
        mov cr4, rax
.return:
        mov rsp, rbp
        pop rbp
        ret