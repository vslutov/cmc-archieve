;   cmc-task-three - comparation of some sort method
;   Copyright (C) 2014 V. S. Lutov
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU Affero General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU Affero General Public License for more details.
;
;   You should have received a copy of the GNU Affero General Public License
;   along with this program.  If not, see <http://www.gnu.org/licenses/>.

%INCLUDE "global.inc"

sort_cmp_less:
    PUSH EBP
    MOV EBP, ESP
    AND ESP, -16

    FLD QWORD [EBP + 8]
    FLD QWORD [EBP + 16]
    FCOMPP
    FNSTSW AX
    SAHF
    SETA AL

    INC DWORD [test_cmp_counter]

    LEAVE
    RET

