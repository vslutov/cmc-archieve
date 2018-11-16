;   problem22-XX - square sort
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
;   Sort the array.


%INCLUDE "io.inc"

SECTION .TEXT
GLOBAL CMAIN
CMAIN:

    GET_UDEC 4, EDX
    MOV EBX, 4
    IMUL EBX, EDX
    SUB ESP, EBX
    MOV EBP, ESP
    SUB EBP, 4
        
    MOV ECX, EDX
.GET_LOOP:
    GET_DEC 4, [EBP + 4 * ECX]
    LOOP .GET_LOOP
    
    MOV ECX, EDX
.OUTER_LOOP:
    MOV EAX, [EBP + 4 * ECX]
    
    PUSH ECX
.INNER_LOOP:
    CMP EAX, [EBP + 4 * ECX]
    JLE .TEST_LOOP
    XCHG EAX, [EBP + 4 * ECX]
.TEST_LOOP:
    LOOP .INNER_LOOP
    POP ECX
    
    MOV [EBP + 4 * ECX], EAX
    LOOP .OUTER_LOOP

    MOV ECX, EDX
.PRINT_LOOP:
    PRINT_DEC 4, [EBP + 4 * ECX]
    PRINT_CHAR ' '
    DEC ECX
    TEST ECX, ECX
    JNZ .PRINT_LOOP

    ADD ESP, EBX
    XOR EAX, EAX
    RET

