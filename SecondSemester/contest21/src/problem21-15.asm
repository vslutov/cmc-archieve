;   problem21-15 - triangle area
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
;   Calc triangle area


%INCLUDE "io.inc"

SECTION .TEXT
GLOBAL CMAIN
CMAIN:
    GET_DEC 4, EAX
    GET_DEC 4, EBX
    GET_DEC 4, ECX
    GET_DEC 4, EDX
    GET_DEC 4, ESI
    GET_DEC 4, EDI
    
    SUB EAX, ESI
    SUB EBX, EDI
    SUB ECX, ESI
    SUB EDX, EDI
    
    IMUL EAX, EDX
    IMUL EBX, ECX
    SUB EAX, EBX
    
    MOV EBX, EAX
    MOV ECX, EAX
    NEG ECX
    SAR EBX, 31
    AND ECX, EBX
    NOT EBX
    AND EAX, EBX
    OR EAX, ECX
    
    XOR EDX, EDX
    MOV ECX, 2
    DIV ECX
    IMUL EDX, 5
    
    PRINT_UDEC 4, EAX
    PRINT_CHAR '.'
    PRINT_UDEC 4, EDX

    XOR EAX, EAX
    RET

