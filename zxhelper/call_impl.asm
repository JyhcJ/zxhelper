.code

; ʾ����һ����ȫ�� x64 ASM ����ģ�壨��ѭ Windows x64 ����Լ����
;section .text

;my_asm_function:
    ; === 1. �������� (Prologue) ===
    ;push    rbp         ; ��������ߵ� RBP
    ;mov     rbp, rsp    ; �����µ�ջ֡��ַ (RBP = ��ǰ RSP)
    ;sub     rsp, 32     ; ����ֲ������ռ䣨���뵽 16 �ֽڱ߽磩

    ; === 2. ������ ===
    ; �������д��Ļ�����...
    ; �ɰ�ȫʹ�����¼Ĵ�������ʧ�ԣ����豣�棩��
    ;   RAX, RCX, RDX, R8, R9, R10, R11
    ; ����ʧ�ԼĴ���������ʹ�ã����뱣�棩��
    ;   RBX, RSI, RDI, R12-R15

    ; === 3. ����β�� (Epilogue) ===
    ;mov     rsp, rbp    ; �ָ� RSP���ͷžֲ��ռ䣩
    ;pop     rbp         ; �ָ������ߵ� RBP
    ;ret     



;����
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

    ;push    rbp         ; ��������ߵ� RBP
    ;mov     rbp, rsp    ; �����µ�ջ֡��ַ (RBP = ��ǰ RSP)
    ;sub     rsp, 20     ; ����ֲ������ռ䣨���뵽 16 �ֽڱ߽磩
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
   
    ;mov     rsp, rbp    ; �ָ� RSP���ͷžֲ��ռ䣩
    ;pop     rbp         ; �ָ������ߵ� RBP
    ;ret   
call_relex endp




call_useObj proc
    sub rsp, 20h
    ;����ȫ
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
;rcx ��ַ+ƫ��   rdx�������    r8�������
    sub rsp,20h
    ;����ȫ
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
    ;;mov rcx,0000000004FBFFD0h ;//�������  ������� + 170 = ѡ�ж���ID
    ;mov rbx,rcx
    ;mov r8 ,rcx
    ;;mov [rcx+170h],0354A0D0h  ;//ѡ�ж���ID
    ;;mov rdx,0000000141227BA8h ;//ָ��Follow 0000000141227BA8  46 6F 6C 6C 6F 77 00 00 42 69 6E 64 00 00 00 00  Follow..Bind....  
    ;
    ;mov rdi,rdx
    ;mov r9,0000000140C420B0h ;//call��ַ
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