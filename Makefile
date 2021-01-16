CFLAGS=-Wall -pedantic -Werror -g3
LDFLAGS=-lc -lncurses

main.exe: main.o freq_list_reader.o printer.o
	$(CC)  $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o *.exe
