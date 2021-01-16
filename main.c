#include "freq_list_reader.h"
#include "printer.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <string.h>

/*
 * Returns an integer in the range [0, n).
 */
int randint(int n);

void handleInterrupt(int);

/*
 * Composes the following steps:
 *      gen a random number
 *      seek to that location, where each word has a weight they sub from the seek number.
 *      print the word to the screen, then scan it in. I think I have to scan in on enter, so one word at a time for now.
 */
int main(int argc, const char** argv) {
    // parse command line options
    const char* filename = argc > 1 ? argv[1] : "wordlist.txt";
    struct freq_list_reader* reader = flr_init(filename);
    if(!reader) {
        perror("could not start up");
        return -1;
    }
    assert(reader);
    int max = flr_max_seek(reader);
    sranddev();
    if(signal(SIGINT, handleInterrupt) == SIG_ERR) {
        return -1;
    }
    printer_init();
    double seconds_elapsed = 0;
    int num_words = 0;
    int total_chars = 0;
    int total_missed_chars = 0;
    while(keep_going) {
        int random = randint(max);
        flr_seek(reader, random);
        char *word = NULL;
        if(flr_read(reader, &word)) {
          assert(false);
          return -1;
        }
        time_t start = time(NULL);
        int num_missed_chars = printer_word(word);
        if(num_missed_chars != -1) {
            time_t end = time(NULL);
            seconds_elapsed += difftime(end, start);
            num_words += 1;
            total_missed_chars += num_missed_chars;
            total_chars += strlen(word);
        }
    }
    printer_close();
    printf("%s\n", "Done! exiting...\n");
    if(num_words) {
        printf("Average WPM: %f\n", num_words / (seconds_elapsed / 60.0));
        double total_chars_d = total_chars;
        double num_missed_chars_d = total_missed_chars;
        printf("Average accuracy: %f%%\n", (total_chars_d - num_missed_chars_d) / total_chars_d * 100.0);
    }
}

void handleInterrupt(int sig) {
    keep_going = false;
}

/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 * from https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
 */
int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    // Supporting larger values for n would requires an even more
    // elaborate implementation that combines multiple calls to rand()
    assert (n <= RAND_MAX);

    // Chop off all of the values that would cause skew...
    int end = RAND_MAX / n; // truncate skew
    assert (end > 0);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}
