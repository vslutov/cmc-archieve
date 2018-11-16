;   problem21-14 - vegetable garden
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
;   Input: N, M, K, D, X, Y
;   A = {0, 1, 2, 3, ...}
;   #A = ceil(N * M * K / D)
;
;   if 60 * X + Y < 6 * 60
;     B = A
;   else
;     B = A \ {0, 3, 6, 9, ...}
;
;   Output: #B


%INCLUDE "io.inc"

SECTION .TEXT

CEILDIV: ;fastcall eax = ceil(eax / ecx)
    PUSH EDX
    XOR EDX, EDX
    DIV ECX
    NEG EDX
    SAR EDX, 31
    AND EDX, 1
    ADD EAX, EDX
    POP EDX
    RET

GLOBAL CMAIN
CMAIN:

    GET_UDEC 4, EAX
    GET_UDEC 4, ECX
    MUL ECX
    GET_UDEC 4, ECX
    MUL ECX
    GET_UDEC 4, ECX
    CALL CEILDIV
    
    MOV EBX, EAX
    MOV ECX, 3
    CALL CEILDIV
    MOV ESI, EAX
    
    GET_UDEC 4, EAX
    MOV ECX, 60
    MUL ECX
    GET_UDEC 4, ECX
    ADD EAX, ECX
    MOV ECX, 60 * 6
    DIV ECX ;eax = 0..3
    MOV ECX, EAX
    SHR ECX, 1
    OR EAX, ECX
    AND EAX, 1
    
    MUL ESI
    SUB EBX, EAX
    
    PRINT_UDEC 4, EBX

    XOR EAX, EAX
    RET

