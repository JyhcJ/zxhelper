.code
;打坐
;call_relex proc
;    push rbx
;    sub rsp, 20h
;    mov ecx, 2
;    mov r15, 140D66140h
;    call r15
;    mov rbx, rax
;    test rax, rax
;    je label_exit
;    mov eax, 2Eh
;    mov rdx, rbx
;    mov word ptr [rbx], ax
;    mov r15, 14171CFD8h
;    mov rcx, qword ptr [r15]
;    lea r8d, [rax - 2Ch]
;    mov rcx, qword ptr [rcx + 40h]
;    mov r15, 1409DC140h
;    call r15
;    mov rcx, rbx
;    add rsp, 20h
;    pop rbx
;    mov r15, 140D66890h
;    jmp r15
;label_exit:
;    add rsp, 20h
;    pop rbx
;    ret
;call_relex endp

call_relex proc
    sub rsp,20h
    ;mov r15,0x14171DFD0
    ;mov rax,qword ptr ds:[r15]
    mov rcx,[rcx]
    mov r15,[rcx+38h]
    mov rcx,[r15+60h]
    mov r15,rdx
    xor rdx,rdx
    mov rdx,1h
    call r15
    add rsp,20h
    ret   
call_relex endp




call_useObj proc
    sub rsp, 20h
    ;mov rcx,0xBFF0510h
    ;mov rdx,0h
    ;mov r8,0x6h
    ;mov r9,0x5d3dh
    ;mov al,1h
    ;mov [rsp + 20h], al
    ;mov rax,1409AB5F2h
    ;jmp rax
    mov r15, [rsp + 28h+20h]
    call r15
    add rsp, 20h
    ret
call_useObj endp


call_hanHua proc
    sub rsp,20h
    ;mov rcx,C0C03A0h
    ;mov r8,1d6f0000h
    ;mov r15,1409DBC20h
    mov r15,r9
    xor r9,r9
    call r15
    add rsp,20h
    ret
call_hanHua endp

call_useObj_encap proc
;rcx 基址+偏移   rdx封包内容    r8封包长度
    sub rsp,20h
    ;mov r15,0x14171CFD8
    ;mov rcx,[r15]
    ;mov rcx,[rcx+0x40]
    ;mov rdx,0x14165b000
    ;mov r8,0xA
    ;mov r9,0x1
    mov r15, [rsp + 28h+20h]
    call r15
    add rsp,20h
    ret
call_useObj_encap endp


call_Select proc
    sub rsp,20h
    mov r14,[rcx]
    mov rcx,[r14+40h]
    ;mov edx,0x382BAA0
    add rcx,170h
    ;mov r15,0x1409AB4F0
    call r8
    add rsp,20h
    ret
call_Select endp

call_teleportation proc
    sub rsp,100h
    mov r15,rcx
    mov r15,[r15]
    mov rcx,[r15+40h]
    add rcx,170h
    ;mov r8,0h
    ;mov r9,0h
    ;mov rdx,1ECFh
    mov r15,[rsp+28h+100h]
    mov [rsp+20h],r15
    mov r15,[rsp+30h+100h]
    mov [rsp+28h],r15
    mov r15,[rsp+38h+100h]
    call r15
    add rsp,100h
    ret
call_teleportation endp
end