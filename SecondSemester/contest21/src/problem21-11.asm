;   problem21-11 - white and square
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
;   Input chess field, like A1 or H7
;   Output count of white field on the right and up from the field.
;   Formula: X * Y / 2


%INCLUDE "io.inc"

SECTION .text
GLOBAL CMAIN
CMAIN:

    GET_CHAR EAX
    SUB EAX, 'A' + 7
    NEG EAX
    GET_CHAR EBX
    SUB EBX, '1' + 7
    NEG EBX
    MUL EBX
    SHR EAX, 1
    PRINT_DEC 4, EAX

    XOR EAX, EAX
    RET
