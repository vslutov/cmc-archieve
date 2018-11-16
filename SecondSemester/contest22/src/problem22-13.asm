;   problem22-13 - long rotation
;   Copyright (C) 2014  Lutov V. S.
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this program.  If not, see <http://www.gnu.org/licenses/>.


;   Task description:
;
;   Rotate 32*N-bits number.


%INCLUDE "io.inc"

SECTION .DATA
    STR_U DB '%u ', 0

SECTION .TEXT
GLOBAL CMAIN
CMAIN:

    GET_UDEC 4, EDI ; edi := n
    TEST EDI, EDI
    JNZ .NEXT
    XOR EAX, EAX
    RET
.NEXT:
    MOV ESI, 4
    IMUL ESI, EDI
    SUB ESP, ESI
    PUSH ESI
    MOV EBP, ESP ; ebp := array
    
    MOV ECX, EDI
.GET_LOOP:
    GET_UDEC 4, [EBP + 4 * ECX]
    LOOP .GET_LOOP
    
    GET_UDEC 4, ECX ; ecx := k
    XOR ESI, ESI
    INC ESI
    SHL ESI, CL
    DEC ESI ; esi := mask
    MOV EDX, ESI
    NOT EDX ; edx := ~mask
    
    XOR EBX, EBX ; ebx = carry
    PUSH ECX
    MOV ECX, EDI
.CALC_LOOP:
    MOV EAX, [EBP + 4 * ECX] ; eax = a[i]
    XCHG ECX, [ESP]
    PUSH EBX
    MOV EBX, ESI
    AND EBX, EAX
    ROR EBX, CL
    AND EAX, EDX
    SHR EAX, CL
    OR EAX, [ESP]
    ADD ESP, 4
    XCHG ECX, [ESP]
    MOV [EBP + 4 * ECX], EAX
    LOOP .CALC_LOOP
    POP ECX
    
    OR [EBP + 4 * EDI], EBX
    
    MOV ECX, EDI
.PRINT_LOOP:
    PUSHA
    PUSH DWORD [EBP + 4 * ECX]
    PUSH STR_U
    CALL printf
    ADD ESP, 4 * 2
    POPA
    LOOP .PRINT_LOOP

    POP ESI
    ADD ESP, ESI

    XOR EAX, EAX
    RET

