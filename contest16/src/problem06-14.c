/*
    problem06-16 - big-endian track
    Copyright (C) 2013  Lutov V. S.

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

    Input size of matrix (16 bit)
    Matrix elements (32 bit per number)

    Output track of matrix in 64 bits.
*/

#include <stdio.h>
#include <inttypes.h>

uint16_t
get_uint16_t (FILE *fin)
{
  uint16_t result;
  for (size_t i = 0; i < 2; ++ i)
    fread((char *)(&result) + 1 - i, 1, 1, fin);
  return result;
}

int32_t
get_int32_t (FILE *fin)
{
  int32_t result;
  for (size_t i = 0; i < 4; ++ i)
    fread((char *)(&result) + 3 - i, 1, 1, fin);
  return result;
}

void
put_int64_t (int64_t value, FILE *fout)
{
  for (size_t i = 0; i < 8; ++ i)
    fwrite((char *)(&value) + 7 - i, 1, 1, fout);
}

int
main (void)
{
  FILE *fin = fopen("matrix.in", "rb");
  FILE *fout = fopen("trace.out", "wb");

  size_t size = get_uint16_t(fin);

  intmax_t matrix[size][size];

  for (size_t i = 0; i < size; ++ i)
    for (size_t j = 0; j < size; ++ j)
        matrix[i][j] = get_int32_t(fin);

  intmax_t track = 0;
  for (size_t i = 0; i < size; ++ i)
    track += matrix[i][i];

  put_int64_t (track, fout);

  fclose(fin);
  fclose(fout);

  return 0;
}

