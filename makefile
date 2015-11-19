CC=g++
CCVER=-std=c++11
CFLAGS=-lpthread

obj/%.o : src/%.cpp
	test -d obj || mkdir obj 
	$(CC) $(CCVER) -c -o $@ $< $(CFLAGS)

main: obj/server.o obj/client.o
	$(CC) $(CCVER) -o server obj/server.o $(CFLAGS)
	$(CC) $(CCVER) -o client obj/client.o $(CFLAGS)

.PHONY: clean

clean:
	rm -rf obj/ server client
