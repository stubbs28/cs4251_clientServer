CC=g++
CCVER=-std=c++11
CFLAGS=-lpthread

OBJDIR=obj
OBJ = server.o client.o

$(OBJDIR)/%.o : %.cpp
	test -d $(OBJDIR) || mkdir $(OBJDIR)
	$(CC) $(CCVER) -c -o $@ $< $(CFLAGS)

main: $(addprefix $(OBJDIR)/, $(OBJ))
	$(CC) $(CCVER) -o server $(OBJDIR)/server.o $(CFLAGS)
	$(CC) $(CCVER) -o client $(OBJDIR)/client.o $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(OBJDIR) *~ core
