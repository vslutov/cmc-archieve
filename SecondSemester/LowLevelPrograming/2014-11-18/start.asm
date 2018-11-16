format ELF
extrn main

section '.text' executable

public _start
_start:
  mov eax, [esp]
  mov [argc], eax

  mov eax, esp
  add eax, 4
  mov [argv], eax

  push dword [argv]
  push dword [argc]
  call main
  add esp, 8

  mov ebx, eax
  mov eax, 1
  int 80h

section '.data' writeable
  argc dd 0
  argv dd 0

