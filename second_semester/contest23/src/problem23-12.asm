;   problem23-12 - Pick's theorem
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
;   Input >> triangle
;   Calc the number i of lattice points in the interior located in the polygon.


%INCLUDE "io.inc"

SECTION .TEXT

AREA:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8

    MOV EAX, [EBP + 12]
    IMUL DWORD [EBP + 16]
    MOV [EBP - 4], EAX
    MOV [EBP - 8], EDX

    MOV EAX, [EBP + 8]
    IMUL DWORD [EBP + 20]
    SUB EAX, [EBP - 4]
    SBB EDX, [EBP - 8]

    CMP EAX, 0
    JGE .EXIT
    NOT EAX
    NOT EDX
    ADD EAX, 1
    ADC EDX, 0
.EXIT:

    LEAVE
    RET

GCD:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8

    MOV EAX, [EBP + 8]
    CMP EAX, 0
    JGE .GREAT_ONE
    NEG EAX
.GREAT_ONE:

    MOV ECX, [EBP + 12]
    CMP ECX, 0
    JGE .GREAT_TWO
    NEG ECX
.GREAT_TWO:

.START_LOOP:
    TEST ECX, ECX
    JZ .EXIT_LOOP
    XOR EDX, EDX
    DIV ECX
    MOV EAX, ECX
    MOV ECX, EDX
    JMP .START_LOOP
.EXIT_LOOP:

    MOV ESP, EBP
    POP EBP
    RET

GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP

    XOR ECX, ECX
.START_LOOP_ONE:
    CMP ECX, 6
    JE .EXIT_LOOP_ONE
    GET_DEC 4, EAX
    PUSH EAX
    INC ECX
    JMP .START_LOOP_ONE
.EXIT_LOOP_ONE:
    SUB ESP, 16

    MOV EAX, [EBP - 4 * 2]
    SUB EAX, [EBP - 4 * 6]
    PUSH EAX

    MOV EAX, [EBP - 4 * 1]
    SUB EAX, [EBP - 4 * 5]
    PUSH EAX

    MOV EAX, [EBP - 4 * 2]
    SUB EAX, [EBP - 4 * 4]
    PUSH EAX

    MOV EAX, [EBP - 4 * 1]
    SUB EAX, [EBP - 4 * 3]
    PUSH EAX

    CALL AREA
    MOV [EBP - 4 * 6 - 4], EAX
    MOV [EBP - 4 * 6 - 8], EDX

    CALL GCD
    MOV [EBP - 4 * 6 - 12], EAX

    MOV EAX, [ESP + 8]
    XCHG [ESP], EAX
    MOV [ESP + 8], EAX

    MOV EAX, [ESP + 12]
    XCHG [ESP + 4], EAX
    MOV [ESP + 12], EAX

    CALL GCD
    ADD [EBP - 4 * 6 - 12], EAX

    MOV EAX, [ESP + 8]
    SUB [ESP], EAX

    MOV EAX, [ESP + 12]
    SUB [ESP + 4], EAX

    CALL GCD
    ADD [EBP - 4 * 6 - 12], EAX

    MOV EAX, [EBP - 4 * 6 - 4]
    MOV EDX, [EBP - 4 * 6 - 8]

    SUB EAX, [EBP - 4 * 6 - 12]
    SBB EDX, 0

    SAR EDX, 1
    RCR EAX, 1

    INC EAX

    PRINT_UDEC 4, EAX
    NEWLINE

    MOV ESP, EBP
    POP EBP

    XOR EAX, EAX
    RET

