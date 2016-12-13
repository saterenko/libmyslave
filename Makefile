CC = gcc
LINK = $(CC)
AR = ar

SRC = ./src
BUILD = ./objs
TEST = ./tests

CFLAGS = -g -Wall -std=gnu99

INCS = -I$(SRC) -I/usr/include/mysql

LIBS = -L/usr/lib/mysql

LDLIBS = -lmysqlclient_r -pthread

DEPS = $(SRC)/cor_array.h \
	$(SRC)/cor_pool.h \
	$(SRC)/myslave.h \
	$(SRC)/myslave_table.h \
	$(SRC)/myslave_types.h \
	$(TEST)/cor_test.h

OBJS = $(BUILD)/cor_array.o \
	$(BUILD)/cor_pool.o \
	$(BUILD)/myslave.o \
	$(BUILD)/myslave_table.o

LIB = $(BUILD)/libmyslave.a

TESTS = $(BUILD)/test_myslave

all: prebuild $(OBJS) $(LIB) $(TESTS)

$(BUILD)/libmyslave.a: $(OBJS)
	$(AR) rcs $@ $^

$(BUILD)/cor_array.o: $(DEPS) \
	$(SRC)/cor_array.c
	$(CC) -c $(CFLAGS) $(INCS) -o $(BUILD)/cor_array.o $(SRC)/cor_array.c

$(BUILD)/cor_pool.o: $(DEPS) \
	$(SRC)/cor_pool.c
	$(CC) -c $(CFLAGS) $(INCS) -o $(BUILD)/cor_pool.o $(SRC)/cor_pool.c

$(BUILD)/myslave.o: $(DEPS) \
	$(SRC)/myslave.c
	$(CC) -c $(CFLAGS) $(INCS) -o $(BUILD)/myslave.o $(SRC)/myslave.c

$(BUILD)/myslave_table.o: $(DEPS) \
	$(SRC)/myslave_table.c
	$(CC) -c $(CFLAGS) $(INCS) -o $(BUILD)/myslave_table.o $(SRC)/myslave_table.c

$(BUILD)/test_myslave: \
	$(BUILD)/test_myslave.o
	$(LINK) -o $(BUILD)/test_myslave $(LIBS) $(BUILD)/test_myslave.o $(LDLIBS) $(LIB)

$(BUILD)/test_myslave.o: $(DEPS) \
	$(TEST)/test_myslave.c
	$(CC) -c $(CFLAGS) $(INCS) -o $(BUILD)/test_myslave.o $(TEST)/test_myslave.c

clean:
	rm -rf $(BUILD)

test: all
	$(foreach test, $(TESTS), ./$(test))

prebuild:
	test -d $(BUILD) || mkdir -p $(BUILD)

install: all
	test -d /usr/local/include || mkdir -p /usr/local/include
	cp src/myslave.h /usr/local/include/
	cp objs/libmyslave.a /usr/local/lib/
