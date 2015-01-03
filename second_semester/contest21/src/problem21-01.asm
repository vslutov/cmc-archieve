;   problem21-01 - black circle
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
;   x(t) = Vx * t + ax/2 * t ** 2
;   y(t) = Vy * t + ay/2 * t ** 2
;
;   Input: Vx, Vy, ax/2, ay/2, t
;   Output: x(t), y(t)


%include "io.inc"

section .text
global CMAIN
CMAIN:
    GET_DEC 4, EAX
    GET_DEC 4, EBX
    GET_DEC 4, ECX
    GET_DEC 4, EDX
    GET_DEC 4, ESI

    IMUL EAX, ESI
    IMUL EBX, ESI
    IMUL ESI, ESI
    IMUL ECX, ESI
    IMUL EDX, ESI

    ADD EAX, ECX
    ADD EBX, EDX

    PRINT_DEC 4, EAX
    PRINT_CHAR ' '
    PRINT_DEC 4, EBX

    XOR EAX, EAX
    RET
