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

#include "helpers.h"

#include <stdlib.h>
#include <stdbool.h>

double_t precision = 1e-6;
size_t counter = 0;

#define _std_solution_size sizeof(double_t) * 3
#define _combined_solution_size _std_solution_size + sizeof(bool)

#define _std_integral_size sizeof(double_t) * 2
#define _simpsons_integral_size _std_integral_size + sizeof(double_t) * 2

// Numerical analysis

int
getCounter (void)
{
  return counter;
}

void
resetCounter (void)
{
  counter = 0;
}

void
setPrecision (double_t value)
{
  precision = value;
}

double_t
solution (Function f, double_t left, double_t right, double_t eps,
  StepFunction step, StepFunction initIterator, StepFunction deinitIterator)
{
  Iterator iterator = (Iterator) malloc( sizeof(struct _IteratorStruct) );
  iterator->left = left;
  iterator->right = right;
  iterator->eps = eps;
  iterator->f = f;

  initIterator(iterator);

  while (!helpers_eq(iterator->left, iterator->right, eps))
  {
    step(iterator);
    ++ counter;
  }

  deinitIterator(iterator);
  double_t value = iterator->value;
  free(iterator);

  return value;
}

void
_std_solution_init (Iterator this)
{
  if (this->right < this->left)
    {
      double_t temp = this->left;
      this->left = this->right;
      this->right = temp;
      this->reversed = true;
    }
  else
      this->reversed = false;

  double_t middle = (this->left + this->right) * 0.5;

  double_t *data = (double_t *)(this->data = malloc(_std_solution_size));
  double_t f_left = data[0] = calc(this->f, this->left, precision);
  double_t f_right = data[1] = calc(this->f, this->right, precision);
  double_t f_middle = calc(this->f, middle, precision);

  data[2] = (f_right - f_left) * (f_left + f_right - 2 * f_middle);
}

void
_std_solution_deinit (Iterator this)
{
  this->value = (this->left + this->right) * 0.5;
  free(this->data);
}

void
_std_solution_move (Iterator this, double_t *selected, double_t middle)
{
  double_t *data = (double_t *)this->data;
  *selected = middle;

  if (selected == &this->left)
    {
      data[0] = calc(this->f, this->left, precision);
      if (data[0] * calc(this->f, this->left + this->eps, precision) <= 0)
        this->left = this->right = this->left + this->eps * 0.5;
    }
  else
    {
      data[1] = calc(this->f, this->right, precision);
      if (data[1] * calc(this->f, this->right-this->eps, precision) <= 0)
        this->left = this->right = this->right - this->eps * 0.5;
    }
}

void
_bisection_step (Iterator this)
{
  double_t middle = (this->left + this->right) * 0.5;
  double_t *data = (double_t *)this->data;
  double_t f_left = data[0];

  if (f_left * calc(this->f, middle, precision) >= 0)
    this->left = middle;
  else
    this->right = middle;
}

double_t
solutionBisectionMethod (Function f, double_t left, double_t right,
  double_t eps)
{
  return solution(f, left, right, eps, _bisection_step, _std_solution_init,
                  _std_solution_deinit);
}

void
_chord_step (Iterator this)
{
  double_t *data = (double_t *)this->data;

  double_t *selected;
  if (data[2] > 0)
    selected = &this->left;
  else
    selected = &this->right;

  double_t f_left = data[0];
  double_t f_right = data[1];
  _std_solution_move(this, selected,
    (this->left * f_right - this->right * f_left) / (f_right - f_left));
}

double_t
solutionChordMethod(Function f, double_t left, double_t right, double_t eps)
{
  return solution(f, left, right, eps, _chord_step, _std_solution_init,
                  _std_solution_deinit);
}

void
_newtons_step (Iterator this)
{
  double_t *data = (double_t *)this->data;

  double_t *selected;
  if (data[2] > 0)
    selected = &this->right;
  else
    selected = &this->left;

  _std_solution_move(this, selected,
    *selected - calc(this->f, *selected, precision) /
    derivative(this->f, *selected, precision));
}

double_t
solutionNewtonsMethod (Function f, double_t left, double_t right, double_t eps)
{
  return solution(f, left, right, eps, _newtons_step, _std_solution_init,
                  _std_solution_deinit);
}


void
combined_init (Iterator this)
{
  _std_solution_init(this);
  this->data = realloc(this->data, _combined_solution_size);
  *(bool *)((char *)this->data + _std_solution_size) = false;
}

