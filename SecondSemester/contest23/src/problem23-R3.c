/*
    problem23-R3 - task 3
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
    Decompilate:

              %include "io.inc"

                    SECTION .text

              GLOBAL CMAIN
                    CMAIN:
                  SUB           ESP, 12
                  GET_DEC       4, [ESP]
                  GET_DEC       4, [ESP + 4]
                  GET_DEC       4, [ESP + 8]
                  CALL          M
                  ADD           ESP, 12
                  PRINT_DEC       4, EAX
                  NEWLINE
                  XOR           EAX, EAX
                  RET

              M:
                  MOV           EAX, DWORD [ESP + 4]
                  MOV           ECX, DWORD [ESP + 8]
                  MOV           EDX, DWORD [ESP + 12]
              .1:
                  CMP           EAX, ECX
                  JGE           .2
                  XOR           EAX, ECX
                  XOR           ECX, EAX
                  XOR           EAX, ECX
              .2:
                  CMP           EAX, EDX
                  JG            .3
                  RET
              .3:
                  XOR           EAX, EDX
                  XOR           EDX, EAX
                  XOR           EAX, EDX
                  JMP           .1

*/

#include <stdio.h>
#include <stdbool.h>

int
M (int a, int b, int c)
{
  while(true)
  {
    if (a < b)
      {
        a ^= b;
        b ^= a;
        a ^= b;
      }

    if (a <= c)
      return a;
    a ^= c;
    c ^= a;
    a ^= c;
  }
}

int
main (void)
{
  int a, b, c;
  scanf("%d%d%d", &a, &b, &c);
  printf("%d\n", M(a, b, c));
  return 0;
}

