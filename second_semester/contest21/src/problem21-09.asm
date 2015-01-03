;   problem21-09 - abs
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
;   Output absolute value of input


%INCLUDE "io.inc"

SECTION .text
GLOBAL CMAIN
CMAIN:
    mov ebp, esp; for correct debugging

    GET_DEC 4, EAX

    MOV ECX, EAX
    SAR ECX, 31
    
    MOV EBX, ECX
    NOT EBX
    AND EBX, EAX
    
    NEG EAX
    AND EAX, ECX
    OR EAX, EBX

    PRINT_DEC 4, EAX

    XOR EAX, EAX
    RET
