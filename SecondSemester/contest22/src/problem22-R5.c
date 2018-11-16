/*
    problem22-R5 - task five
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

        section .data

        section .text
        global CMAIN
        CMAIN:

          GET_UDEC 4, ECX
          MOV EDX, 0
          SHR ECX, 1
          JC END

          L1:
          GET_DEC 4, EAX
          GET_DEC 4, EBX
          IMUL EAX, EBX
          ADD EDX, EAX
          DEC ECX
          JNZ L1

          END:
          PRINT_DEC 4, EDX
          NEWLINE
          XOR EAX, EAX
          RET
*/

#include <stdio.h>

int
main (void)
{
  size_t n;
  scanf("%zu", &n);

  int sum = 0;

  if (n % 2 == 0)
  {
    n /= 2;
    for (size_t i = 0; i < n; ++ i)
      {
        int x, y;
        scanf("%d %d", &x, &y);
        sum += x * y;
      }
  }

  printf("%d\n", sum);

  return 0;
}

