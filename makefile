CC = g++
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lcurl 
TARGET = bin/main
INCLUDE = -Iinclude/htmlcxx
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
