;   problem21-06 - bits extract
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
;   a & ((1 << N) - 1)


%INCLUDE "io.inc"

SECTION .text
GLOBAL CMAIN
CMAIN:

    GET_UDEC 4, EAX
    GET_UDEC 1, BL
    MOV CL, 32
    SUB CL, BL
    SHL EAX, CL
    SHR EAX, CL
    PRINT_UDEC 4, EAX

    XOR EAX, EAX
    RET