void
_combined_step (Iterator this)
{
  bool *flag = (bool *)((char *)this->data + _std_solution_size);
  if ((*flag = !*flag))
    _chord_step(this);
  else
    _newtons_step(this);
}

double_t
solutionCombinedMethod (Function f, double_t left, double_t right,
                        double_t eps)
{
  return solution(f, left, right, eps, _combined_step, _std_solution_init,
                  _std_solution_deinit);
}

double_t
integral (Function f, double_t left, double_t right, double_t eps,
  StepFunction step, StepFunction initIterator, StepFunction deinitIterator)
{
  Iterator iterator = (Iterator) malloc( sizeof(struct _IteratorStruct) );
  iterator->left = left;
  iterator->right = right;
  iterator->eps = eps * 0.5;
  iterator->f = f;

  initIterator(iterator);

  double_t prev_value;
  step(iterator);
  ++ counter;
  double_t value = iterator->value;

  do
    {
      prev_value = value;
      step(iterator);
      ++ counter;
      value = iterator->value;
    }
  while ( !helpers_eq(prev_value, value, eps * 0.5) );

  deinitIterator(iterator);
  value = iterator->value;
  free(iterator);

  return value;
}

void
_std_integral_init (Iterator this)
{
  this->data = malloc(_std_integral_size);
  double_t *data = (double_t *)this->data;

  if (this->right < this->left)
    {
      double_t temp = this->left;
      this->left = this->right;
      this->right = temp;
      this->reversed = true;
    }
  else
      this->reversed = false;

  data[0] = this->right - this->left; // step
  double_t a = data[1] = this->eps / (this->right - this->left); // funtion eps
  this->value = (calc(this->f, this->left, a)  + calc(this->f, this->right, a))
                * 0.5 * (this->right - this->left);
}

void
_std_integral_deinit (Iterator this)
{
  if (this->reversed)
    this->value = - this->value;

  free(this->data);
}

void
_rectangle_step (Iterator this)
{
  double_t *data = (double_t *)this->data;

  double_t h = data[0];
  h = h * 0.5;
  data[0] = h;

  double_t a = data[1];

  this->value = 0;
  for (double_t x = this->left + h * 0.5; x < this->right; x += h)
    this->value += calc(this->f, x, a);

  this->value *= h;
}

double_t
integralRectangleMethod(Function f, double_t left, double_t right,
  double_t eps)
{
  return integral(f, left, right, eps, _rectangle_step, _std_integral_init,
                  _std_integral_deinit);
}

void
_trapezoidal_step (Iterator this)
{
  double_t *data = (double_t *)this->data;

  double_t h = data[0];
  h = h * 0.5;
  data[0] = h;

  double_t a = data[1];

  double_t sum = 0;

  for (double_t x = this->left + h; x < this->right; x += 2 * h)
    sum += calc(this->f, x, a);

  this->value = this->value * 0.5 + sum * h;
}

double_t
integralTrapezoidalMethod (Function f, double_t left, double_t right,
  double_t eps)
{
  return integral (f, left, right, eps, _trapezoidal_step,
                   _std_integral_init, _std_integral_deinit);
}

void
_simpsons_step (Iterator this)
{
  double_t *data = (double_t *)this->data;

  double_t h = data[0];
  h = h * 0.5;
  data[0] = h;

  double_t a = data[1];

  double_t sum_even = data[2];
  double_t sum_odd = 0;
  double_t sum_first_last = data[3];

  for (double_t x = this->left + h; x < this->right; x += 2 * h)
  {
    sum_odd += calc(this->f, x, a);
  }

  this->value = h / 3.0 * (4.0 * sum_odd + 2.0 * sum_even + sum_first_last);
  data[2] = sum_odd + sum_even; // sum_even
}

void
_simpsons_init (Iterator this)
{
  _std_integral_init(this);
  this->data = realloc(this->data, _simpsons_integral_size);
  double_t *data = (double_t *)this->data;
  data[1] *= 0.5; // eps (becouse multiply by 3/4)
  data[2] = 0;    // previous step hasn't happent yet
  data[3] = calc(this->f, this->left, data[1]) +
            calc(this->f, this->right, data[1]);
}

double_t
integralSimpsonsRule (Function f, double_t left, double_t right, double_t eps)
{
  return integral(f, left, right, eps, _simpsons_step, _simpsons_init,
                  _std_integral_deinit);
}

#undef _std_solution_size
#undef _combined_solution_size

#undef _std_integral_size
#undef _simpsons_integral_size

