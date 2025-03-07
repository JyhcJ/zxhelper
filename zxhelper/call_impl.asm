.code
;´ò×ø
call_relex proc
    push rbx
    sub rsp, 20h
    mov ecx, 2
    mov r15, 140D66140h
    call r15
    mov rbx, rax
    test rax, rax
    je label_exit
    mov eax, 2Eh
    mov rdx, rbx
    mov word ptr [rbx], ax
    mov r15, 14171CFD8h
    mov rcx, qword ptr [r15]
    lea r8d, [rax - 2Ch]
    mov rcx, qword ptr [rcx + 40h]
    mov r15, 1409DC140h
    call r15
    mov rcx, rbx
    add rsp, 20h
    pop rbx
    mov r15, 140D66890h
    jmp r15
label_exit:
    add rsp, 20h
    pop rbx
    ret
call_relex endp

call_useObj proc
    sub rsp, 38h
    ;mov rcx,0xBFF0510h
    ;mov rdx,0h
    ;mov r8,0x6h
    ;mov r9,0x5d3dh
    mov al,1h
    mov [rsp + 20h], al
    mov rax, 1409a9c70h
    call rax
    add rsp, 38h
    ret
call_useObj endp

call_hanHua proc
    sub rsp,20h
    ;mov rcx,C0C03A0h
    ;mov r8,1d6f0000h
    mov r15,1409DBC20h
    call r15
    add rsp,20h
    ret
call_hanHua endp
end