;   problem23-06 - zeroes
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
;   Input >> n, A[n], k
;   Output << #{x from A[n]: count_of_zeroes(x) = k}


%INCLUDE "io.inc"

SECTION .TEXT

COUNT_OF_ZEROES:
    PUSH EBP
    MOV EBP, ESP
    PUSH EBX
    AND ESP, 0XFFFFFFF0

    MOV EBX, [EBP + 8]
    XOR EAX, EAX
.START_LOOP:
    TEST EBX, EBX
    JZ .EXIT_LOOP
    SHR EBX, 1
    JC .ONE
    INC EAX
.ONE:
    JMP .START_LOOP
.EXIT_LOOP:

    MOV EBX, [EBP + 12]
    CMP EAX, EBX
    JE .YES
    XOR EAX, EAX
    JMP .FIN
.YES:
    XOR EAX, EAX
    INC EAX
.FIN:

    LEA ESP, [EBP - 4]
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

    GET_UDEC 4, EDX
    NEG EDX

    XOR ECX, ECX
.START_LOOP_ONE:
    CMP ECX, EDX
    JE .EXIT_LOOP_ONE
    GET_UDEC 4, EAX
    PUSH EAX
    DEC ECX
    JMP .START_LOOP_ONE
.EXIT_LOOP_ONE:

    AND ESP, 0XFFFFFFF0
    SUB ESP, 16

    GET_UDEC 4, EBX
    MOV [ESP + 4], EBX

    XOR ECX, ECX
    XOR EBX, EBX
.START_LOOP_TWO:
    CMP ECX, EDX
    JE .EXIT_LOOP_TWO

    MOV EAX, [EBP + 4 * ECX - 4 - 3 * 4]
    MOV [ESP], EAX
    CALL COUNT_OF_ZEROES
    ADD EBX, EAX

    DEC ECX
    JMP .START_LOOP_TWO
.EXIT_LOOP_TWO:

    PRINT_UDEC 4, EBX
    PRINT_CHAR 10

    LEA ESP, [EBP - 3 * 4]
    POP EDX
    POP ECX
    POP EBX
    POP EBP

    XOR EAX, EAX
    RET

