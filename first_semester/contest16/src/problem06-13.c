/*
    problem06-13 - table sort
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

    In the input file input.txt the first line contains the number n, which
    specifies the column number you want to sort in ascending order table that
    is stored in the file after that date. Columns are numbered from zero.

    The data contained in the table are described in the file starting from
    the second line as follows: on each line of the file contains information
    about a single row of the table; the string is described as a sequence of
    fields, where a field is either a quoted string or an integer.

    Fields are separated with a semicolon and an arbitrary number of spaces.
    After the last field in the line there is no semicolon.
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_COLONS = 20;
const size_t MAX_RECORDS = 10 * 1000;
const size_t MAX_STRING_LENGTH = 100;

typedef char *String;
typedef intmax_t Number;

typedef enum
{
  NUMBER,
  STRING
} RecordType;

typedef struct
{
  RecordType type;
  union
  {
    Number number;
    String string;
  };
} Value;

typedef struct _RecordStruct
{
  size_t colons;
  Value *array;
} *Record;

Record
new_record (void)
{
  Record record = (Record) malloc( sizeof(struct _RecordStruct) );
  record->array = (Value *) malloc( sizeof(Value) * MAX_COLONS);
  record->colons = 0;
  return record;
}

void
delete_record (Record record)
{
  for (size_t i = 0; i < record->colons; ++ i)
    if (record->array[i].type == STRING)
      free(record->array[i].string);

  free(record->array);
  free(record);
}

bool
get_record (Record record)
{
  int current_char = ';';

  while ((current_char != '\n') && (current_char != EOF))
  {
    current_char = getchar();

    while (current_char == ' ')
      current_char = getchar();

    if (current_char == EOF)
      {
        if (record->colons == 0)
          return false;
        else
          return true;
      }
    else if (current_char == '"')
      {
        size_t length = 0;
        record->array[record->colons].type = STRING;
        record->array[record->colons].string = (String)
                                               malloc(MAX_STRING_LENGTH + 1);
        do
          {
            current_char = getchar();
            if (current_char != '"')
              {
                record->array[record->colons].string[length] = current_char;
                ++ length;
              }
          }
        while (current_char != '"');

        record->array[record->colons].string[length] = 0;
        current_char = getchar();
      }
    else
      {
        bool negative;
        if (current_char == '-')
        {
          negative = true;
          current_char = getchar();
        }
        else
          negative = false;

        record->array[record->colons].type = NUMBER;
        record->array[record->colons].number = current_char - '0';

        while (('0' <= current_char) && (current_char <= '9'))
          {
            current_char = getchar();

            if (('0' <= current_char) && (current_char <= '9'))
              record->array[record->colons].number =
                record->array[record->colons].number * 10 + current_char - '0';
          }

        if (negative)
          record->array[record->colons].number *= -1;
      }

    ++ record->colons;

    while (current_char == ' ')
      current_char = getchar();
  }

  return true;
}

void
print_record (Record record, bool last_line)
{
  for (size_t i = 0; i < record->colons - 1; ++ i)
    if (record->array[i].type == NUMBER)
      printf("%jd;", record->array[i].number);
    else
      printf("\"%s\";", record->array[i].string);

  if (record->array[record->colons - 1].type == NUMBER)
    printf("%jd", record->array[record->colons - 1].number);
  else
    printf("\"%s\"", record->array[record->colons - 1].string);

  if (!last_line)
    printf("\n");
}

size_t sort_colon;
RecordType sort_type;

int
compar (const void *first_pointer, const void *second_pointer)
{
  Record first_record = *((Record *) first_pointer);
  Record second_record = *((Record *) second_pointer);

  if (sort_type == NUMBER)
    return + first_record->array[sort_colon].number
           - second_record->array[sort_colon].number;
  else
    return strcmp(first_record->array[sort_colon].string,
                  second_record->array[sort_colon].string);
}

int
main (void)
{
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  scanf("%zu\n", &sort_colon);

  Record *table = (Record *) malloc(sizeof(Record) * MAX_RECORDS);

  size_t length = 0;
  bool another_one = true;
  do
    {
      table[length] = new_record();
      if (get_record(table[length]))
        ++ length;
      else
        another_one = false;
    }
  while (another_one);

  if (length > 0)
    {
      sort_type = table[0]->array[sort_colon].type;
      qsort(table, length, sizeof(Record), compar);
    }

  for (size_t i = 0; i < length - 1; ++ i)
    {
      print_record(table[i], false);
      delete_record(table[i]);
    }

  print_record(table[length - 1], true);
  delete_record(table[length - 1]);

  free(table);

  fclose(stdin);
  fclose(stdout);

  return 0;
}

