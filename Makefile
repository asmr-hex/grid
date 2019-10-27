CXX := g++ -std=c++11
CXXFLAGS := -Wall #-Wextra -Werror
LDFLAGS := -L/usr/local/include -L/usr/local/lib 
LIBS := -lmonome -lboost_thread-mt
INCLUDE := -Isrc/
BUILD := ./build
OBJ_DIR := $(BUILD)/objects
BIN_DIR := $(BUILD)/bin
TARGET := grid
SRC := $(wildcard src/*cpp)

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(BIN_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) -o $(BIN_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

run:
	@$(BIN_DIR)/$(TARGET)

clean:
	-@rm -rvf $(BUILD)/


# IDIR=/usr/include/rtaudio
# LDIR=/usr/local/lib

# DEPS=src/*.h $(IDIR)/*.h
# LIBS=-lrtaudio -lmonome

# CFLAGS=-I$(IDIR) -L$(LDIR)

# all: grid

# debug: CFLAGS+=-ggdb3 -Wall
# debug: grid

# Molisam:$(OBJ) $(DEPS)
# $(CC) -o $@ $(SRC) $(CFLAGS) $(LIBS)

# run:
# ./grid

# clean:
# rm grid
