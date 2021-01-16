#include "freq_list_reader.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Design ideas:
 *      read whole file in init
 *      store two lists- one for words, another for weights.
 *      Seek will find its offset in weights, read translates that to a word.
 *      close frees the list memory.
 */

struct freq_list_reader {
  char **word_arr;
  double *weight_arr;
  int arr_len;
  int cur_idx;
};

struct freq_list_reader *flr_init(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "%s\n", "Could not open file");
    return NULL;
  }
  struct freq_list_reader *reader = malloc(sizeof(struct freq_list_reader));
  if (!reader) {
    fprintf(stderr, "%s\n", "Could not allocate enough memory....how!");
    return NULL;
  }
  if (fscanf(fp, "%d", &reader->arr_len) != 1) {
    fprintf(stderr, "%s\n",
            "Could not read first line that specifies how many lines of the "
            "file to read.");
    return NULL;
  }
  reader->word_arr = malloc(sizeof(char *) * reader->arr_len);
  if (!reader->word_arr) {
    fprintf(stderr, "%s\n", "Could not allocate enough memory....how!");
    return NULL;
  }
  reader->weight_arr = malloc(sizeof(double) * reader->arr_len);
  if (!reader->weight_arr) {
    fprintf(stderr, "%s\n", "Could not allocate enough memory....how!");
    return NULL;
  }
  for (int i = 0; i < reader->arr_len; ++i) {
    char *word = malloc(MAX_WORD_LEN * sizeof(char));
    if (!word || fscanf(fp, "%64s %lf", word, reader->weight_arr + i) != 2) {
      if (word) {
        fprintf(stderr, "Could not parse line %d\n",
                i + 2); // 2 is for converting to 1-indexed and to account for
                        // first line of file
      } else {
        fprintf(stderr, "%s\n", "Could not allocate enough memory....how!");
      }
      reader->arr_len = i;
      flr_close(reader); // cleans things up.
      return NULL;
    }
    reader->word_arr[i] = word;
  }
  if (fclose(fp)) {
    perror("could not close file");
  }
  return reader;
}

int flr_max_seek(struct freq_list_reader *reader) { return reader->arr_len; }

int flr_seek(struct freq_list_reader *reader, int value) {
  assert(value >= 0 && value < reader->arr_len);
  reader->cur_idx = value;
  return 0;
}

int flr_read(struct freq_list_reader *reader, char **word_ptr) {
  *word_ptr = reader->word_arr[reader->cur_idx];
  return 0;
}

void flr_close(struct freq_list_reader *reader) {
  for (int i = 0; i < reader->arr_len; ++i) {
    free(reader->word_arr[i]);
  }
  free(reader->word_arr);
  free(reader->weight_arr);
  free(reader);
}
