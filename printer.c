#include "printer.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <ncurses.h>
volatile int keep_going = 1;
// print in middle is borrowed and slightly modified from Pradeep Padala's Ncurses HOWTO guide. Thank you.
void print_in_middle(WINDOW *win, int starty, int startx, int width, const char *string, const char* prev_str);

#define YELLOW_COLOR_PAIR 1
#define RED_COLOR_PAIR 2

void printer_init() {
    initscr();
    start_color();
    noecho();
    init_pair(YELLOW_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(RED_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
    print_in_middle(stdscr, 0, 0, 0, "Press control-c, then any other character to quit at any time.", "");
}

const char* prev_word = NULL;
uint64_t prev_miss_mask = 0;
int printer_word(const char* word) {
    attron(COLOR_PAIR(1));
    print_in_middle(stdscr, LINES / 2, 0, 0, word, prev_word);
    attroff(COLOR_PAIR(1));
    uint64_t miss_mask = 0;
    bool forward;
    int num_missed_chars = 0;
    /* int x, y; */
    /* getyx(stdscr, y, x); */
    /* mvprintw(0, 1, "Miss mask: %x old mask: %x", miss_mask, prev_miss_mask); */
    /* move(y, x); */
    assert(strlen(word) < sizeof(miss_mask) * 8);
    for(int i = 0; i < strlen(word); i += forward) {
        forward = true;
        int c = getch();
        if(!keep_going)
            return -1;
        uint64_t char_mask = (1 << i);
        if(c == word[i]) {
            // set c's attr to BOLD
            chgat(1, A_BOLD, YELLOW_COLOR_PAIR, NULL);
            uint64_t old_mask = miss_mask;
            miss_mask &= ~char_mask; // set bit for this char to 0
            if(old_mask > miss_mask) {
                // if a bit is actually flipped to 0, then the value must have decreased
                assert(num_missed_chars);
                --num_missed_chars;
            }
        } else if(c == 8 || c == 127) {
            // backspace / delete was pressed
            forward = false;
            if(i) {
                int x, y;
                getyx(stdscr, y, x);
                move(y, x - 1);
                chgat(1, A_NORMAL, YELLOW_COLOR_PAIR, NULL);
                i -= 1; // have to undo the subsequent ++ in the step of the for loop
            }
        } else {
            // set c's attr to BOLD and color to red
            chgat(1, A_BOLD, RED_COLOR_PAIR, NULL);
            uint64_t old_mask = miss_mask;
            miss_mask |= char_mask; // set this char's bit to 1
            if(miss_mask > old_mask) {
                // if a bit actually flipped, the new value will be larger
                ++num_missed_chars;
                assert(num_missed_chars <= strlen(word));
            }
        }
        /* int x, y; */
        /* getyx(stdscr, y, x); */
        /* mvprintw(0, 1, "Miss mask: %x old mask: %x", miss_mask, prev_miss_mask); */
        /* move(x, y); */
        if(forward) {
            int x, y;
            getyx(stdscr, y, x);
            move(y, x+1);
        }
    }
    prev_word = word;
    prev_miss_mask = miss_mask;
    return num_missed_chars;
}

void printer_close() {
    endwin();
}

// printer_word should move to the middle row, left column, print the word in, say, light brown, then accept user input. If the char is correct, it should udpate the char to be bold. If it is not correct, it should change the char to red.

void print_in_middle(WINDOW *win, int starty, int startx, int width, const char *string, const char* prev_str)
{	int length, x, y, rows;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		getmaxyx(stdscr, rows, width);

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + temp; // I always want to start at the left most column. Maybe a constant offset...
    move(y, 0);
    clrtoeol();
    size_t prev_len;
    if(prev_str && (prev_len = strlen(prev_str)) < x) {
        // old word can ft
        int prev_start_x = x - prev_len - 1; // 1 is for space
        mvwprintw(win, y, x - prev_len - 1, "%s ", prev_str);
        for(int i = 0; i < sizeof(prev_miss_mask) * 8; ++i) {
            uint64_t mask = 1 << i;
            bool missed = prev_miss_mask & mask;
            if(missed) {
                mvchgat(y, prev_start_x + i, 1, A_BOLD, RED_COLOR_PAIR, NULL);
            }
        }
    }

	mvwprintw(win, y, x, "%s", string);
    move(y, x);
	refresh();
}
