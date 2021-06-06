/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-07 01:43:59
 * @LastEditTime: 2021-06-07 01:45:27
 */

#include <string.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

// Symbol table functions
// Copyright (c) 2019 Warren Toomey, GPL3

static int Globs = 0;		// Position of next free global symbol slot

// Determine if the symbol s is in the global symbol table.
// Return its slot position or -1 if not found.
int findGlob(char *s) {
  int i;

  for (i = 0; i < Globs; i++) {
    if (*s == *Gsym[i].name && !strcmp(s, Gsym[i].name))
      return (i);
  }
  return (-1);
}

// Get the position of a new global symbol slot, or die
// if we've run out of positions.
static int newGlob(void) {
  int p;

  if ((p = Globs++) >= NSYMBOLS)
    fatal("Too many global symbols");
  return (p);
}

// Add a global symbol to the symbol table.
// Return the slot number in the symbol table
int addGlob(char *name) {
  int y;

  // If this is already in the symbol table, return the existing slot
  if ((y = findGlob(name)) != -1)
    return (y);

  // Otherwise get a new slot, fill it in and
  // return the slot number
  y = newGlob();
  Gsym[y].name = strdup(name);
  return (y);
}
