CC = g++
CFLAGS = -Wall -Wextra 
LDFLAGS = -lcurl 
TARGET = bin/main
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
