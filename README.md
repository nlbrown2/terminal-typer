# Learn Typing
This project is an excuse for me to learn DVORAK and C. The goal is to provide words to type based on the English alphabet, and a corpus of text, i.e. my old programming projects.

## Input
The input to this is a list of words that are sorted by frequency. wordlist.txt comes from [this](https://norvig.com/ngrams/count_1w.txt). All credit for the list belongs to [Peter Norvig](http://norvig.com/). I am currently working on a way to generate this input given a list of files- this should enable anyone to generate this kind of input for any corpus they want. I haven't tested support with non-ASCII characters, but would be willing to incorporate that, if desired.

## Interaction
The program uses ncurses to draw on the screen, one word at a time. Each keystroke is checked for correctness, and will appear red if misspelled, or bold if spelled correctly.

## Output
Once you press control-c, followed by any other key, then the program will end. It should print your average WPM and accuracy. Let me know of any other metrics you might like to see! I'm considering adding support to track peak WPM, least-accurate word, slowest word, and maybe fastest word.
