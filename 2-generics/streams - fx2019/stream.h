#ifndef PARADIGMS_PROBLEMS_STREAMS_STREAM_H_
#define PARADIGMS_PROBLEMS_STREAMS_STREAM_H_

#include <stdbool.h>

typedef struct {
  int x;
  int y;
} Pair;

Pair P(int x, int y);

typedef enum {
	      INTEGERS = 1,
	      STRINGS = 2,
	      PAIRS = 3
} StreamType;

typedef struct {
  StreamType type;
  
  void* _;

  // INTEGERS
  int start;
  int end;
  int inc;

  // STRINGS
  int num_strings;
  int current_string;
  char** strings;
  bool free;

  // Pairs
  int num_pairs;
  int current_pair;
  Pair* pairs;
} Stream;

void StreamInitWithIntegers(Stream* s, int start, int end, int inc);
void StreamInitWithStrings(Stream* s, int num_strings, char** strings);
void StreamInitWithStringsNoFree(Stream* s, int num_strings, char** strings);
void StreamInitWithPairs(Stream* s, int num_pairs, Pair* pairs);

void StreamDestroy(Stream* s);

// Returns pointer to the next element from the stream.
// Returns NULL pointer at the end of the stream.
// Calling StreamGetNext on already fully consumed stream will keep returning NULL.
// Passes ownership of the returned pointer to the caller.
void* StreamGetNext(Stream* s);

#endif // PARADIGMS_PROBLEMS_STREAMS_STREAM_H_
