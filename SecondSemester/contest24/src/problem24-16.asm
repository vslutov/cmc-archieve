;   problem24-16 - DNS
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
;   Realize hashtable [string => unsigned integer].

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


%DEFINE HASHTABLE_SIZE 400031
%DEFINE NODE_SIZE 8

CEXTERN malloc
CEXTERN free

CEXTERN scanf
CEXTERN printf
CEXTERN freopen
CEXTERN fclose

CEXTERN get_stdin
CEXTERN get_stdout

CEXTERN strcmp


SECTION .RODATA
    INPUT_FILENAME DB "input.txt", 0
    INPUT_MODE DB "r", 0

    OUTPUT_FILENAME DB "output.txt", 0
    OUTPUT_MODE DB "w", 0

    SCANF_INT DB "%u", 0
    SCANF_STRING DB "%s", 0
    SCANF_STRING_AND_INT DB "%s%u", 0
    PRINTF_INT DB "%u", 10, 0
    PRINTF_MINUS_ONE DB "-1", 10, 0


SECTION .TEXT

GET_INT:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV DWORD [ESP], SCANF_INT
    LEA EAX, [EBP - 4]
    MOV [ESP + 4], EAX
    CALL scanf

    MOV EAX, [EBP - 4]
    LEAVE
    RET


PRINT_INT:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV DWORD [ESP], PRINTF_INT
    MOV EAX, [EBP + 8]
    MOV [ESP + 4], EAX
    CALL printf

    LEAVE
    RET


PRINT_MINUS_ONE:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    MOV DWORD [ESP], PRINTF_MINUS_ONE
    CALL printf

    LEAVE
    RET


HASH:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    XOR EDX, EDX
    XOR EAX, EAX
    MOV ECX, [EBP + 8]
.BEGIN_LOOP:
    MOV EDX, 257
    MUL EDX
    MOV [EBP + 8], ECX
    MOVZX ECX, BYTE [ECX]
    ADD EAX, ECX
    MOV ECX, HASHTABLE_SIZE
    DIV ECX
    MOV EAX, EDX
    MOV ECX, [EBP + 8]
    INC ECX
    MOVZX EDX, BYTE [ECX]
    TEST EDX, EDX
    JNZ .BEGIN_LOOP

    LEAVE
    RET


READ_HASHTABLE:
    PUSH EBP
    MOV EBP, ESP
    PUSH ESI
    PUSH EDI
    SUB ESP, 16 + 16

    CALL GET_INT
    MOV [EBP - 12], EAX

    IMUL EAX, 101
    ADD EAX, HASHTABLE_SIZE * NODE_SIZE
    MOV [ESP], EAX
    CALL malloc

    MOV ESI, EAX
    MOV EDI, EAX
    ADD EDI, HASHTABLE_SIZE * NODE_SIZE

    MOV ECX, HASHTABLE_SIZE
.BEGIN_PRELOOP:
    MOV DWORD [ESI + NODE_SIZE * ECX - NODE_SIZE], 0
    LOOP .BEGIN_PRELOOP


    MOV ECX, [EBP - 12]
.BEGIN_LOOP:
    MOV [EBP - 12], ECX

    MOV DWORD [ESP], SCANF_STRING_AND_INT
    MOV [ESP + 4], EDI
    LEA EAX, [EBP - 16]
    MOV [ESP + 8], EAX
    CALL scanf

    MOV [ESP], EDI
    CALL HASH

.FILLED:
    LEA EDX, [ESI + EAX * NODE_SIZE]
    MOV ECX, [EDX]
    TEST ECX, ECX
    JZ .EMPTY
    XOR EDX, EDX
    INC EAX
    MOV ECX, HASHTABLE_SIZE
    DIV ECX
    MOV EAX, EDX
    JMP .FILLED
.EMPTY:

    MOV [EDX], EDI
    MOV EAX, [EBP - 16]
    MOV [EDX + 4], EAX
    ADD EDI, 101

    MOV ECX, [EBP - 12]
    LOOP .BEGIN_LOOP

    MOV EAX, ESI
    LEA ESP, [EBP - 8]
    POP EDI
    POP ESI
    POP EBP
    RET


SEARCH:
    PUSH EBP
    MOV EBP, ESP
    PUSH EDI
    PUSH ESI
    PUSH EBX
    SUB ESP, 12 + 16


    MOV EAX, [EBP + 8]
    MOV [ESP], EAX
    CALL HASH

    MOV ESI, [EBP + 12]
    MOV EBX, EAX

.NOT_MATCH:
    LEA EDI, [ESI + EBX * NODE_SIZE]

    MOV EAX, [EDI]
    TEST EAX, EAX
    JZ .FAIL

    MOV [ESP], EAX
    MOV EAX, [EBP + 8]
    MOV [ESP + 4], EAX
    CALL strcmp

    TEST EAX, EAX
    JZ .CATCH

    LEA EAX, [EBX + 1]
    XOR EDX, EDX
    MOV ECX, HASHTABLE_SIZE
    DIV ECX
    MOV EBX, EDX
    JMP .NOT_MATCH

.CATCH:
    MOV EAX, [EDI + 4]
    MOV [ESP], EAX
    CALL PRINT_INT
    JMP .EXIT

.FAIL:
    CALL PRINT_MINUS_ONE

.EXIT:
    LEA ESP, [EBP - 12]
    POP EBX
    POP ESI
    POP EDI
    POP EBP
    RET

DO_ACTIONS:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    MOV DWORD [ESP], 101
    CALL malloc
    MOV [EBP - 4], EAX

    CALL GET_INT
    MOV ECX, EAX

.BEGIN_LOOP:
    MOV [EBP - 8], ECX

    MOV DWORD [ESP], SCANF_STRING
    MOV EAX, [EBP - 4]
    MOV [ESP + 4], EAX
    CALL scanf

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    MOV EAX, [EBP + 8]
    MOV [ESP + 4], EAX
    CALL SEARCH

    MOV ECX, [EBP - 8]
    LOOP .BEGIN_LOOP

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    CALL free

    LEAVE
    RET


GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 8 + 16

    CALL get_stdin
    MOV [ESP + 8], EAX
    MOV DWORD [ESP + 4], INPUT_MODE
    MOV DWORD [ESP], INPUT_FILENAME
    CALL freopen

    CALL get_stdout
    MOV [ESP + 8], EAX
    MOV DWORD [ESP + 4], OUTPUT_MODE
    MOV DWORD [ESP], OUTPUT_FILENAME
    CALL freopen

    CALL READ_HASHTABLE

    MOV [EBP - 4], EAX

    MOV [ESP], EAX
    CALL DO_ACTIONS

    CALL get_stdin
    MOV [ESP], EAX
    CALL fclose

    CALL get_stdout
    MOV [ESP], EAX
    CALL fclose

    MOV EAX, [EBP - 4]
    MOV [ESP], EAX
    CALL free

    XOR EAX, EAX
    LEAVE
    RET

