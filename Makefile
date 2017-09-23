#Makefile for Build rio

CC= gcc -std=gnu99
CFLAGS= -O2 -Wall -fPIC
#CFLAGS= -pg -g -Wall -fPIC
LIBS= -lpthread

AR= ar rc
RANLIB= ranlib

RIO_SO= librio.so
RIO_A= librio.a
RIO_O= comm.o reactor.o reactor_event.o reactor_epoll.o \
	   list.o minheap.o hashmap.o thread_pool.o
RIO_H= rio.h

TEST_RIO_BIN= test/test_rio.out
TEST_RIO_O= test/test_rio.o
TEST_HASHMAP_BIN= test/test_hashmap.out
TEST_HASHMAP_O= test/test_hashmap.o
TEST_MACRO_LIST_BIN= test/test_macro_list.out
TEST_MACRO_LIST_O= test/test_macro_list.o
TEST_THREAD_POOL_O= test/test_thread_pool.o
TEST_THREAD_POOL_BIN= test/test_thread_pool.out

INSTALL_SO= /usr/local/lib
INSTALL_A= /usr/local/lib
INSTALL_H= /usr/local/include

all: $(RIO_SO) $(RIO_A) $(TEST_RIO_BIN) $(TEST_HASHMAP_BIN) $(TEST_MACRO_LIST_BIN) \
	$(TEST_THREAD_POOL_BIN)

$(RIO_SO): $(RIO_O)
	$(CC) -shared -o $@ $(RIO_O)

$(RIO_A): $(RIO_O)
	$(AR) $@ $(RIO_O)
	$(RANLIB) $@

$(TEST_RIO_BIN): $(RIO_O) $(TEST_RIO_O)
	$(CC) -pg -o $@ $(TEST_RIO_O) $(RIO_O) $(LIBS)

$(TEST_HASHMAP_BIN): $(RIO_O) $(TEST_HASHMAP_O)
	$(CC) -o $@ $(TEST_HASHMAP_O) $(RIO_O) $(LIBS)

$(TEST_MACRO_LIST_BIN): $(TEST_MACRO_LIST_O)
	$(CC) -o $@ $(TEST_MACRO_LIST_O)

$(TEST_THREAD_POOL_BIN): $(TEST_THREAD_POOL_O) $(RIO_O)
	$(CC) -o $@ $(TEST_THREAD_POOL_O) $(RIO_O) $(LIBS)

comm.o: comm.c comm.h
reactor.o: reactor.c reactor.h reactor_event.h reactor_epoll.h comm.h
reactor_event.o: reactor_event.c reactor_event.h reactor.h
reactor_epoll.o: reactor_epoll.c reactor_epoll.h
list.o: list.c list.h
minheap.o: minheap.c minheap.h
hashmap.o: hashmap.c hashmap.h macro_list.h
thread_pool.o: thread_pool.h thread_pool.c
test/test_rio.o: test/test_rio.c include/rio.h
test/test_hashmap.o: test/test_hashmap.c hashmap.h
test/test_macro_list.o: test/test_macro_list.c macro_list.h
test/test_thread_pool.o: test/test_thread_pool.c thread_pool.h

test: all
	cd test && valgrind --tool=memcheck --leak-check=full ./test_rio.out

clean:
	rm -f $(RIO_A) $(RIO_O) $(RIO_SO) \
		$(TEST_RIO_O) $(TEST_RIO_BIN) $(TEST_HASHMAP_O) $(TEST_HASHMAP_BIN) \
		$(TEST_MACRO_LIST_O) $(TEST_MACRO_LIST_BIN) test/gmon.out $(TEST_THREAD_POOL_BIN)

install:
	install -p -m 0755 $(RIO_SO) $(INSTALL_SO)
	install -p -m 0644 $(RIO_A) $(INSTALL_A)
	cd include && install -p -m 0644 $(RIO_H) $(INSTALL_H)
	ldconfig

uninstall:
	cd $(INSTALL_SO) && rm $(RIO_SO)
	cd $(INSTALL_A) && rm $(RIO_A)
	cd $(INSTALL_H) && rm $(RIO_H)


.PHONY: all test clean install uninstall
