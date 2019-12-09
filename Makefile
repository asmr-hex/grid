# -----------------------------  c o m m o n  -------------------------------


OS       := $(shell uname -s)

# define compilation variables
CXX          := g++ -std=c++17
CXXFLAGS     := -Wall #-Wextra -Werror

#LDFLAGS     := -L/usr/local/include -L/usr/local/lib
INCLUDE_DIR  := ./vendor
INCLUDE      := -I/usr/include/rtmidi

# set lib boost shared library name based on OS
ifeq ($(OS),Linux)
	LIB_BOOST := -lboost_system -lboost_thread
endif
ifeq ($(OS),Darwin)
	LIB_BOOST := -lboost_thread-mt
endif
LIBS         := -lmonome $(LIB_BOOST) -lrtmidi -lyaml-cpp

BUILD        := ./build
OBJ_DIR      := $(BUILD)/objects
BIN_DIR      := $(BUILD)/bin



# -----------------------------  b i n a r y  -------------------------------



BIN_TARGET   := anemone
ANEMONE_SRC  := $(wildcard src/anemone/*cpp)
BIN_SRC      := $(ANEMONE_SRC) src/main.cpp
BIN_OBJECTS  := $(BIN_SRC:%.cpp=$(OBJ_DIR)/%.o)
CONF         := ./conf/config.yml



# -----------------------------  t e s t s  -------------------------------



TEST_DIR             := test
UNIT_TEST_DIR        := $(TEST_DIR)/unit
INTEGRATION_TEST_DIR := $(TEST_DIR)/integration

INCLUDE_TEST         := -I$(INCLUDE_DIR)/catch2 -I$(INCLUDE_DIR)/trompeloeil

UNIT_TEST_TARGET     := run_unit_tests
UNIT_TEST_SRC        := $(ANEMONE_SRC) test/unit/main.cpp
UNIT_TEST_OBJECTS    := $(UNIT_TEST_SRC:%.cpp=$(OBJ_DIR)/%.o)



# -----------------------------  t e s t s  -------------------------------

# list all phony targets, i.e. non-file target commands
.PHONY: default packages all build clean debug test release

# make the all target the default target
default: all

# install all packages needed
# TODO: install catch2 & tromploiel for testing on raspbian!
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


# ---------------------- c o m p i l a t i o n -------------------------------


# compile source files into object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

# compile object files into binary
$(BIN_DIR)/$(BIN_TARGET): $(BIN_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) -o $(BIN_DIR)/$(BIN_TARGET) $(BIN_OBJECTS)

# compile object files into binary
$(BIN_DIR)/$(UNIT_TEST_TARGET): $(UNIT_TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) -o $(BIN_DIR)/$(UNIT_TEST_TARGET) $(UNIT_TEST_OBJECTS)


# by default, running 'make' will create the build directory and
# produce the target binary.
all: build

# create build directories
build: $(BIN_DIR)/$(BIN_TARGET)

# include debugging flags for compilation
debug: CXXFLAGS += -DDEBUG -g
debug: all

# run tests (for now just unit tests)
test: INCLUDE += $(INCLUDE_TEST)
test: $(BIN_DIR)/$(UNIT_TEST_TARGET)
	@$(BIN_DIR)/$(UNIT_TEST_TARGET)


# include release flags for compilation
release: CXXFLAGS += -O2
release: all

# run the compiled binary. compile the binary if not done so already.
run: build
	@$(BIN_DIR)/$(BIN_TARGET) $(CONF)

# clean up
clean:
	-@rm -rvf $(BUILD)

