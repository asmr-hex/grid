CXX      := g++ -std=c++11
CXXFLAGS := -Wall #-Wextra -Werror
#LDFLAGS := -L/usr/local/include -L/usr/local/lib
#INCLUDE := -Isrc/
LIBS     := -lmonome -lboost_thread-mt -lrtmidi -lyaml-cpp
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
BIN_DIR  := $(BUILD)/bin
TARGET   := grid
SRC      := $(wildcard src/*cpp)
CONF     := ./conf/config.yml

# create list of object files to compile
OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# by default, running 'make' will create the build directory and
# produce the target binary.
all: build $(BIN_DIR)/$(TARGET)

# compile source files intpo object files.
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

# compile object files into binary
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) -o $(BIN_DIR)/$(TARGET) $(OBJECTS)

# list all phony targets, i.e. non-file target commands
.PHONY: all build clean debug release

# create build dire tories
build:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

# include debugging flags for compilation
debug: CXXFLAGS += -DDEBUG -g
debug: all

# include release flags for compilation
release: CXXFLAGS += -O2
release: all

# run the compiled binary. compile the binary if not done so already.
run: build $(BIN_DIR)/$(TARGET)
	@$(BIN_DIR)/$(TARGET) $(CONF)

# clean up
clean:
	-@rm -rvf $(BUILD)
