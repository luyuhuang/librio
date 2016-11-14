#Makefile for Build rio

CC= gcc -std=gnu99
CFLAGS= -O2 -Wall -fPIC

AR= ar
RANLIB= ranlib

RIO_SO= librio.so
RIO_A= librio.a
RIO_O= comm.o reacter.o reacter_event.o reacter_epoll.o \
	   list.o minheap.o hashmap.o
RIO_INC= include/rio.h

INSTALL_SO= /usr/local/lib
INSTALL_INC= /usr/local/include

all: $(RIO_SO) $(RIO_A)

$(RIO_SO): $(RIO_O)
	$(CC) -shared -o $@ $(RIO_O)

$(RIO_A): $(RIO_O)
	$(AR) $@ $(RIO_O)
	$(RANLIB) $@

comm.o: comm.c comm.h
reacter.o: reacter.c reacter.h reacter_event.h reacter_epoll.h comm.h
reacter_event.o: reacter_event.c reacter_event.h reacter.h
reacter_epoll.o: reacter_epoll.c reacter_epoll.h
list.o: list.c list.h
minheap.o: minheap.c minheap.h
hashmap.o: hashmap.c hashmap.h list.h

clean:
	rm -f $(RIO_A) $(RIO_O) $(RIO_SO)

install:
	install -p -m 0755 $(RIO_SO) $(INSTALL_SO)
	install -p -m 0644 $(RIO_INC) $(INSTALL_INC)

uninstall:

.PHONY: all clean install uninstall
