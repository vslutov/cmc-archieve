;   problem22-12 - binary zeros
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
;   In this task, you need to count the number of integers in the range
;   1 to N, in the binary representation which contains exactly K
;   significant zeros.

%INCLUDE "io.inc"

SECTION .text
GLOBAL CMAIN
CMAIN:
    GET_UDEC 4, ESI ; esi := x
    GET_UDEC 4, EDI ; edi := k
    INC EDI

    XOR EAX, EAX
    XOR EBX, EBX ; ebx := count of positions
.REVERSE:
    SHR ESI, 1
    RCL EAX, 1
    INC EBX
    TEST ESI, ESI
    JNZ .REVERSE
    MOV ESI, EAX

    MOV EBP, EDI
    SHL EBP, 2
    SUB ESP, EBP

    MOV ECX, EDI
    XOR EAX, EAX
.ZERO:
    DEC ECX
    MOV [ESP + ECX * 4], EAX
    TEST ECX, ECX
    JNE .ZERO

    XOR EDX, EDX
.CALC:
    MOV ECX, EDI
.ITERATE:
    CMP ECX, 1
    JE .END_ITERATE
    DEC ECX
    MOV EAX, [ESP + ECX * 4]
    ADD EAX, [ESP + ECX * 4 - 4]
    MOV [ESP + ECX * 4], EAX
    JMP .ITERATE
.END_ITERATE:
    INC DWORD [ESP]

    SHR ESI, 1
    JC .END
    CMP EDX, EDI
    JE .END
    DEC DWORD [ESP + 4 * EDX]
    INC EDX
.END:
    DEC EBX
    JNE .CALC

    MOV EAX, [ESP + EDI * 4 - 4]
    PRINT_UDEC 4, EAX
    NEWLINE
    ADD ESP, EBP

    XOR EAX, EAX
    RET

