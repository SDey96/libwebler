CXX = g++ --std=c++14 -O3
LIB = -lpthread -lcurl
WEB_LIB = -lwebler
INC = -I include

BUILD_DIR = build
C_BUILD_DIR = $(BUILD_DIR)/crawler
D_BUILD_DIR = $(BUILD_DIR)/downloader

BIN_DIR = bin

C_SRC_DIR = src/crawler
C_INC_DIR = include/crawler
D_SRC_DIR = src/downloader
D_INC_DIR = include/downloader
TEST_DIR = test

SYS_INC_DIR = /usr/local/include/webler
ARCHIVE_DIR = /usr/local/lib

LIB_NAME = libwebler

# all source files
C_SOURCES = $(shell find $(C_SRC_DIR) -type f -name *.cpp)
D_SOURCES = $(shell find $(D_SRC_DIR) -type f -name *.cpp)
# target build files
C_BUILT_OBJECTS = $(patsubst $(C_SRC_DIR)/%,$(C_BUILD_DIR)/%,$(C_SOURCES:.cpp=.o))
D_BUILT_OBJECTS = $(patsubst $(D_SRC_DIR)/%,$(D_BUILD_DIR)/%,$(D_SOURCES:.cpp=.o))

_C_INCLUDE = $(shell find $(C_INC_DIR) -type f -name *.hpp)
_D_INCLUDE = $(shell find $(D_INC_DIR) -type f -name *.hpp)
# include file names
C_INCLUDE = $(patsubst $(C_INC_DIR)/%,%,$(_C_INCLUDE))
D_INCLUDE = $(patsubst $(D_INC_DIR)/%,%,$(_D_INCLUDE))

install: place_headers build_msg $(C_BUILT_OBJECTS) $(D_BUILT_OBJECTS)
	@echo ""
	@echo "Creating archive => $(ARCHIVE_DIR)/$(LIB_NAME).a"
	@mkdir -p $(ARCHIVE_DIR)
	@ar -cvq $(ARCHIVE_DIR)/$(LIB_NAME).a $(C_BUILT_OBJECTS) $(D_BUILT_OBJECTS)
	@echo ""
	@printf "Installing desktop app ..."
	@cp desktop_app/webler/webler /usr/local/bin/webler
	@chmod +x /usr/local/bin/webler
	@chown root: /usr/local/bin/webler
	@chmod 755 /usr/local/bin/webler
	@echo "done"
	@echo ""
	@echo "Process finished ..."
	@echo ""
	@echo "########## Usage ##########"
	@echo "Include:"
	@echo "#include <webler/WebCrawler.hpp>"
	@echo "#include <webler/Downloader.hpp>"
	@echo ""
	@echo "Compile:"
	@echo "To link webler library to your program, use the follow flags"
	@echo "\"--std=c++14 $(WEB_LIB) -lpthread -lcurl\" with your compilation"
	@echo ""
	@echo "\$$ g++ -o executable program.cpp --std=c++14 $(WEB_LIB) -lpthread -lcurl"
	@echo ""
	@echo "To use desktop app, type 'webler' in terminal"
	@echo "###########################"

# building object files
$(C_BUILD_DIR)/%.o: $(C_SRC_DIR)/%.cpp
	@printf %s "=> $@ ..."
	@mkdir -p $(C_BUILD_DIR)
	@$(CXX) $(LIB) $(INC) -c -o $@ $<
	@echo "done"

# building object files
$(D_BUILD_DIR)/%.o: $(D_SRC_DIR)/%.cpp
	@printf %s "=> $@ ..."
	@mkdir -p $(D_BUILD_DIR)
	@$(CXX) $(LIB) $(INC) -c -o $@ $<
	@echo "done"

build_msg:
	@echo "Building objects"

place_headers:
	@mkdir -p $(SYS_INC_DIR)
	@for header in $(C_INCLUDE); do \
		echo "Include => $(SYS_INC_DIR)/$$header";\
		cp $(C_INC_DIR)/$$header $(SYS_INC_DIR)/$$header;\
	done
	@mkdir -p $(SYS_INC_DIR)
	@for header in $(D_INCLUDE); do \
		echo "Include => $(SYS_INC_DIR)/$$header";\
		cp $(D_INC_DIR)/$$header $(SYS_INC_DIR)/$$header;\
	done
	@echo ""

uninstall:
	@rm -rf $(SYS_INC_DIR)/
	@rm -f $(ARCHIVE_DIR)/$(LIB_NAME).a
	@rm -f /usr/local/bin/webler

reinstall: uninstall install

# tests
test: test_web_crawler

start_server:
	python test/test_server/server.py

test_directory:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)/test

test_channel: test_directory $(BUILD_DIR)/test/test_channel.o
	@echo "\n# Testing channel ..."
	$(CXX) -o $(BIN_DIR)/test_channel $(BUILD_DIR)/test/test_channel.o  $(WEB_LIB) $(LIB)
	./$(BIN_DIR)/test_channel
	@echo ""

$(BUILD_DIR)/test/test_channel.o: $(TEST_DIR)/test_channel.cpp
	$(CXX) -c -o $(BUILD_DIR)/test/test_channel.o $(TEST_DIR)/test_channel.cpp


test_http: test_directory $(BUILD_DIR)/test/test_http.o
	@echo "\n# Testing http ..."
	$(CXX) -o $(BIN_DIR)/test_http $(BUILD_DIR)/test/test_http.o $(WEB_LIB) $(LIB)
	./$(BIN_DIR)/test_http
	@echo ""

$(BUILD_DIR)/test/test_http.o: $(TEST_DIR)/test_http.cpp
	$(CXX) -c -o $(BUILD_DIR)/test/test_http.o $(TEST_DIR)/test_http.cpp

test_dm: test_directory $(BUILD_DIR)/test/test_dm.o
	@echo "\n# Testing dm ..."
	$(CXX) -o $(BIN_DIR)/test_dm $(BUILD_DIR)/test/test_dm.o $(WEB_LIB) $(LIB)
	./$(BIN_DIR)/test_dm
	@echo ""

$(BUILD_DIR)/test/test_dm.o: $(TEST_DIR)/test_dm.cpp
	$(CXX) -c -o $(BUILD_DIR)/test/test_dm.o $(TEST_DIR)/test_dm.cpp

test_web_crawler: test_directory $(BUILD_DIR)/test/test_web_crawler.o
	@echo "\n# Testing depth handler ..."
	$(CXX) -o $(BIN_DIR)/test_web_crawler $(BUILD_DIR)/test/test_web_crawler.o $(WEB_LIB) $(LIB)
	./$(BIN_DIR)/test_web_crawler
	@echo ""

$(BUILD_DIR)/test/test_web_crawler.o: $(TEST_DIR)/test_web_crawler.cpp
	$(CXX) -c -o $(BUILD_DIR)/test/test_web_crawler.o $(TEST_DIR)/test_web_crawler.cpp

clean:
	@echo "Cleaning $(BUILD_DIR)/* ..."
	@rm -rf $(BUILD_DIR)/*
	@echo "Cleaning $(BIN_DIR)/* ..."
	@rm -rf $(BIN_DIR)/*

tt: reinstall test


.PHONY: clean install uninstall reinstall test test_channel test_http test_web_crawler place_headers build_msg
