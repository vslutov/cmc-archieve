
; fasm demonstration of writing simple ELF executable

format ELF executable 3
entry start

segment readable executable

start:

  mov eax, 4 ; write
  mov ebx, 1 ; stdout
  mov ecx, msg
  mov edx, msg_size
  int 0x80

  mov eax, 1 ; exit
  mov ebx, 0 ; code = 0
  int 0x80

segment readable writeable

msg db 'Hello world!',0xA
msg_size = $-msg
