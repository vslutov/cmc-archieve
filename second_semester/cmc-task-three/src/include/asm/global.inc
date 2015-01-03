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

%IFNDEF GLOBAL_INC
%DEFINE GLOBAL_INC

%IFNDEF SORT_KIND

%ERROR UNDEFINED SORT KIND

%ENDIF

%DEFINE ASCENDING 1
%DEFINE DESCENDING 2

GLOBAL sort_cmp_less
GLOBAL sort_swap

GLOBAL test_rdtsc

EXTERN test_cmp_counter
EXTERN test_swap_counter

%ENDIF

