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

#include "apmath.h"
#include "apmath-dev.h"

#include <stdlib.h>

// Standart functions

double_t
calc (Function function, double_t x, double_t eps)
{
  return function->calc(function, x, eps);
}

double_t
_wrap_calc (Function this, double_t x, double_t eps)
{
  return (*(NativeFunction *)this->data)(x);
}

Function
wrap (NativeFunction function)
{
  Function this = (Function)malloc( sizeof(struct _FunctionStruct) );
  this->calc = _wrap_calc;
  this->data = malloc( sizeof(NativeFunction) );
  *(NativeFunction *)this->data = function;
  return this;
}

double_t
_substract_calc (Function this, double_t x, double_t eps)
{
  Function a = ((Function *)this->data)[0];
  Function b = ((Function *)this->data)[1];
  return calc(a, x, eps * 0.5) - calc(b, x, eps * 0.5);
}

Function
substract (Function a, Function b)
{
  Function this = (Function)malloc( sizeof(struct _FunctionStruct) );
  this->calc = _substract_calc;
  this->data = malloc( sizeof(Function) * 2);
  ((Function *)this->data)[0] = a;
  ((Function *)this->data)[1] = b;
  return this;
}

