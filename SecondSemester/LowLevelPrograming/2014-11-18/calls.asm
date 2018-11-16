format ELF

section '.text' executable

generic_syscall_3:
  push ebp
  mov ebp, esp
  sub esp, 4 * 2
  mov [esp], ebx

  mov ebx, [ebp + 8]
  mov ecx, [ebp + 12]
  mov edx, [ebp + 16]
  int 80h

  mov edx, eax
  and edx, 0fffff000h
  cmp edx, 0fffff000h
  jnz .okay

  mov [sys_errno], eax
  mov eax, -1

.okay:
  pop ebx
  mov esp, ebp
  pop ebp
  ret

public sys_read
sys_read:
  mov eax, 3
  jmp generic_syscall_3

public sys_write
sys_write:
  mov eax, 4
  jmp generic_syscall_3

section '.data' writeable

public sys_errno
  sys_errno dd 0
