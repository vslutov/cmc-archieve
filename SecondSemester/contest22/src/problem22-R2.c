/*
    problem22-R2 - task 2
    Copyright (C) 2014  Lutov V. S.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
    Task description:

    Retranslate:

        %include "io.inc"

        SECTION .text

        GLOBAL CMAIN
        CMAIN:
            GET_UDEC        4, EAX

            MOV             EBX, EAX
            DEC             EBX
            XOR             EAX, EBX
            ADD             EAX, 1
            RCR             EAX, 1

            PRINT_UDEC      4, EAX
            NEWLINE

            XOR             EAX, EAX
            RET

*/

#include <stdio.h>
#include <inttypes.h>

int
main (void)
{
  uintmax_t x;
  uintmax_t y;
  uintmax_t max = (((unsigned) -1) >> 1) + 1;

  scanf("%ju", &x);
  for (y = 1; (y != max) && (x % (y << 1) == 0); y <<= 1) {}

  printf("%ju\n", y);

  return 0;
}

