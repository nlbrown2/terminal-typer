#ifndef FREQ_LIST_READER_H
#define FREQ_LIST_READER_H
#include <stdio.h>
#define MAX_WORD_LEN 64
/*
 * Abstraction over a frequency list. Works similarly to files due to seek support.
 */

struct freq_list_reader;

// support methods like inc and dec.
// Main loop will gen random number, seek this to it, and read the word at that locaticn.

// inits a freq_list_reader on the provided filename.
// TODO: take an array of bytes instead?
struct freq_list_reader* flr_init(const char* filename);

// gives max value callers can seek to
int flr_max_seek(struct freq_list_reader*);

// seeks to the word at the specified value.
int flr_seek(struct freq_list_reader*, int value);

// reads the word at the current offset.
int flr_read(struct freq_list_reader*, char**);

// cleans everything up.
void flr_close(struct freq_list_reader*);
#endif
