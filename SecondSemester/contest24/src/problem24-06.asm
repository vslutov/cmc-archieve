;   problem24-06 - pyramid
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
;   Check binary heap in little-endian format.

%ifdef CYGWIN

%macro CEXTERN 1
    extern _%1
    %define %1 _%1
%endmacro
%define CMAIN _main

%elifdef DARWIN

%macro CEXTERN 1
    extern _%1
    %define %1 _%1
%endmacro
%define CMAIN _main

%elifdef UNIX

%macro CEXTERN 1
    extern %1
%endmacro
%define CMAIN main

%else
%error "Select OS type providing '-DCYGWIN', '-DUNIX' or '-DDARWIN' option"
%endif


CEXTERN malloc

CEXTERN fopen
CEXTERN fclose
CEXTERN fread
CEXTERN fwrite


SECTION .RODATA
    INPUT_FILENAME DB "input.bin", 0
    INPUT_MODE DB "rb", 0

    OUTPUT_FILENAME DB "output.bin", 0
    OUTPUT_MODE DB "wb", 0


SECTION .TEXT

READ_HEAP:
    PUSH EBP
    MOV EBP, ESP
    PUSH ESI
    PUSH EDI
    PUSH EBX
    SUB ESP, 12 + 16

    MOV EDI, [EBP + 8]
    MOV ESI, [EBP + 12]

.BEGIN_LOOP:

    MOV [ESP + 12], ESI
    MOV DWORD [ESP + 8], 1
    MOV DWORD [ESP + 4], 4
    LEA EAX, [EBP - 16]
    MOV [ESP], EAX
    CALL fread

    TEST EAX, EAX
    JZ .END_LOOP

    MOV EAX, [EBP - 16]
    ADD EDI, 4
    MOV [EDI], EAX

    JMP .BEGIN_LOOP
.END_LOOP:

    MOV ESI, [EBP + 8]
    SUB EDI, ESI
    SHR EDI, 2
    MOV [ESI], EDI

    MOV EAX, ESI

    LEA ESP, [EBP - 12]
    POP EBX
    POP EDI
    POP ESI
    POP EBP
    RET


CHECK_HEAP:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    MOV EDX, [EBP + 8]
    MOV [EBP - 4], EDX ; heap
    MOV EDX, [EDX]

    MOV ECX, 1
    MOV DWORD [EBP - 16], 3 ; flag
.BEGIN_LOOP:
    INC ECX
    CMP ECX, EDX
    JG .END_LOOP

    MOV EAX, [EBP - 4]
    MOV EAX, [EAX + 4 * ECX]
    MOV [EBP - 8], EAX ; heap[i]
    MOV [EBP - 12], ECX ; i
    SHR ECX, 1
    MOV EAX, [EBP - 4]
    MOV EAX, [EAX + 4 * ECX]
    MOV ECX, [EBP - 12]
    CMP EAX, [EBP - 8]
    JL .LESS
    JNG .BEGIN_LOOP
    AND DWORD [EBP - 16], -2
    JMP .BEGIN_LOOP
.LESS:
    AND DWORD [EBP - 16], -3
    JMP .BEGIN_LOOP
.END_LOOP:

    MOV EAX, [EBP - 16]

    CMP EAX, 0
    JE .EXIT
    CMP EAX, 2
    JE .MINUS_ONE

    MOV EAX, 1
    JMP .EXIT

.MINUS_ONE:
    MOV EAX, -1

.EXIT:
    LEAVE
    RET

OUTPUT:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    MOV EAX, [EBP + 12]
    MOV [ESP + 12], EAX
    MOV DWORD [ESP + 8], 1
    MOV DWORD [ESP + 4], 4
    LEA EAX, [EBP + 8]
    MOV DWORD [ESP], EAX
    CALL fwrite

    LEAVE
    RET


GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 16 + 8

    MOV DWORD [ESP], INPUT_FILENAME
    MOV DWORD [ESP + 4], INPUT_MODE
    CALL fopen

    MOV [EBP - 4], EAX

    MOV DWORD [ESP], 4 * 1024 * 1024 + 4
    CALL malloc

    MOV [EBP - 8], EAX

    MOV [ESP], EAX
    MOV EAX, [EBP - 4]
    MOV [ESP + 4], EAX
    CALL READ_HEAP

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    CALL fclose

    MOV DWORD [ESP], OUTPUT_FILENAME
    MOV DWORD [ESP + 4], OUTPUT_MODE
    CALL fopen

    MOV [EBP - 4], EAX
    MOV [ESP + 4], EAX

    MOV EAX, [EBP - 8]
    MOV [ESP], EAX
    CALL CHECK_HEAP

    MOV [ESP], EAX
    CALL OUTPUT

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    CALL fclose

    XOR EAX, EAX
    LEAVE
    RET

