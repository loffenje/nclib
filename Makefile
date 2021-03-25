CC = gcc
CFLAGS = -g -Wall

TARGET = main
SRC_DIR = ./src

all: $(TARGET)

$(TARGET): $(SRC_DIR)/$(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/$(TARGET).c

clean:
	$(RM) $(TARGET)

