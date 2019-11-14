CC = g++ -Wall -Werror
OBJS = proc
proc:
	$(CC) -o proc list_proc.cc
clean:
	rm -f $(OBJS)