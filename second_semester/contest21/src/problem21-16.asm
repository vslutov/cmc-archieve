;   problem21-16 - equation system
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
;   Solve system:
;     (a11 & x) ^ (a12 & y) = b1
;     (a21 & x) ^ (a22 & y) = b2


%INCLUDE "io.inc"

SECTION .text
GLOBAL CMAIN
CMAIN:
    ; eax := x
    ; ebx := y 
    XOR EAX, EAX
    NOT EAX
    XOR EBX, EBX
    NOT EBX
    
    SUB ESP, 4 * 6
    MOV EBP, ESP
    
    GET_UDEC 4, [EBP + 4 * 0] ; a11
    GET_UDEC 4, [EBP + 4 * 1] ; a12
    GET_UDEC 4, [EBP + 4 * 2] ; a21
    GET_UDEC 4, [EBP + 4 * 3] ; a22
    GET_UDEC 4, [EBP + 4 * 4] ; b1
    GET_UDEC 4, [EBP + 4 * 5] ; b2

    MOV ECX, [EBP + 4 * 4]
    OR  ECX, [EBP + 4 * 5]
    ; (b1 == 0) && (b2 == 0) <=> x = 0; y = 0
    AND EAX, ECX
    AND EBX, ECX
    
    MOV ECX, [EBP + 4 * 4]
    XOR ECX, [EBP + 4 * 0]
    MOV EDX, [EBP + 4 * 5]
    XOR EDX, [EBP + 4 * 2]
    OR  ECX, EDX 
    ; (b1 ^ a11 == 0) && (b2 ^ a21 == 0) <=> x = 1; y = 0
    AND EBX, ECX
    NOT ECX
    OR  EAX, ECX
    
    MOV ECX, [EBP + 4 * 4]
    XOR ECX, [EBP + 4 * 1]
    MOV EDX, [EBP + 4 * 5]
    XOR EDX, [EBP + 4 * 3]
    OR ECX, EDX
    ; (b1 ^ a12 == 0) && (b2 ^ a22 == 0) <=> x = 0; y = 1
    AND EAX, ECX
    NOT ECX
    OR  EBX, ECX
    
    ; else x = 1; y = 1
    
    PRINT_UDEC 4, EAX
    PRINT_CHAR ' '
    PRINT_UDEC 4, EBX
    
    ADD ESP, 4 * 6

    XOR EAX, EAX
    RET

