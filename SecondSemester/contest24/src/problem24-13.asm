;   problem24-13 - happy stack
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
;   Realize:
;   void apply (int *array, size_t length, void (*fn) (...), int n, ...);
;
;   Input >> array
;   apply(array, length(array), fprintf, 2, stdout, "%d\n");
;

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

CEXTERN fprintf
CEXTERN get_stdout
CEXTERN scanf


SECTION .RODATA
    SCANF_STRING DB "%d", 0
    FPRINTF_STRING DB "%d", 10, 0


SECTION .TEXT

APPLY:
    PUSH EBP
    MOV EBP, ESP
    PUSH ESI
    PUSH EDI
    SUB ESP, 16

    MOV ESI, [EBP + 8]
    MOV EAX, [EBP + 12]
    SHL EAX, 2
    SUB ESP, EAX
    AND ESP, -16

    MOV ECX, [EBP + 12]
.BEGIN_LOOP:
    TEST ECX, ECX
    JZ .END_LOOP
    DEC ECX
    MOV [EBP - 12], ECX

    MOV EDI, ESP

    MOV [EBP - 16], ESI
    LEA ESI, [EBP + 24]
    MOV ECX, [EBP + 20]
.BEGIN_SUBLOOP:
    TEST ECX, ECX
    JZ .END_SUBLOOP
    DEC ECX

    MOV EAX, [ESI]
    MOV [EDI], EAX
    ADD ESI, 4
    ADD EDI, 4

    JMP .BEGIN_SUBLOOP
.END_SUBLOOP:
    MOV ESI, [EBP - 16]

    MOV EAX, [ESI]
    ADD ESI, 4
    MOV [EDI], EAX

    MOV EAX, [EBP + 16]
    CALL EAX

    MOV ECX, [EBP - 12]
    JMP .BEGIN_LOOP
.END_LOOP:

    LEA ESP, [EBP - 8]
    POP EDI
    POP ESI
    POP EBP
    RET


GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16 + 16 + 16

    MOV DWORD [ESP], SCANF_STRING
    LEA EAX, [EBP - 4]
    MOV [ESP + 4], EAX
    CALL scanf

    MOV EAX, [EBP - 4]
    SHL EAX, 2
    MOV [ESP], EAX
    CALL malloc

    MOV [EBP - 8], EAX
    MOV EDX, EAX

    MOV ECX, [EBP - 4]
.BEGIN_LOOP:
    TEST ECX, ECX
    JZ .END_LOOP
    DEC ECX
    MOV [EBP - 12], ECX

    MOV DWORD [ESP], SCANF_STRING
    MOV [ESP + 4], EDX
    MOV [EBP - 16], EDX
    CALL scanf
    MOV EDX, [EBP - 16]

    ADD EDX, 4
    MOV ECX, [EBP - 12]
    JMP .BEGIN_LOOP
.END_LOOP:

    MOV DWORD [ESP + 20], FPRINTF_STRING
    CALL get_stdout
    MOV [ESP + 16], EAX
    MOV DWORD [ESP + 12], 2
    MOV DWORD [ESP + 8], fprintf
    MOV EAX, [EBP - 4]
    MOV [ESP + 4], EAX
    MOV EAX, [EBP - 8]
    MOV [ESP], EAX
    CALL APPLY

    MOV EAX, [EBP - 8]
    MOV [ESP], EAX
    CALL free

    XOR EAX, EAX
    LEAVE
    RET

