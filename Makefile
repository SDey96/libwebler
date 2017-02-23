CXX = g++ --std=c++14 -O3
LIB = -lpthread -lcurl
WEB_LIB = -lwebler
INC = -I include

BUILD_DIR = build
C_BUILD_DIR = $(BUILD_DIR)/crawler

BIN_DIR = bin

C_SRC_DIR = src/crawler
C_INC_DIR = include/crawler
TEST_DIR = test

C_SYS_INC_DIR = /usr/local/include/webler
ARCHIVE_DIR = /usr/local/lib

LIB_NAME = libwebler

# all source files
C_SOURCES = $(shell find $(C_SRC_DIR) -type f -name *.cpp)
# target build files
C_BUILT_OBJECTS = $(patsubst $(C_SRC_DIR)/%,$(C_BUILD_DIR)/%,$(C_SOURCES:.cpp=.o))

_C_INCLUDE = $(shell find $(C_INC_DIR) -type f -name *.hpp)
# include file names
C_INCLUDE = $(patsubst $(C_INC_DIR)/%,%,$(_C_INCLUDE))

install: place_headers build_msg $(C_BUILT_OBJECTS)
	@echo "Creating archive files ..."
	@mkdir -p $(ARCHIVE_DIR)
	@ar -cvq $(ARCHIVE_DIR)/$(LIB_NAME).a $(C_BUILT_OBJECTS)
	@echo ""
	@echo "Process finished ..."
	@echo ""
	@echo "## Usage ##"
	@echo "Include:"
	@echo "#include <webler/WebCrawler.hpp>"
	@echo ""
	@echo "Compile:"
	@echo "To link webler library to your program, use the follow flags"
	@echo "\"--std=c++14 $(WEB_LIB) -lpthread -lcurl\" with your compilation"
	@echo ""
	@echo "g++ -o executable program.cpp --std=c++14 $(WEB_LIB) -lpthread -lcurl"
	@echo ""

# building object files
$(C_BUILD_DIR)/%.o: $(C_SRC_DIR)/%.cpp
	@echo "Building $* ..."
	@mkdir -p $(C_BUILD_DIR)
	@$(CXX) $(LIB) $(INC) -c -o $@ $<

build_msg:
	@echo "Building objects ..."

place_headers:
	@mkdir -p $(C_SYS_INC_DIR)
	@for header in $(C_INCLUDE); do \
		cp $(C_INC_DIR)/$$header $(C_SYS_INC_DIR)/$$header;\
	done

uninstall:
	@rm -rf $(C_SYS_INC_DIR)/
	@rm -f $(ARCHIVE_DIR)/$(LIB_NAME).a

reinstall: uninstall install

# tests
test: test_web_crawler

test_channel: $(BUILD_DIR)/test/test_channel.o
	@echo "\n# Testing channel ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/test_channel $(BUILD_DIR)/test/test_channel.o  $(WEB_LIB) $(LIB)
	./$(BIN_DIR)/test_channel
	@echo ""

$(BUILD_DIR)/test/test_channel.o: $(TEST_DIR)/test_channel.cpp
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -c -o $(BUILD_DIR)/test/test_channel.o $(TEST_DIR)/test_channel.cpp


test_http: $(BUILD_DIR)/test/test_http.o
	@echo "\n# Testing http ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/test_http $(BUILD_DIR)/test/test_http.o $(WEB_LIB).a $(LIB)
	./$(BIN_DIR)/test_http
	@echo ""

$(BUILD_DIR)/test/test_http.o: $(TEST_DIR)/test_http.cpp
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -c -o $(BUILD_DIR)/test/test_http.o $(TEST_DIR)/test_http.cpp

test_web_crawler: $(BUILD_DIR)/test/test_web_crawler.o
	@echo "\n# Testing depth handler ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/test_web_crawler $(BUILD_DIR)/test/test_web_crawler.o $(WEB_LIB) $(LIB)
	./$(BIN_DIR)/test_web_crawler
	@echo ""

$(BUILD_DIR)/test/test_web_crawler.o: $(TEST_DIR)/test_web_crawler.cpp
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -c -o $(BUILD_DIR)/test/test_web_crawler.o $(TEST_DIR)/test_web_crawler.cpp


clean:
	@echo "Cleaning $(BUILD_DIR)/* ..."
	@rm -r $(BUILD_DIR)/*
	@echo "Cleaning $(BIN_DIR)/* ..."
	@rm -r $(BIN_DIR)/*

.PHONY: clean install uninstall reinstall test test_channel test_http test_web_crawler place_headers build_msg
