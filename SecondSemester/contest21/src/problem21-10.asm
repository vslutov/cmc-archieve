;   problem21-10 - pandora year
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
;   Output day of year


%INCLUDE "io.inc"

SECTION .text
GLOBAL CMAIN

IF: ;stdcall
    ;a ? b : c
    
    PUSH EBP
    MOV EBP, ESP
    
    ;a = [ebp + 8]
    ;b = [ebp + 12]
    ;c = [ebp + 16]
    
    PUSH EBX
    
    MOV EAX, [EBP + 8]
    SAR EAX, 31
    
    MOV EBX, [EBP + 8]
    NEG EBX
    SAR EBX, 31
    
    OR EAX, EBX
    MOV EBX, EAX
    NOT EBX
    
    AND EAX, [EBP + 12]
    AND EBX, [EBP + 16]
    
    OR EAX, EBX

    POP EBX
    POP EBP
    RETN 12     
    

CMAIN:
    GET_UDEC 4, ESI
    DEC ESI

    XOR EDX, EDX
    XOR ECX, ECX

.START:
    PUSH ECX
    
    SUB ECX, ESI
    
    PUSH .EXIT
    PUSH .LOOP
    PUSH ECX
    CALL IF
    
    POP ECX
    
    JMP EAX

.LOOP:
    MOV EBX, ECX
    AND EBX, 1
    
    PUSH 41
    PUSH 42
    PUSH EBX
    CALL IF
    
    ADD EDX, EAX

    INC ECX
    JMP .START

.EXIT:
    GET_UDEC 4, ESI
    ADD EDX, ESI
    PRINT_UDEC 4, EDX    

    XOR EAX, EAX
    RET
    