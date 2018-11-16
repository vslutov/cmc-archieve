/*
    LICENSE - GNU GPL v3

    cmc-task-one - equation solving and definite integrals
    Copyright (C) 2014  V. S. Lutov

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

#ifndef APMATH_DEV_H_GUARD
#define APMATH_DEV_H_GUARD

#include <stdbool.h>

typedef double_t (*CalcFunction) (Function, double_t, double_t);

struct _FunctionStruct
{
  CalcFunction calc;
  void *data;
};

typedef struct _IteratorStruct
{
  double_t left, right, eps;
  bool reversed;
  Function f;

  double_t value;

  void *data;
} *Iterator;

int getCounter (void);
void resetCounter (void);
void setPrecision (double_t);

typedef void (*StepFunction)(Iterator);

double_t
integral (Function, double_t, double_t, double_t, StepFunction, StepFunction,
           StepFunction);

void _std_solution_init (Iterator);
void _std_solution_deinit (Iterator);
void _std_solution_move (Iterator, double_t *, double_t);

void _std_integral_init (Iterator);
void _std_integral_deinit (Iterator);

#endif

