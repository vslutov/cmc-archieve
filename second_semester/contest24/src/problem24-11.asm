;   problem24-11 - list on array
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
;   To sequence apply rule:
;   a ... k m ... r s ... z
;   => (m r) =>
;   m ... r a ... k s ... z
;
;   Input >> size of array, count of rules, rules.
;   Output << array

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

CEXTERN fscanf
CEXTERN fprintf
CEXTERN fputc
CEXTERN fopen
CEXTERN fclose


SECTION .RODATA
    INPUT_FILENAME DB "input.txt", 0
    INPUT_MODE DB "r", 0

    OUTPUT_FILENAME DB "output.txt", 0
    OUTPUT_MODE DB "w", 0

    FSCANF_STRING DB "%d", 0
    FPRINTF_STRING DB "%d ", 0


SECTION .TEXT

GET_INT:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    MOV EAX, [EBP + 8]
    MOV [ESP], EAX
    MOV DWORD [ESP + 4], FSCANF_STRING
    LEA EAX, [EBP - 4]
    MOV DWORD [ESP + 8], EAX
    CALL fscanf

    MOV EAX, [EBP - 4]
    LEAVE
    RET


GET_ARRAY:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV EAX, [EBP + 8]
    INC EAX
    IMUL EAX, 12

    MOV [ESP], EAX
    CALL malloc

    MOV [EBP - 4], EAX

    MOV ECX, [EBP + 8]
    XOR EDX, EDX
.BEGIN_LOOP:
    MOV [EAX], EDX
    INC EDX
    MOV [EAX + 12 + 4], EAX
    ADD EAX, 12
    MOV [EAX - 4], EAX
    LOOP .BEGIN_LOOP

    MOV [EAX], EDX
    MOV ECX, [EBP - 4]
    MOV DWORD [EAX + 8], ECX

    MOV EAX, ECX
    LEAVE
    RET


PRINT_ARRAY:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    MOV EAX, [EBP + 8]
    MOV [EBP - 4], EAX

.BEGIN_LOOP:
    MOV EAX, [EBP - 4]
    MOV EAX, [EAX + 8]

    CMP EAX, [EBP + 8]
    JE .END_LOOP

    MOV [EBP - 4], EAX
    MOV EAX, [EAX]

    MOV [ESP + 8], EAX
    MOV DWORD [ESP + 4], FPRINTF_STRING
    MOV EAX, [EBP + 12]
    MOV [ESP], EAX
    CALL fprintf

    JMP .BEGIN_LOOP
.END_LOOP:

    MOV DWORD [ESP], 10
    MOV EAX, [EBP + 12]
    MOV [ESP + 4], EAX
    CALL fputc

    LEAVE
    RET


DO_ACTIONS:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    MOV ECX, [EBP + 8]
.BEGIN_LOOP:
    MOV [EBP - 4], ECX

    MOV EAX, [EBP + 16]
    MOV [ESP], EAX
    CALL GET_INT
    IMUL EAX, 12
    ADD EAX, [EBP + 12]
    MOV [EBP - 8], EAX
    CALL GET_INT
    IMUL EAX, 12
    MOV ECX, [EBP + 12] ; ecx = head
    ADD EAX, ECX        ; eax = b
    MOV ECX, [ECX + 8]  ; ecx = first
    MOV EDX, [EAX + 8]  ; edx = b->next
    MOV [EAX + 8], ECX
    MOV [ECX + 4], EAX
    MOV ECX, [EBP - 8]  ; ecx = a
    MOV EAX, [ECX + 4]  ; eax = a->prev
    MOV [EAX + 8], EDX
    MOV [EDX + 4], EAX
    MOV EAX, [EBP + 12] ; eax = head
    MOV [EAX + 8], ECX
    MOV [ECX + 4], EAX

    MOV ECX, [EBP - 4]
    LOOP .BEGIN_LOOP

    LEAVE
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

    MOV [ESP], EAX
    CALL GET_INT

    MOV [ESP], EAX
    CALL GET_ARRAY

    MOV [EBP - 8], EAX

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    CALL GET_INT

    MOV [ESP], EAX
    MOV EAX, [EBP - 8]
    MOV [ESP + 4], EAX
    MOV EAX, [EBP - 4]
    MOV [ESP + 8], EAX
    CALL DO_ACTIONS

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
    CALL PRINT_ARRAY

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    CALL fclose

    MOV EAX, [EBP - 8]
    MOV [ESP], EAX
    CALL free

    XOR EAX, EAX
    LEAVE
    RET

