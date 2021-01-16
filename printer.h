#ifndef PRINTER_H
#define PRINTER_H
void printer_init(); // sets up ncurses, for now
int printer_word(const char* word); // returns 0 if normal, 1 if interrupted
void printer_close(); // cleans up ncurses, for now
// set this to false/0 if you want the printer to stop accepting characters for the current word
// due to current ncurses limitations, a character will still have to be read, so one may have to <C-c> then hit one more character before the update to keep_going is noticed.
extern volatile int keep_going;
#endif
