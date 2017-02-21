CXX = g++ --std=c++14
LIB = -lpthread -lcurl
INC = -I include

BUILD_DIR = build
C_BUILD_DIR = $(BUILD_DIR)/crawler

BIN_DIR = bin

C_SRC_DIR = src/crawler
C_INC_DIR = include/crawler
TEST_DIR = test

C_SYS_INC_DIR = /usr/local/include/web
ARCHIVE_DIR = /usr/local/lib/web

# all source files
C_SOURCES = $(shell find $(C_SRC_DIR) -type f -name *.cpp)
# target build files
C_BUILT_OBJECTS = $(patsubst $(C_SRC_DIR)/%,$(C_BUILD_DIR)/%,$(C_SOURCES:.cpp=.o))

_C_INCLUDE = $(shell find $(C_INC_DIR) -type f -name *.hpp)
# include file names
C_INCLUDE = $(patsubst $(C_INC_DIR)/%,%,$(_C_INCLUDE))

# building object files
$(C_BUILD_DIR)/%.o: $(C_SRC_DIR)/%.cpp
	@mkdir -p $(C_BUILD_DIR)
	@$(CXX) $(LIB) $(INC) -c -o $@ $<

install: place_headers build_msg $(C_BUILT_OBJECTS)
	@echo "Creating archive files ..."
	@mkdir -p $(ARCHIVE_DIR)
	@ar -cvq $(ARCHIVE_DIR)/web.a $(C_BUILT_OBJECTS)
	@echo ""
	@echo "Process finished ..."
	@echo ""
	@echo "To link web library to your program, use the follow syntax"
	@echo "use \"--std=c++14 /usr/local/lib/web/web.a -lpthread -lcurl\" with your compilation"
	@echo ""
	@echo "Usage:"
	@echo "#include <web/http.hpp>"
	@echo "#include <web/channel.hpp>"
	@echo "#include <web/crawler.hpp>"
	@echo "g++ -o your_executable your_program.cpp --std=c++14 /usr/local/lib/web/web.a -lpthread -lcurl"
	@echo ""

build_msg:
	@echo "Building object files ..."
	@echo ""

place_headers:
	@mkdir -p $(C_SYS_INC_DIR)
	@for header in $(C_INCLUDE); do \
		cp $(C_INC_DIR)/$$header $(C_SYS_INC_DIR)/$$header;\
	done

uninstall:
	rm -r $(C_SYS_INC_DIR)/
	rm -r $(ARCHIVE_DIR)/

reinstall: uninstall install

# tests
test: test_channel test_http test_depth_handler test_depth_manager
	@echo "\nAll test passed!"

test_channel: $(BUILD_DIR)/test/test_channel.o
	@echo "\n# Testing channel ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/test_channel $(BUILD_DIR)/test/test_channel.o  $(ARCHIVE_DIR)/web.a $(LIB)
	./$(BIN_DIR)/test_channel
	@echo ""

$(BUILD_DIR)/test/test_channel.o: $(TEST_DIR)/test_channel.cpp
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -c -o $(BUILD_DIR)/test/test_channel.o $(TEST_DIR)/test_channel.cpp


test_http: $(BUILD_DIR)/test/test_http.o
	@echo "\n# Testing http ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/test_http $(BUILD_DIR)/test/test_http.o $(ARCHIVE_DIR)/web.a $(LIB)
	./$(BIN_DIR)/test_http
	@echo ""

$(BUILD_DIR)/test/test_http.o: $(TEST_DIR)/test_http.cpp
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -c -o $(BUILD_DIR)/test/test_http.o $(TEST_DIR)/test_http.cpp


test_depth_handler: $(BUILD_DIR)/test/test_depth_handler.o
	@echo "\n# Testing depth handler ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/test_depth_handler $(BUILD_DIR)/test/test_depth_handler.o $(ARCHIVE_DIR)/web.a $(LIB)
	./$(BIN_DIR)/test_depth_handler
	@echo ""

$(BUILD_DIR)/test/test_depth_handler.o: $(TEST_DIR)/test_depth_handler.cpp
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -c -o $(BUILD_DIR)/test/test_depth_handler.o $(TEST_DIR)/test_depth_handler.cpp

test_depth_manager: $(BUILD_DIR)/test/test_depth_manager.o
	@echo "\n# Testing depth handler ..."
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/test_depth_manager $(BUILD_DIR)/test/test_depth_manager.o $(ARCHIVE_DIR)/web.a $(LIB)
	./$(BIN_DIR)/test_depth_manager
	@echo ""

$(BUILD_DIR)/test/test_depth_manager.o: $(TEST_DIR)/test_depth_manager.cpp
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -c -o $(BUILD_DIR)/test/test_depth_manager.o $(TEST_DIR)/test_depth_manager.cpp


clean:
	@echo "Cleaning $(BUILD_DIR)/* ..."
	@rm -r $(BUILD_DIR)/*
	@echo "Cleaning $(BIN_DIR)/* ..."
	@rm -r $(BIN_DIR)/*

clean_test: reinstall test_depth_manager

.PHONY: clean install uninstall reinstall test test_channel test_http test_depth_handler place_headers build_msg clean_test