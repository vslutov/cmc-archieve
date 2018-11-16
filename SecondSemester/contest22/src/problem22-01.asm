;   problem22-01 - three maximum
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
;   Output three maximum element of sequence.


%INCLUDE "io.inc"

SECTION .TEXT
GLOBAL CMAIN
CMAIN:
    mov ebp, esp; for correct debugging

    XOR EBX, EBX
    BTS EBX, 31
    MOV EDX, EBX
    MOV ESI, EBX ; ebx, edx, esi - three maximum
    
    GET_UDEC 4, ECX
.LOOP:
    GET_DEC 4, EAX
    CMP EAX, EBX
    JLE .CMP2
    XCHG EAX, EBX
.CMP2:
    CMP EAX, EDX
    JLE .CMP3
    XCHG EAX, EDX
.CMP3:
    CMP EAX, ESI
    JLE .LOOP_TEST
    XCHG EAX, ESI
.LOOP_TEST:
    LOOP .LOOP

    PRINT_DEC 4, EBX
    PRINT_CHAR ' '
    PRINT_DEC 4, EDX
    PRINT_CHAR ' '
    PRINT_DEC 4, ESI

    XOR EAX, EAX
    RET

