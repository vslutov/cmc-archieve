;   problem24-02 - substring
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
;   In >> string one, string two
;   If string one in string two then out << "1 2"
;   If string two in string one then out << "2 1"
;   Else out << "0"


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


CEXTERN strlen
CEXTERN strstr

CEXTERN puts
CEXTERN fgets

CEXTERN get_stdin
CEXTERN get_stdout

%DEFINE MAX_LENGTH 1002

SECTION .DATA
    S_ONE_IN_TWO DB '1 2', 10, 0
    S_TWO_IN_ONE DB '2 1', 10, 0
    S_NONE DB '0', 10, 0

SECTION .TEXT

GET_STRING: ; void get_string (char *);
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 24

    MOV ECX, [EBP + 8]
    MOV [ESP], ECX
    MOV DWORD [ESP + 4], MAX_LENGTH
    CALL get_stdin
    MOV [ESP + 8], EAX
    CALL fgets
    CALL strlen
    MOV ECX, [EBP + 8]
    CMP BYTE [ECX + EAX - 1], 10
    JNE .EXIT
    MOV BYTE [ECX + EAX - 1], 0
.EXIT:

    LEAVE
    RET

GLOBAL CMAIN
CMAIN:
    PUSH EBP
    MOV EBP, ESP
    SUB ESP, 2 * MAX_LENGTH
    AND ESP, -16
    SUB ESP, 16

    MOV [ESP], EBP
    SUB DWORD [ESP], MAX_LENGTH
    CALL GET_STRING
    SUB DWORD [ESP], MAX_LENGTH
    CALL GET_STRING

    MOV DWORD [ESP + 4], EBP
    SUB DWORD [ESP + 4], MAX_LENGTH

    CALL strstr

    TEST EAX, EAX
    JNZ .ONE_IN_TWO

    MOV DWORD [ESP], EBP
    SUB DWORD [ESP], MAX_LENGTH
    MOV DWORD [ESP + 4], EBP
    SUB DWORD [ESP + 4], 2 * MAX_LENGTH

    CALL strstr

    TEST EAX, EAX
    JNZ .TWO_IN_ONE

    MOV DWORD [ESP], S_NONE
    CALL puts
    JMP .EXIT

.ONE_IN_TWO:
    MOV DWORD [ESP], S_ONE_IN_TWO
    CALL puts
    JMP .EXIT

.TWO_IN_ONE:
    MOV DWORD [ESP], S_TWO_IN_ONE
    CALL puts

.EXIT:
    XOR EAX, EAX
    LEAVE
    RET

