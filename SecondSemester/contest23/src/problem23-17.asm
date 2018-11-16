;   problem23-17 - multiplication
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
;   Calc a * b * c


%INCLUDE "io.inc"

SECTION .RODATA
    ONE_DIGITS DD 1000

SECTION .TEXT

%DEFINE COUNT_OF_DIGITS 10

MULT:
    PUSH EBP
    MOV EBP, ESP
    PUSH EBX
    PUSH ESI
    PUSH EDI
    AND ESP, -16
    MOV EDI, [EBP + 8]
    MOV ESI, [EBP + 12]
    MOV EDX, [EBP + 16]
    XOR EBX, EBX
.START_OUT_LOOP:
    CMP EBX, COUNT_OF_DIGITS
    JE .EXIT_OUT_LOOP

    XOR EAX, EAX
    MOV [EDI + 4 * EBX], EAX
    XOR ECX, ECX
.START_IN_LOOP:
    CMP ECX, EBX
    JA .EXIT_IN_LOOP

    MOV EAX, EBX
    SUB EAX, ECX
    MOV EAX, [ESI + 4 * EAX]
    IMUL EAX, DWORD [EDX + 4 * ECX]
    ADD [EDI + 4 * EBX], EAX

    INC ECX
    JMP .START_IN_LOOP
.EXIT_IN_LOOP:

    INC EBX
    JMP .START_OUT_LOOP
.EXIT_OUT_LOOP:

    XOR ECX, ECX
    XOR EAX, EAX
.START_LOOP_TWO:
    CMP ECX, COUNT_OF_DIGITS
    JE .EXIT_LOOP_TWO

    XOR EDX, EDX
    ADD EAX, [EDI + 4 * ECX]
    DIV DWORD [ONE_DIGITS]
    MOV [EDI + 4 * ECX], EDX

    INC ECX
    JMP .START_LOOP_TWO
.EXIT_LOOP_TWO:

    MOV EAX, EDI

    LEA ESP, [EBP - 12]
    POP EDI
    POP ESI
    POP EBX
    POP EBP
    RET


INT_TO_LONG:
    PUSH EBP
    MOV EBP, ESP
    PUSH EDI
    AND ESP, -16

    MOV EDI, [EBP + 8]
    MOV EAX, [EBP + 12]

    XOR ECX, ECX
.START_LOOP_ONE:
    CMP ECX, COUNT_OF_DIGITS
    JE .EXIT_LOOP_ONE

    XOR EDX, EDX
    DIV DWORD [ONE_DIGITS]
    MOV [EDI + 4 * ECX], EDX
    INC ECX
    JMP .START_LOOP_ONE
.EXIT_LOOP_ONE:

    MOV EAX, EDI

    LEA ESP, [EBP - 4]
    POP EDI
    POP EBP
    RET


PRINT_LONG:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV ECX, COUNT_OF_DIGITS - 1
    MOV EDX, [EBP + 8]
    XOR EAX, EAX
.START_LOOP:
    CMP ECX, 0
    JL .EXIT_LOOP
    TEST EAX, EAX
    JZ .HASNT_DIGIT
    CMP DWORD [EDX + 4 * ECX], 100
    JGE .PRINT_INT
    PRINT_CHAR '0'
    CMP DWORD [EDX + 4 * ECX], 10
    JGE .PRINT_INT
    PRINT_CHAR '0'
.PRINT_INT:
    PRINT_UDEC 4, [EDX + 4 * ECX]
    JMP .NEXT
.HASNT_DIGIT:
    CMP DWORD [EDX + 4 * ECX], 0
    JE .NEXT
    PRINT_UDEC 4, [EDX + 4 * ECX]
    OR EAX, [EDX + 4 * ECX]
.NEXT:

    DEC ECX
    JMP .START_LOOP
.EXIT_LOOP:

    TEST EAX, EAX
    JNZ .NOT_ZERO
    PRINT_UDEC 4, EAX
.NOT_ZERO:

    XOR EAX, EAX
    LEAVE
    RET

GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, COUNT_OF_DIGITS * 4 * 3 + 3 * 4
    AND ESP, -16

    GET_UDEC 4, EAX
    MOV [ESP], EBP
    SUB DWORD [ESP], COUNT_OF_DIGITS * 4
    MOV [ESP + 4], EAX
    CALL INT_TO_LONG ; x[0] = a
    GET_UDEC 4, EAX
    SUB DWORD [ESP], COUNT_OF_DIGITS * 4
    MOV [ESP + 4], EAX
    CALL INT_TO_LONG ; x[1] = b
    SUB DWORD [ESP], COUNT_OF_DIGITS * 4
    MOV [ESP + 4], EBP
    SUB DWORD [ESP + 4], COUNT_OF_DIGITS * 4
    MOV [ESP + 8], EBP
    SUB DWORD [ESP + 8], COUNT_OF_DIGITS * 4 * 2
    CALL MULT ; x[2] = a * b = x[0] = x[1]
    GET_UDEC 4, EAX
    MOV [ESP + 4], EAX
    MOV [ESP], EBP
    SUB DWORD [ESP], COUNT_OF_DIGITS * 4
    CALL INT_TO_LONG ; x[0] = c
    SUB DWORD [ESP], COUNT_OF_DIGITS * 4
    MOV [ESP + 4], EBP
    SUB DWORD [ESP + 4], COUNT_OF_DIGITS * 4
    SUB DWORD [ESP + 8], COUNT_OF_DIGITS * 4
    CALL MULT ; x[1] = a * b * c = x[0] * x[2]
    CALL PRINT_LONG
    NEWLINE

    LEAVE
    XOR EAX, EAX
    RET

