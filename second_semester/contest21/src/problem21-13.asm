;   problem21-13 - king
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
;   abs(x - y)


%INCLUDE "io.inc"

SECTION .TEXT


EABS: ;fastcall abs

    PUSH EBX
    MOV EBX, EAX
    SAR EBX, 31
    SHL EBX, 1
    INC EBX
    IMUL EAX, EBX
    POP EBX
    RET

GLOBAL CMAIN
CMAIN:

    GET_CHAR EAX
    GET_CHAR EBX
    GET_CHAR ECX
    GET_CHAR ECX
    GET_CHAR EDX
    
    SUB EAX, ECX
    CALL EABS
    XCHG EAX, EBX
    SUB EAX, EDX
    CALL EABS
    ADD EAX, EBX
    
    PRINT_DEC 4, EAX

    XOR EAX, EAX
    RET
