# -----------------------------  c o m m o n  -------------------------------


OS                  := $(shell uname -s)


CXX                 := g++ -std=c++17
CXXFLAGS            := -Wall #-Wextra -Werror


INCLUDE_DIR         := ./vendor
INCLUDE             := -I/usr/include/rtmidi


ifeq ($(OS),Linux)
	LIB_BOOST   := -lboost_system -lboost_thread
endif
ifeq ($(OS),Darwin)
	LIB_BOOST   := -lboost_thread-mt
endif
LIBS                := -lmonome $(LIB_BOOST) -lrtmidi -lyaml-cpp
#LDFLAGS            := -L/usr/local/include -L/usr/local/lib


BUILD_DIR           := ./build
OBJ_DIR             := $(BUILD_DIR)/objects
BIN_DIR             := $(BUILD_DIR)/bin


# -----------------------------  b i n a r y  -----------------------------


SRC_DIR      := src
ANEMONE_DIR  := $(SRC_DIR)/anemone

INCLUDE      += -I$(SRC_DIR)

BIN_TARGET   := anemone
ANEMONE_SRC  := $(shell find $(ANEMONE_DIR) -type f -name '*.cpp')
BIN_SRC      := $(ANEMONE_SRC) $(SRC_DIR)/main.cpp
BIN_OBJECTS  := $(BIN_SRC:%.cpp=$(OBJ_DIR)/%.o)


CONF         := ./conf/config.yml


# -----------------------------  t e s t s  -------------------------------


TEST_DIR             := test
UNIT_TEST_DIR        := $(TEST_DIR)/unit
INTEGRATION_TEST_DIR := $(TEST_DIR)/integration

INCLUDE_TEST         := -I$(INCLUDE_DIR)/catch2 -I$(INCLUDE_DIR)/trompeloeil -I$(TEST_DIR)

UNIT_TEST_TARGET     := run_unit_tests
UNIT_TEST_SRC        := $(ANEMONE_SRC) $(shell find $(UNIT_TEST_DIR) -type f -name '*.cpp')
UNIT_TEST_OBJECTS    := $(UNIT_TEST_SRC:%.cpp=$(OBJ_DIR)/%.o)


# -----------------------------  c o m m a n d s  -------------------------


# list all phony targets, i.e. non-file target commands
.PHONY: all build clean debug default  packages test coverage release


default: build


all: packages clean test clean release


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


$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<


# binary target
$(BIN_DIR)/$(BIN_TARGET): $(BIN_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) -o $(BIN_DIR)/$(BIN_TARGET) $(BIN_OBJECTS)


# test target
$(BIN_DIR)/$(UNIT_TEST_TARGET): $(UNIT_TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(LIBS) -o $(BIN_DIR)/$(UNIT_TEST_TARGET) $(UNIT_TEST_OBJECTS)


# binary target (alias)
build: $(BIN_DIR)/$(BIN_TARGET)


debug: CXXFLAGS += -DDEBUG -g
debug: run


# run tests (for now just unit tests)
test: INCLUDE += $(INCLUDE_TEST)
test: $(BIN_DIR)/$(UNIT_TEST_TARGET)
	@$(BIN_DIR)/$(UNIT_TEST_TARGET) -r compact -s


coverage: CXXFLAGS += -O0 -g --coverage
coverage: test
	@lcov -c -d . -o coverage.info
	@lcov --extract coverage.info "$(PWD)/$(ANEMONE_DIR)/*" -o coverage.info
	@genhtml --legend coverage.info -o coverage
	@open coverage/index.html


release: CXXFLAGS += -O2
release: build


run: build
	@$(BIN_DIR)/$(BIN_TARGET) $(CONF)


clean:
	-@rm -rvf $(BUILD_DIR)

