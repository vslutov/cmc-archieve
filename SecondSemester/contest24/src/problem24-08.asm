;   problem24-08 - sorting
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
;   Sort 32-bits signed int list.

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
CEXTERN free
CEXTERN qsort

CEXTERN fopen
CEXTERN fclose
CEXTERN fscanf
CEXTERN fprintf
CEXTERN fputc


SECTION .RODATA

    INPUT_FILENAME DB "input.txt", 0
    INPUT_MODE DB "r", 0

    OUTPUT_FILENAME DB "output.txt", 0
    OUTPUT_MODE DB "w", 0

    FSCANF_STRING DB "%d", 0
    FPRINTF_STRING DB "%d ", 0


SECTION .TEXT

READ_LIST:
    PUSH EBP
    MOV EBP, ESP
    PUSH ESI
    PUSH EDI
    SUB ESP, 16

    MOV EDI, [EBP + 8]
    MOV ESI, [EBP + 12]

.BEGIN_LOOP:
    ADD EDI, 4
    MOV [ESP], ESI
    MOV DWORD [ESP + 4], FSCANF_STRING
    MOV [ESP + 8], EDI
    CALL fscanf
    CMP EAX, 1
    JE .BEGIN_LOOP

    SUB EDI, [EBP + 8]
    SHR EDI, 2
    DEC EDI

    MOV ESI, [EBP + 8]
    MOV [ESI], EDI

    MOV EAX, ESI

    LEA ESP, [EBP - 8]
    POP EDI
    POP ESI
    POP EBP
    RET


COMPAR:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV EAX, [EBP + 8]
    MOV EAX, [EAX]
    MOV ECX, [EBP + 12]
    MOV ECX, [ECX]
    SUB EAX, ECX

    JNO .OK
    ADD EAX, ECX
.OK:

    LEAVE
    RET


PRINT_LIST:
    PUSH EBP
    MOV EBP, ESP
    PUSH ESI
    PUSH EDI
    SUB ESP, 16

    MOV ESI, [EBP + 8]
    MOV EDI, [EBP + 12]

    MOV ECX, [ESI]
.BEGIN_LOOP:
    TEST ECX, ECX
    JZ .END_LOOP
    DEC ECX
    MOV [EBP - 4], ECX

    MOV [ESP], EDI
    MOV DWORD [ESP + 4], FPRINTF_STRING
    ADD ESI, 4
    MOV EAX, [ESI]
    MOV [ESP + 8], EAX
    CALL fprintf

    MOV ECX, [EBP - 4]
    JMP .BEGIN_LOOP
.END_LOOP:

    MOV DWORD [ESP], 10
    MOV [ESP + 4], EDI
    CALL fputc

    MOV EAX, [EBP + 8]

    LEA ESP, [EBP - 8]
    POP EDI
    POP ESI
    POP EBP
    RET


GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    MOV DWORD [ESP], INPUT_FILENAME
    MOV DWORD [ESP + 4], INPUT_MODE
    CALL fopen
    MOV [EBP - 4], EAX

    MOV [ESP + 4], EAX

    MOV DWORD [ESP], 4 + 4 * 1000
    CALL malloc
    MOV [EBP - 8], EAX

    MOV [ESP], EAX

    CALL READ_LIST

    MOV ECX, EAX
    ADD EAX, 4
    MOV [ESP], EAX
    MOV EAX, [ECX]
    MOV [ESP + 4], EAX
    MOV DWORD [ESP + 8], 4
    MOV DWORD [ESP + 12], COMPAR
    CALL qsort

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
    CALL PRINT_LIST

    MOV [ESP], EAX
    CALL free

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    CALL fclose

    XOR EAX, EAX
    LEAVE
    RET

