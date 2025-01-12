CC = clang++
CFLAGS = -std=c++17
SRC = $(wildcard src/*.cpp)
OUT = intel8086

all: release

debug: CFLAGS += -g -O0
debug: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)_debug

release: CFLAGS += -O3
release: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)_release

clean:
	rm -f $(OUT)_debug $(OUT)_release