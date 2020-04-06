DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRFIL := files/

CFLAGS := -I $(DIRHEA) -c -Wall -ansi
LDLIBS := -pthread -std=c++11
CC := g++

all : dirs main

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

wordBrowser: 
	$(CC) -o $(DIROBJ)WordBrowser.o $(DIRSRC)WordBrowser.cpp $(CFLAGS) $(LDLIBS)

ssooiigle: $(DIROBJ)SSOOIIGLE.o 
		$(CC) -o $(DIREXE)SSOOIIGLE $^ $(DIROBJ)WordBrowser.o $(LDLIBS)

$(DIROBJ)SSOOIIGLE.o: $(DIRSRC)SSOOIIGLE.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)


main: wordBrowser ssooiigle


solution:
	./$(DIREXE)SSOOIIGLE example1.txt ley 5

test:
	./$(DIREXE)SSOOIIGLE example2.txt david 3


clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
