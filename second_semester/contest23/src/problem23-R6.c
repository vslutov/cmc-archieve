/*
    problem23-R6 - task 6
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
                        SUB     ESP, 4
                        GET_UDEC    4, [ESP]
                        CALL    F
                        ADD     ESP, 4
                        PRINT_DEC   4, EAX
                        NEWLINE
                        XOR     EAX, EAX
                        RET

                    F:
                        MOV     EAX, 32
                        CMP     DWORD [ESP], .R
                        CMOVNZ  ECX, EAX
                        CMOVZ   ECX, DWORD [ESP + 8]
                        MOV     EAX, DWORD [ESP + 4]
                        AND     ECX, ECX
                        JNZ     .L
                        XOR     EAX, EAX
                        RET
                    .L:
                        PUSH    EBX
                        MOV     EBX, EAX
                        AND     EBX, 1
                        SHR     EAX, 1
                        DEC     ECX
                        PUSH    ECX
                        PUSH    EAX
                        CALL    F
                    .R:
                        ADD     ESP, 8
                        INC     EAX
                        SUB     EAX, EBX
                        POP     EBX
                        RET

*/

#include <stdio.h>

int
F (unsigned a, unsigned b)
{
  if (b == 0)
    return 0;
  unsigned c = a & 1;
  a >>= 1;
  -- b;
  return F(a, b) + 1 - c;
}

int
main (void)
{
  unsigned a;
  scanf("%u", &a);
  printf("%d\n", F(a, 32));
  return 0;
}

