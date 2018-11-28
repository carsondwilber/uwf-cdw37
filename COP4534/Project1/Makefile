CXX=gcc -std=c11

all:	SpellCheck

SpellCheck:	SpellCheck.o spellChecker.o hashTable.o
	$(CXX) -o $@ $^
	
SpellCheck.o: main.c spellChecker.h hashTable.h
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

spellChecker.o:	spellChecker.c spellChecker.h hashTable.h
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

hashTable.o:	hashTable.c hashTable.h
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -rf *.o
