;   problem23-02 - ones
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
;   How many ones in ternary notation of number?


%INCLUDE "io.inc"

SECTION .TEXT

F:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 24

    MOV EAX, [EBP + 8]
    TEST EAX, EAX
    JNZ .ELSE

    XOR EAX, EAX

    ADD ESP, 24
    POP EBP
    RET

.ELSE:

    MOV [EBP - 4], EBX
    MOV [EBP - 8], EDX
    XOR EDX, EDX
    MOV EBX, 3
    DIV EBX

    MOV [ESP], EAX
    CALL F

    CMP EDX, 1
    JNZ .NOT_ONE
    INC EAX
.NOT_ONE:

    MOV EBX, [EBP - 4]
    MOV EDX, [EBP - 8]
    ADD ESP, 24
    POP EBP
    RET

GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8

    GET_UDEC 4, EAX

    MOV [ESP], EAX
    CALL F

    PRINT_UDEC 4, EAX
    PRINT_CHAR 10

    ADD ESP, 8
    POP EBP

    XOR EAX, EAX
    RET

