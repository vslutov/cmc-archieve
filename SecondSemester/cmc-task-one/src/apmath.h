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

#ifndef APMATH_H_GUARD
#define APMATH_H_GUARD

#include <math.h>

typedef struct _FunctionStruct *Function;

typedef double_t (*NativeFunction) (double_t);

double_t calc(Function, double_t, double_t); // CalcFunction

Function wrap (NativeFunction);
Function substract (Function, Function);

double_t derivative (Function, double_t, double_t);
Function derivate (Function);

// SolveFunction

double_t solutionBisectionMethod (Function, double_t, double_t, double_t);
double_t solutionChordMethod (Function, double_t, double_t, double_t);
double_t solutionNewtonsMethod (Function, double_t, double_t, double_t);
double_t solutionCombinedMethod (Function, double_t, double_t, double_t);

double_t integralRectangleMethod (Function, double_t, double_t, double_t);
double_t integralTrapezoidalMethod (Function, double_t, double_t, double_t);
double_t integralSimpsonsRule (Function, double_t, double_t, double_t);

#endif

