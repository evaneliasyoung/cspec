CC         = g++
BASE_FLAGS = -std=c++17
SOURCES    = $(wildcard *.cpp) $(wildcard **/*.cpp)
OBJECTS    = ${SOURCES:%.cpp=%.o}
EXEC       = cspec.exe

release: TARGET_FLAGS = -O3
debug: TARGET_FLAGS = -g -Wall -O0 -W -pedantic-errors

release: EXEC = cspec.rel.exe
debug: EXEC = cspec.dbg.exe
leakcheck: EXEC = cspec.dbg.exe
callgrind: EXEC = cspec.dbg.exe
clean: EXEC = cspec.rel.exe cspec.dbg.exe

default: release
all: clean release run

hostos_wrapper =
ifeq ($(OS),Windows_NT)
	hostos := Windows
else
	hostos := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

libs_wrapper=
ifeq ($(hostos),Windows)
	TARGET_LIBS = -lgdi32 -lversion -lOle32 -lOleAut32 -lwbemuuid
endif
ifeq ($(hostos),Darwin)
endif
ifeq ($(hostos),Linux)
endif

%.o: %.cpp
	$(CC) $(BASE_FLAGS) $(TARGET_FLAGS) -c $< -o $@

release: $(OBJECTS)
	$(CC) $(BASE_FLAGS) $(TARGET_FLAGS) $(OBJECTS) $(TARGET_LIBS) -o $(EXEC)

debug: $(OBJECTS)
	$(CC) $(BASE_FLAGS) $(TARGET_FLAGS) $(OBJECTS) $(TARGET_LIBS) -o $(EXEC)

leakcheck: debug
	valgrind --leak-check=full ./$(EXEC)

callgrind: debug
	valgrind --tool=callgrind ./$(EXEC)

run: release
	./$(EXEC)

clean:
	-@rm -f $(OBJECTS) >/dev/null 2>&1
	-@rm -f $(EXEC) >/dev/null 2>&1
