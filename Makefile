OS       := $(shell uname -s)

# set lib boost shared library name based on OS
ifeq ($(OS),Linux)
	LIB_BOOST := -lboost_system -lboost_thread
endif
ifeq ($(OS),Darwin)
	LIB_BOOST := -lboost_thread-mt
endif

# define compilation variables
CXX      := g++ -std=c++11
CXXFLAGS := -Wall #-Wextra -Werror
#LDFLAGS := -L/usr/local/include -L/usr/local/lib
INCLUDE  := -I/usr/include/rtmidi
LIBS     := -lmonome $(LIB_BOOST) -lrtmidi -lyaml-cpp
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
.PHONY: packages all build clean debug release

# install all packages needed
packages:
ifeq ($(OS),Linux)
	@echo "installing packages for Linux..."
	sudo apt-get update && sudo apt-get dist-upgrade
	sudo apt-get -y install \
		git \
		libconfuse-dev \
		liblo-dev \
		libudev-dev \
		libavahi-compat-libdnssd1 \
		libavahi-compat-libdnssd-dev
	@echo "installing libmonome"
	@mkdir deps && \
	cd deps && \
	git clone https://github.com/monome/libmonome.git && \
	cd libmonome && \
	./waf configure && \
	./waf && \
	sudo ./waf install && \
	git clone https://github.com/monome/serialosc.git && \
	cd serialosc && \
	git submodule init && git submodule update && \
	./waf configure && \
	./waf && \
	sudo ./waf install
	@rm -rf deps
	sudo apt-get -y install \
		libboost-all-dev \
		librtmidi3 \
		librtaudio-dev \
		librtmidi-dev \
		libyaml-cpp-dev
endif
ifeq ($(OS),Darwin)
	@sudo echo "installing packages for OSX..."
endif


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
