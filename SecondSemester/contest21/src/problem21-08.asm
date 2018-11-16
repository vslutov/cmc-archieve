;   problem21-08 - masked merge
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
;   (a & c) | (b & ~c)


%INCLUDE "io.inc"

SECTION .text
GLOBAL CMAIN
CMAIN:

    GET_HEX 4, EAX
    GET_HEX 4, EBX
    GET_HEX 4, ECX
    
    AND EAX, ECX
    NOT ECX
    AND EBX, ECX
    OR EAX, EBX
    
    PRINT_HEX 4, EAX

    XOR EAX, EAX
    RET

