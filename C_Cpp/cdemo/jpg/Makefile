ROOT=..
include $(ROOT)/Make.defines

PROGS =	# jpg_demo

all:	$(PROGS)

%:	%.c $(LIBM)
	$(CC) $(CFLAGS) $@.c -o $(ROOT)/out/jpg_$@ $(LDFLAGS) $(LDLIBS) -lcurl -lm

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o file.hole

include $(ROOT)/Make.libapue.inc
