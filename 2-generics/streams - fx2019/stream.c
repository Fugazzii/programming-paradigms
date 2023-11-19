#include "stream.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Pair P(int x, int y) {
  Pair p;
  p.x = x;
  p.y = y;
  return p;
}

void StreamInitWithIntegers(Stream* s, int start, int end, int inc) {
  s->type = INTEGERS;
  s->_ = malloc(1);
  s->start = start;
  s->end = end;
  s->inc = inc;
}

void StreamInitWithStrings(Stream* s, int num_strings, char** strings) {
  s->type = STRINGS;
  s->_ = malloc(1);
  s->num_strings = num_strings;
  s->current_string = 0;
  s->strings = strings;
  s->free = true;
}

void StreamInitWithStringsNoFree(Stream* s, int num_strings, char** strings) {
  s->type = STRINGS;
  s->_ = malloc(1);
  s->num_strings = num_strings;
  s->current_string = 0;
  s->strings = strings;
  s->free = false;
}

void StreamInitWithPairs(Stream* s, int num_pairs, Pair* pairs) {
  s->type = PAIRS;
  s->_ = malloc(1);
  s->num_pairs = num_pairs;
  s->current_pair = 0;
  s->pairs = pairs;
}

void StreamDestroy(Stream* s) {
  free(s->_);
  switch (s->type) {
  case INTEGERS:
    break;
  case STRINGS:
    if (s->free) {
      for (int i = s->current_string; i < s->num_strings; ++i) {
	free(s->strings[i]);
      }
    }
    break;
  case PAIRS:
    break;
  }
}

void* StreamGetNextInteger(Stream* s) {
  if (s->start >= s->end) {
    return NULL;
  }
  int* value = malloc(sizeof(int));
  assert(value != NULL);
  *value = s->start;
  s->start += s->inc;
  return value;
}

void* StreamGetNextString(Stream* s) {
  if (s->current_string == s->num_strings) {
    return NULL;
  }
  char* value = malloc(sizeof(char*));
  assert(value != NULL);
  value = s->strings[s->current_string];
  ++s->current_string;
  return value;
}

void* StreamGetNextPair(Stream* s) {
  if (s->current_pair == s->num_pairs) {
    return NULL;
  }
  ++s->current_pair;
  return &s->pairs[s->current_pair - 1];
}

void* StreamGetNext(Stream* s) {
  switch (s->type) {
  case INTEGERS:
    return StreamGetNextInteger(s);
  case STRINGS:
    return StreamGetNextString(s);
  case PAIRS:
    return StreamGetNextPair(s);
  }
}

