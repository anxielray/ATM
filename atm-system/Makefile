CC = gcc

CFLAGS = -Wall -Wextra -g

LDFLAGS = -lcrypto

SRC_DIR = src

SOURCES = $(wildcard $(SRC_DIR)/*.c)

OBJECTS = $(SOURCES:.c=.o)

TARGET = atm

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

setup:
	mkdir -p data
	touch data/users.txt
	touch data/records.txt
	touch data/notifications.txt
	touch data/temp_notif.txt
	chmod 666 data/users.txt data/records.txt data/notifications.txt data/temp_notif.txt

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean clean_cache all

clean_cache:
	rm -rf data

run: all
	./$(TARGET)

.PHONY: all clean rebuild run setup
