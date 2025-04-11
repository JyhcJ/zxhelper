.code

; 示例：一个安全的 x64 ASM 函数模板（遵循 Windows x64 调用约定）
;section .text

;my_asm_function:
    ; === 1. 函数序言 (Prologue) ===
    ;push    rbp         ; 保存调用者的 RBP
    ;mov     rbp, rsp    ; 设置新的栈帧基址 (RBP = 当前 RSP)
    ;sub     rsp, 32     ; 分配局部变量空间（对齐到 16 字节边界）

    ; === 2. 函数体 ===
    ; 在这里编写你的汇编代码...
    ; 可安全使用以下寄存器（易失性，无需保存）：
    ;   RAX, RCX, RDX, R8, R9, R10, R11
    ; 非易失性寄存器（如需使用，必须保存）：
    ;   RBX, RSI, RDI, R12-R15

    ; === 3. 函数尾声 (Epilogue) ===
    ;mov     rsp, rbp    ; 恢复 RSP（释放局部空间）
    ;pop     rbp         ; 恢复调用者的 RBP
    ;ret     



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

    ;push    rbp         ; 保存调用者的 RBP
    ;mov     rbp, rsp    ; 设置新的栈帧基址 (RBP = 当前 RSP)
    ;sub     rsp, 20     ; 分配局部变量空间（对齐到 16 字节边界）
    ;sub rsp,32h
    ;-----------

    mov rcx,[rcx]
    mov r15,[rcx+38h]
    mov rcx,[r15+60h]
    mov r15,rdx
    xor rdx,rdx
    mov rdx,1h
    sub rsp,32h
    call r15

    ;-----------
    add rsp,32h
    ret   
   
    ;mov     rsp, rbp    ; 恢复 RSP（释放局部空间）
    ;pop     rbp         ; 恢复调用者的 RBP
    ;ret   
call_relex endp




call_useObj proc
    sub rsp, 20h
    ;不安全
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
    ;sub rsp,20h
    ;mov rcx,C0C03A0h
    ;mov r8,1d6f0000h
    ;mov r15,1409DBC20h
    mov r15,r9
    xor r9,r9
    sub rsp,32h
    call r8
    add rsp,32h
    ;add rsp,20h
    ret
call_hanHua endp

call_useObj_encap proc
;rcx 基址+偏移   rdx封包内容    r8封包长度
    sub rsp,20h
    ;不安全
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
  
    mov r14,[rcx]
    mov rcx,[r14+40h]
    ;mov edx,0x382BAA0
    add rcx,170h
    ;mov r15,0x1409AB4F0
    sub rsp,32h
    call r8
    add rsp,32h
    ret
call_Select endp

call_teleportation proc
    ;sub rsp,100h
    mov r15,rcx
    mov r15,[r15]
    mov rcx,[r15+40h]
    add rcx,170h
    ;mov r8,0h
    ;mov r9,0h
    ;mov rdx,1ECFh
    sub rsp,100h
    mov r15,[rsp+28h+100h]
    mov [rsp+20h],r15
    mov r15,[rsp+30h+100h]
    mov [rsp+28h],r15
    mov r15,[rsp+38h+100h]
    call r15
    add rsp,100h
    ret
call_teleportation endp



call_follow proc
    
    ;sub rsp,180h
    ;;mov rcx,0000000004FBFFD0h ;//界面对象  界面对象 + 170 = 选中对象ID
    ;mov rbx,rcx
    ;mov r8 ,rcx
    ;;mov [rcx+170h],0354A0D0h  ;//选中对象ID
    ;;mov rdx,0000000141227BA8h ;//指令Follow 0000000141227BA8  46 6F 6C 6C 6F 77 00 00 42 69 6E 64 00 00 00 00  Follow..Bind....  
    ;
    ;mov rdi,rdx
    ;mov r9,0000000140C420B0h ;//call地址
    ;call r9
    ;add rsp,180h
    ;ret
        

    ;48 8B CF 48 63 D8 E8 ?? ?? ?? ?? 48 8B C8 41 B0 01 8B D3 E8 ?? ?? ?? ?? 48 8B CF 
    sub rsp,40h
    call rcx
    mov rcx,rax
    ;mov rcx,0x0000000021600140
    xor r9d,r9d
    xor eax,eax
    xor r8d,r8d
    mov rdx,00000000000000EEh

    ;mov r8,0x0000000000000000
    ;mov r9,0x0000000000000000
 
    ;mov r15,00000001402D4830h
    ;mov [rsp+20],0x000000000382BAA0
    mov r15,[rsp+28h+40h]
    mov [rsp+20h],r15

    mov r15,[rsp+30h+40h]
    call r15
    add rsp,40h
    ret


call_follow endp
end