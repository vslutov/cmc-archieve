;   problem21-12 - cards
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
;   Print card name by id.


%INCLUDE "io.inc"

SECTION .DATA
    FIRST_SYMBOL DB '23456789TJQKA'
    SECOND_SYMBOL DB 'SCDH'
    COUNT_FIRST_SYMBOL DD 52 / 4

SECTION .TEXT
GLOBAL CMAIN
CMAIN:
    GET_DEC 4, EAX
    DEC EAX
    XOR EDX, EDX
    DIV DWORD [COUNT_FIRST_SYMBOL]
    PRINT_CHAR [FIRST_SYMBOL + EDX]
    PRINT_CHAR [SECOND_SYMBOL + EAX]

    XOR EAX, EAX
    RET
