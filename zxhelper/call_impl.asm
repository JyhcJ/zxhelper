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
end