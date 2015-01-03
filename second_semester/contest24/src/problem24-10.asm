;   problem24-10 - search tree
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
;   Realize search tree.

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

CEXTERN scanf
CEXTERN printf
CEXTERN getchar


%DEFINE NODE_SIZE 20


SECTION .RODATA
    SCANF_COMMAND DB "%c", 0
    SCANF_INT DB "%d", 0
    SCANF_TWO_INT DB "%d%d", 0
    PRINTF_STRING DB "%d %d", 10, 0


SECTION .bss
    HEAP RESD 1
    HEAP_HEAD RESD 1


SECTION .TEXT

NEW_NODE:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV EAX, [HEAP]
    ADD DWORD [HEAP], NODE_SIZE

    LEAVE
    RET


SCANLINE:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

.BEGIN_LOOP:
    CALL getchar
    CMP EAX, 10
    JNE .BEGIN_LOOP

    LEAVE
    RET


ADD:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

.ENTER:
    MOV ECX, [EBP + 8]
    MOV EAX, [ECX]
    TEST EAX, EAX
    JNZ .NOT_NULL

    MOV [EBP - 4], ECX
    CALL NEW_NODE

    MOV ECX, [EBP + 12]
    MOV [EAX], ECX
    MOV ECX, [EBP + 16]
    MOV [EAX + 4], ECX
    MOV DWORD [EAX + 8], 0
    MOV DWORD [EAX + 12], 0
    MOV DWORD [EAX + 16], 0

    MOV ECX, [EBP - 4]
    MOV [ECX], EAX

    JMP .EXIT

.NOT_NULL:
    MOV EDX, [EBP + 12]
    CMP EDX, [EAX]

    JL .LEFT
    JG .RIGHT

    MOV EDX, [EBP + 16]
    MOV [EAX + 4], EDX
    MOV DWORD [EAX + 16], 0
    JMP .EXIT

.LEFT:
    ADD EAX, 8
    MOV [EBP + 8], EAX
    JMP .ENTER

.RIGHT:
    ADD EAX, 12
    MOV [EBP + 8], EAX
    JMP .ENTER

.EXIT:
    LEAVE
    RET


FIND:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

.ENTER:
    MOV ECX, [EBP + 8]
    TEST ECX, ECX
    JNZ .NOT_NULL

    XOR EAX, EAX
    JMP .EXIT

.NOT_NULL:
    MOV EAX, [EBP + 12]
    CMP EAX, [ECX]

    JL .LEFT
    JG .RIGHT

    XOR EAX, EAX
    MOV EDX, [ECX + 16]
    TEST EDX, EDX
    JNZ .EXIT
    MOV EAX, ECX
    JMP .EXIT

.LEFT:
    MOV EAX, [ECX + 8]
    MOV [EBP + 8], EAX
    JMP .ENTER

.RIGHT:
    MOV EAX, [ECX + 12]
    MOV [EBP + 8], EAX
    JMP .ENTER

.EXIT:
    LEAVE
    RET


DELETE:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV EAX, [EBP + 8]
    MOV [ESP], EAX
    MOV EAX, [EBP + 12]
    MOV [ESP + 4], EAX
    CALL FIND

    TEST EAX, EAX
    JZ .EXIT

    MOV DWORD [EAX + 16], 1

.EXIT:
    LEAVE
    RET


GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16 + 16

    MOV DWORD [ESP], NODE_SIZE * 15000
    CALL malloc
    MOV [HEAP], EAX
    MOV [HEAP_HEAD], EAX

    MOV DWORD [EBP - 4], 0

.BEGIN_LOOP:
    CALL getchar

    CMP EAX, 'A'
    JE .ADD

    CMP EAX, 'D'
    JE .DELETE

    CMP EAX, 'S'
    JE .SEARCH

    JMP .END_LOOP

.ADD:
    MOV DWORD [ESP], SCANF_TWO_INT
    LEA EAX, [EBP - 8]
    MOV [ESP + 4], EAX
    LEA EAX, [EBP - 12]
    MOV [ESP + 8], EAX
    CALL scanf
    CALL SCANLINE

    LEA EAX, [EBP - 4]
    MOV [ESP], EAX
    MOV EAX, [EBP - 8]
    MOV [ESP + 4], EAX
    MOV EAX, [EBP - 12]
    MOV [ESP + 8], EAX
    CALL ADD

    JMP .BEGIN_LOOP

.DELETE:
    MOV DWORD [ESP], SCANF_INT
    LEA EAX, [EBP - 8]
    MOV [ESP + 4], EAX
    CALL scanf
    CALL SCANLINE

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    MOV EAX, [EBP - 8]
    MOV [ESP + 4], EAX
    CALL DELETE

    JMP .BEGIN_LOOP

.SEARCH:
    MOV DWORD [ESP], SCANF_INT
    LEA EAX, [EBP - 8]
    MOV [ESP + 4], EAX
    CALL scanf
    CALL SCANLINE

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    MOV EAX, [EBP - 8]
    MOV [ESP + 4], EAX
    CALL FIND

    TEST EAX, EAX
    JZ .BEGIN_LOOP
    MOV ECX, [EAX]
    MOV [ESP + 4], ECX
    MOV ECX, [EAX + 4]
    MOV [ESP + 8], ECX
    MOV DWORD [ESP], PRINTF_STRING
    CALL printf

    JMP .BEGIN_LOOP

.END_LOOP:

    MOV EAX, [HEAP_HEAD]
    MOV [ESP], EAX
    CALL free

    XOR EAX, EAX
    LEAVE
    RET

