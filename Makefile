CC := g++
INC := -I Simple-Web-Server-v3.1.1

rewriter: rewriter.o
	$(CC) $^ -o rewriter -lstdc++fs -lpthread

rewriter.o: rewriter.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
