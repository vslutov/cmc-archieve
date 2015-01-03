/*
    LICENSE - GNU GPL v3

    cmc-task-two - check two polynomials equivalence
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

#include "polynomial.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
  intmax_t degree;
  intmax_t factor;
} Monomial;

typedef struct _MonomialListStruct
{
  Monomial value;
  struct _MonomialListStruct *next;
} *MonomialList;

MonomialList
newMonomialList (void)
{
  return NULL;
}

MonomialList
addHeadMonomialList (MonomialList list, Monomial value)
{
  MonomialList newElem = (MonomialList)
                         malloc( sizeof(struct _MonomialListStruct) );
  newElem->value = value;
  newElem->next = list;
  return newElem;
}

MonomialList
mergeMonomialList (MonomialList a, MonomialList b)
{
  if (a == NULL)
    return b;
  else
    {
      MonomialList head = a;

      while (a->next != NULL)
        a = a->next;
      a->next = b;

      return head;
    }
}

void
splitMonomialList (MonomialList list, MonomialList middle,
                   MonomialList *pless, MonomialList *pgreat)
{
  MonomialList less, great;
  *pless = less = NULL;
  *pgreat = great = NULL;
  while (list != NULL)
  {
    if (list->value.degree < middle->value.degree)
      {
        if (less == NULL)
          *pless = less = list;
        else
          less = less->next = list;
        list = list->next;
        less->next = NULL;
      }
    else
      {
        if (great == NULL)
          *pgreat = great = list;
        else
          great = great->next = list;
        list = list->next;
        great->next = NULL;
      }
  }
}

MonomialList
sortMonomialList (MonomialList list)
{
  if (list == NULL)
    return NULL;
  else
    {
      MonomialList middle = list;
      MonomialList less, great;
      splitMonomialList(list->next, middle, &less, &great);
      less = sortMonomialList(less);
      great = sortMonomialList(great);
      middle->next = NULL;
      return mergeMonomialList(great, mergeMonomialList(middle, less));
    }
}

struct _PolynomialStruct
{
  MonomialList list;
};

Polynomial
newPolynomial (void)
{
  Polynomial result = (Polynomial) malloc( sizeof(struct _PolynomialStruct) );
  result->list = newMonomialList();
  return result;
}

void
addMonomial (Polynomial this, Monomial monomial)
{
  this->list = addHeadMonomialList(this->list, monomial);
}

bool
isEqualMonomialList (MonomialList a, MonomialList b)
{
  while ((a != NULL) && (b != NULL))
    if ((a->value.degree == b->value.degree) &&
        (a->value.factor == b->value.factor))
      {
        a = a->next;
        b = b->next;
      }
    else
      return false;

  return (a == NULL) && (b == NULL);
}

char
getInt (char (*getChar)(void), intmax_t *value)
{
  *value = 0;
  char currentChar = getChar();

  while ((currentChar >= '0') && (currentChar <= '9'))
  {
    *value = *value * 10 + (intmax_t)(currentChar - '0');
    currentChar = getChar();
  }

  return currentChar;
}

void
wrong (void)
{
  printf("Wrong format\n");
  exit(-1);
}

Polynomial bufferToPolynomial (char (*getChar)(void))
{
  Polynomial result = newPolynomial();
  bool complete = false;
  bool negative = false;

  while (!complete)
  {
    Monomial current = {0, 0};
    char currentChar;

    currentChar = getInt (getChar, &current.factor);
    if ((currentChar == '-') && (current.factor == 0))
      {
        negative = true;
        continue;
      }

    if (current.factor == 0) current.factor = 1;
    if (negative) current.factor = -current.factor;
    negative = false;

    switch (currentChar)
      {
        case ' ':
        complete = true;
        case '-':
        negative = true;
        case '+':
        addMonomial(result, current);
        break;
        case 'X':
        switch ((currentChar = getChar()))
          {
            case ' ':
            complete = true;
            case '-':
            negative = true;
            case '+':
            current.degree = 1;
            addMonomial(result, current);
            continue;
            case '^':
            currentChar = getInt(getChar, &current.degree);
            addMonomial(result, current);
            if (currentChar == ' ') complete = true;
            else if (currentChar == '-') negative = true;
            else if (currentChar != '+') wrong();
            break;
            default:
            wrong();
          }
        break;
        default:
        wrong();
      }
  }

  result->list = sortMonomialList(result->list);

  return result;
}

FILE *opened_file;

char
getCharFromFile (void)
{
  return getc(opened_file);
}

Polynomial fileToPolynomial (FILE *file)
{
  opened_file = file;
  return bufferToPolynomial(getCharFromFile);
}

char *opened_string;
size_t position_string;

char
getCharFromString (void)
{
  return opened_string[position_string ++];
}

Polynomial stringToPolynomial (char *string)
{
  opened_string = string;
  position_string = 0;
  return bufferToPolynomial(getCharFromString);
}

bool
isEqualPolynomial (Polynomial a, Polynomial b)
{
  return isEqualMonomialList(a->list, b->list);
}

