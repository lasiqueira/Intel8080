CC = clang++
CFLAGS = -O3 -std=c++17
CFLAGS = -O3
SRC = src/main.cpp
OUT = intel8086

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)