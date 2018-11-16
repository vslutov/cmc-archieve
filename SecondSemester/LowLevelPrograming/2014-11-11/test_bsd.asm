
; fasm demonstration of writing simple ELF executable

format ELF executable 3
entry start

segment readable executable

start:

  push dword msg_size
  push dword msg
  push dword 1 ; stdout
  mov eax, 4 ; write
  call kernel
  add esp, 16

  push dword 0
  mov eax, 1
  call kernel

kernel:
  int 80h
  ret

segment readable writeable

msg db 'Hello world!',0xA
msg_size = $-msg
