# copyright 2021-2022 Mirza Ana-Maria 311CA

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99
DEBUG=-g

# define target
TARGETS=main

build: $(TARGETS)

main: ./*.c
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@

pack:
	zip -FSr 311CA_MirzaAna-Maria_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
