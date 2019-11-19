CC = g++ -Wall -Werror
OBJS = proc
proc: list_proc.cc list_proc.hpp Process.cc Process.hpp
	$(CC) Process.cc -o ./build/proc list_proc.cc
clean:
	rm -f $(OBJS)