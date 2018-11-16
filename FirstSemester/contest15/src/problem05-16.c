/*
    problem05-16 - matrix exponentiation by squaring
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

    F[n] = F[n-1]*A[1] + ... + F[n-k]*A[k]
    Print out F[n] mod p
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  uintmax_t **array;
  size_t width, height;
} Matrix;

Matrix
new_matrix (size_t height, size_t width)
{
  Matrix result = {.width = width, .height = height};
  result.array = (uintmax_t **) malloc(height * sizeof(uintmax_t *));
  assert(result.array != NULL);
  for (size_t i = 0; i < height; ++ i)
  {
    result.array[i] = (uintmax_t *) calloc(width, sizeof(uintmax_t));
    assert(result.array[i] != NULL);
  }
  return result;
}

void
delete_matrix(Matrix A)
{
  for (size_t i = 0; i < A.height; ++ i)
    free(A.array[i]);
  free(A.array);
}

Matrix copy_matrix (Matrix A)
{
  Matrix result = new_matrix(A.height, A.width);
  for (size_t i = 0; i < A.height; ++ i)
    for (size_t j = 0; j < A.width; ++ j)
      result.array[i][j] = A.array[i][j];
  return result;
}

Matrix
multiply (Matrix A, Matrix B, intmax_t module)
{
  Matrix result = new_matrix(A.height, B.width);
  for (size_t i = 0; i < A.height; ++ i)
    for (size_t j = 0; j < B.width; ++ j)
      for (size_t k = 0; k < A.width; ++ k)
        result.array[i][j] = (result.array[i][j] +
                              A.array[i][k] * B.array[k][j]) % module;
  delete_matrix(A);
  delete_matrix(B);
  return result;
}

Matrix
power (Matrix A, intmax_t pow, intmax_t module)
{
  if (pow <= 0)
    {
      Matrix result = new_matrix(A.height, A.width);
      for (size_t i = 0; i < A.height; ++ i)
        result.array[i][i] = 1;
      delete_matrix(A);
      return result;
    }
  if (pow % 2 == 1)
    {
      Matrix temp = copy_matrix(A);
      temp = power(temp, pow - 1, module);
      return multiply(temp, A, module);
    }
  else
    {
      Matrix temp = copy_matrix(A);
      temp = multiply(A, temp, module);
      return power(temp, pow / 2, module);
    }
}

int
main (void)
{
  size_t size;
  uintmax_t pow;
  uintmax_t module;
  scanf("%zu %ju %ju", &size, &pow, &module);

  Matrix A, b;

  A = new_matrix(size, size);
  b = new_matrix(size, 1);

  for (size_t i = 0; i < size; ++ i)
    scanf("%ju", &b.array[size - i - 1][0]);

  for (size_t j = 0; j < size; ++ j)
    scanf("%ju", &A.array[0][j]);
  for (size_t i = 1; i < size; ++ i)
    A.array[i][i - 1] = 1;

  if (pow <= size)
    {
      printf("%ju\n", b.array[size - pow][0] % module);
      delete_matrix(A);
      delete_matrix(b);
    }
  else
    {
      b = multiply(power(A, pow - size, module), b, module);

      printf("%ju\n", b.array[0][0]);
      delete_matrix(b);
    }

  return 0;
}

