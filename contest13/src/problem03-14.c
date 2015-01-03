/*
    problem03-14 - Cheatmate
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

    Each player can to put the figure under fight another player.
    Tolik has knight.
    Garik has queen.

    They are cheaters?
    Who win?
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct Figure
{
  int x, y;
};

bool
equal (struct Figure a, struct Figure b)
{
  return (a.x == b.x) && (a.y == b.y);
}

bool
knight_hits (struct Figure knight, struct Figure f)
{
  int x = f.x - knight.x;
  int y = f.y - knight.y;

  if (x < 0)
    x = -x;

  if (y < 0)
    y = -y;

  if (x > y)
    {
      int t;
      t = x;
      x = y;
      y = t;
    }

  return (x == 1) && (y == 2);
}

bool
queen_hits (struct Figure queen, struct Figure f)
{
  int x = f.x - queen.x;
  int y = f.y - queen.y;

  if (x < 0)
    x = -x;

  if (y < 0)
    y = -y;

  return (!equal(queen, f)) && ((x == 0) || (y == 0) || (x == y));
}

struct Figure
get_figure (char *s)
{
  struct Figure f;
  f.x = (int)s[0] - (int)'A';
  f.y = (int)s[1] - (int)'1';
  return f;
}

int
main (void)
{
  struct Figure knight, queen;
  knight.x = 0;
  knight.y = 0;
  queen.x = 0;
  queen.y = 0;

  const bool TOLIK = true;
  const bool GARIK = false;
  bool current_player = GARIK;

  struct Figure last_field;
  last_field.x = 7;
  last_field.y = 7;

  char s[3];

  while (strcmp(s, "X"))
  {
    scanf("%s", s);
    if (strcmp(s, "X"))
      {
        if (equal(knight, last_field) || equal(queen, last_field))
          {
            printf("Cheaters\n");
            return 0;
          }

        if (current_player == GARIK)
          {
            queen = get_figure(s);
            if (!knight_hits(knight, queen))
              {
                printf("Cheaters\n");
                return 0;
              }
          }
        else if (current_player == TOLIK)
          {
            knight = get_figure(s);
            if (!queen_hits(queen, knight))
              {
                printf("Cheaters\n");
                return 0;
              }
          }

        current_player = !current_player;
      }
  }


  if (equal(queen, last_field))
    printf("Garik\n");
  else if (equal(knight, last_field))
    printf("Tolik\n");
  else
    printf("Cheaters\n");

  return 0;
}

