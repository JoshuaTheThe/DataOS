        ; Broader General function for testing ANY* feature
        global FeatureIsPresentRDX
        global FeatureIsPresentRCX
        global FeatureCR4Enable
        global FeatureCR0Enable
        global GetCR3
        global SetCR3
FeatureIsPresentRDX:
        push rbp
        mov rbp, rsp
        push rdi
        mov rax, 0x01
        cpuid
        pop rcx
        mov rax, 0x01
        shl rax, cl
        test rdx, rax
        jz .notPresent
        mov rax, 0x01
        jmp .return
.notPresent:
        mov rax, 0x00
.return:
        mov rsp, rbp
        pop rbp
        ret
FeatureIsPresentRCX:
        push rbp
        mov rbp, rsp
        push rdi
        mov rax, 0x01
        cpuid
        mov rdx, rcx ; Test RCX instead of RDX
        pop rcx
        mov rax, 0x01
        shl rax, cl
        test rdx, rax
        jz .notPresent
        mov rax, 0x01
        jmp .return
.notPresent:
        mov rax, 0x00
.return:
        mov rsp, rbp
        pop rbp
        ret

FeatureCR0Enable:
        push rbp
        mov rbp, rsp
        mov rbx, cr0
        mov rax, 0x0001
        mov cl, dil
        shl rax, cl
        or rbx, rax
        mov cr0, rbx
.return:
        mov rsp, rbp
        pop rbp
        ret
SetCR3:
        push rbp
        mov rbp, rsp
        mov rbx, cr3
        mov rax, 0x0001
        mov cl, dil
        shl rax, cl
        or rbx, rax
        mov cr3, rbx
.return:
        mov rsp, rbp
        pop rbp
        ret
FeatureCR4Enable:
        push rbp
        mov rbp, rsp
        mov rbx, cr4
        mov rax, 0x0001
        mov cl, dil
        shl rax, cl
        or rbx, rax
        mov cr4, rbx
.return:
        mov rsp, rbp
        pop rbp
        ret

GetCR3:
        push rbp
        mov rbp, rsp
        mov rax, cr3
.return:
        mov rsp, rbp
        pop rbp
        ret