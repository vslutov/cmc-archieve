;   problem21-02 - volume
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
;   Formula: floor(A * B * C / V)


%include "io.inc"

section .text
MUL3: ; fastcall EAX:EDX = EAX:EDX * ECX
    PUSH EAX
    MOV EAX, EDX
    MUL ECX
    XCHG EAX, [ESP]
    MUL ECX
    ADD EDX, [ESP]
    ADD ESP, 4
    RET
    

global CMAIN
CMAIN:
    GET_UDEC 4, EAX
    XOR EDX, EDX
        
    GET_UDEC 4, ECX
    CALL MUL3
    
    GET_UDEC 4, ECX
    CALL MUL3
    
    GET_UDEC 4, ECX
    DIV ECX
    
    PRINT_UDEC 4, EAX
    
    XOR EAX, EAX
    RET