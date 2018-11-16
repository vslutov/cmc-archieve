;   problem23-09 - insufficient numbers
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
;   IS_INSUFFICIENT(x) = sum(y: x % y == 0, x != y) < x
;   Print k-ordered insufficient number.


%INCLUDE "io.inc"

SECTION .TEXT

IS_INSUFFICIENT:
    PUSH EBP
    MOV EBP, ESP
    PUSH EBX
    PUSH ECX
    PUSH EDX
    AND ESP, -16

    XOR ECX, ECX
    XOR EBX, EBX
.START_LOOP:
    INC ECX
    MOV EAX, [EBP + 8]
    CMP ECX, EAX
    JE .EXIT_LOOP
    XOR EDX, EDX
    DIV ECX
    TEST EDX, EDX
    JNZ .START_LOOP
    ADD EBX, ECX
    JMP .START_LOOP
.EXIT_LOOP:
    XOR EAX, EAX
    CMP EBX, [EBP + 8]
    JNB .EXIT
    INC EAX
.EXIT:
    LEA ESP, [EBP - 3 * 4]
    POP EDX
    POP ECX
    POP EBX
    POP EBP
    RET

GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    PUSH EBX
    PUSH ECX
    PUSH EDX
    AND ESP, -16

    GET_UDEC 4, EBX

    XOR ECX, ECX
    XOR EDX, EDX
.START_LOOP:
    INC ECX
    MOV [ESP], ECX
    CALL IS_INSUFFICIENT
    TEST EAX, EAX
    JZ .START_LOOP
    INC EDX
    CMP EDX, EBX
    JNE .START_LOOP

    PRINT_UDEC 4, ECX
    PRINT_CHAR 10

    LEA ESP, [EBP - 3 * 4]
    POP EDX
    POP ECX
    POP EBX
    POP EBP

    XOR EAX, EAX
    RET

