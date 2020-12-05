# specify all source files
SRCS = finalprogramming.c
# specify target here 
TARG = execute
# specify compiler, compile flags, and needed libs
CC = gcc
OPTS = -c
# this translates .c files in src list to .o’s
OBJS= $(SRCS:.c=.o)
# all (is not really needed, but is used to generate the target
all: $(TARG)
# this generates the target executab
$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS)
# this is a generic rule for .o files
%.o: %.c
	$(CC) $(OPTS) -c $< -o $@
# and finally, a clean line
clean:
	rm -f $(OBJS) $(TARG)
