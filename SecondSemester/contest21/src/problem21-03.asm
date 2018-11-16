;   problem21-06 - two ball
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
;   Input: count of R-, G-, B-colored balls in first bucket and in
;          one
;   Output: How many ways to put up two balls from two bucket with
;           two color


%INCLUDE "io.inc"

MUL2:; fastcall EAX = AX * CX
    PUSH EDX
    MUL CX
    XCHG AX, DX
    SHL EAX, 16
    MOV AX, DX
    POP EDX
    RET

SECTION .text
GLOBAL CMAIN
CMAIN:
    GET_UDEC 2, AX
    GET_UDEC 2, BX
    GET_UDEC 2, CX
    
    MOV DX, BX
    SHL DX, 1
    ADD AX, BX; AX = R + B
    ADD CX, BX; CX = B + G
    MOV BX, AX
    ADD BX, CX
    SUB BX, DX; BX = R + G
    
    XCHG AX, CX
    SHL ECX, 16
    MOV CX, AX; ECX = [R + B:B + G]

    GET_UDEC 2, AX
    CALL MUL2
    PUSH EAX
    
    GET_UDEC 2, AX
    MOV CX, BX
    CALL MUL2
    ADD [ESP], EAX
    
    GET_UDEC 2, AX
    SHR ECX, 16
    CALL MUL2
    ADD EAX, [ESP]
    ADD ESP, 4
    
    PRINT_UDEC 4, EAX

    XOR EAX, EAX
    RET