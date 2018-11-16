/*
    problem06-11 - hard calculator
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

    Calculate infix notation expression.
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "vector.template.h"

typedef enum
{
  OPERATOR,
  NUMBER,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  END_OF_INPUT
} TokenType;

typedef struct
{
  TokenType type;
  union
  {
    char operator;
    intmax_t number;
  };
} Token;

VECTOR(intmax_t)

intmax_t
calc (Token token)
{
  static Vector_intmax_t calc_stack = NULL;
  if (calc_stack == NULL)
    calc_stack = new_vector_intmax_t();

  if (token.type == NUMBER)
  {
    push_back_vector_intmax_t(calc_stack, token.number);
    return token.number;
  }
  else if (token.type == OPERATOR)
    {
      intmax_t second_operand = pop_back_vector_intmax_t(calc_stack);
      intmax_t first_operand = pop_back_vector_intmax_t(calc_stack);

      if (token.operator == '+')
        push_back_vector_intmax_t(calc_stack, first_operand + second_operand);
      else if (token.operator == '-')
        push_back_vector_intmax_t(calc_stack, first_operand - second_operand);
      else if (token.operator == '*')
        push_back_vector_intmax_t(calc_stack, first_operand * second_operand);
      else if (token.operator == '/')
        push_back_vector_intmax_t(calc_stack, first_operand / second_operand);

      return calc_stack->array[calc_stack->size - 1];
    }
  else // if (token.type == END_OF_INPUT)
    {
      intmax_t result = pop_back_vector_intmax_t(calc_stack);
      delete_vector_intmax_t(calc_stack);
      return result;
    }
}

size_t
priority (Token token)
{
  if ((token.operator == '+') || (token.operator == '-'))
    return 1;
  else
    return 2;
}

Token
get_token (void)
{
  static int previous_char = ' ';
  int current_char;
  Token token;

  current_char = previous_char;
  while (current_char == ' ')
    {
      current_char = getchar();
      if (current_char == '\n')
        current_char = ' ';
    }

  if ((current_char == '+') || (current_char == '-') ||
      (current_char == '*') || (current_char == '/'))
    {
      token.type = OPERATOR;
      token.operator = current_char;
      previous_char = ' ';
      return token;
    }
  else if (current_char == '(')
    {
      token.type = OPEN_BRACKET;
      previous_char = ' ';
      return token;
    }
  else if (current_char == ')')
    {
      token.type = CLOSE_BRACKET;
      previous_char = ' ';
      return token;
    }
  else if (('0' <= current_char) && (current_char <= '9'))
    {
      intmax_t number = current_char - '0';
      while (('0' <= current_char) && (current_char <= '9'))
        {
          current_char = getchar();
          if (('0' <= current_char) && (current_char <= '9'))
            number = number * 10 + (current_char - '0');
        }
      token.type = NUMBER;
      token.number = number;
      previous_char = current_char == '\n' ? ' ' : current_char;
      return token;
    }
  else // if (current_char == EOF)
    {
      token.type = END_OF_INPUT;
      previous_char = EOF;
      return token;
    }
}

VECTOR(Token)

int
main (void)
{
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  Vector_Token sort_stack = new_vector_Token();

  while (true)
  {
    Token token = get_token();

    if (token.type == NUMBER)
      calc(token);
    else if(token.type == OPERATOR)
      {
        while ((sort_stack->size > 0) &&
               (sort_stack->array[sort_stack->size - 1].type == OPERATOR) &&
               (priority(token) < priority(sort_stack->array[sort_stack->size])))
          calc(pop_back_vector_Token(sort_stack));

        push_back_vector_Token(sort_stack, token);
      }
    else if (token.type == OPEN_BRACKET)
      push_back_vector_Token(sort_stack, token);
    else if (token.type == CLOSE_BRACKET)
      {
        while (sort_stack->array[sort_stack->size - 1].type != OPEN_BRACKET)
          calc(pop_back_vector_Token(sort_stack));
        pop_back_vector_Token(sort_stack);
      }
    else if (token.type == END_OF_INPUT)
      {
        while (sort_stack->size > 0)
          calc(pop_back_vector_Token(sort_stack));

        delete_vector_Token(sort_stack);

        printf("%jd\n", calc(token));

        break;
      }
  }

  fclose(stdin);
  fclose(stdout);
  return 0;
}

