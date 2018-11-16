format ELF executable 3
entry start

segment readable
    vec1 rb 256

segment readable executable

start:
    mov esi, vec1
    mov ecx, 256
    mov al, '@'
    call fill_memory

    mov eax, 0
    ret


; al  - byte for filling
; ecx - size of array
; esi - array
fill_memory:

    jecxz .quit

.lp:
    mov [esi], al
    inc esi
    loop .lp

.quit:
    ret

