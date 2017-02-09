.PHONY = build build_object clean test

CXX = g++
FLAGS = --std=c++11 -lpthread -lcurl
BUILD_FILES = src/http.cpp src/crawler.cpp
BUILT_OBJECTS = http.o crawler.o

install: build_object $(BUILT_OBJECTS)

	@echo "Creating archive files ..."

	@ar -cvq /usr/local/lib/web/web.a $(BUILT_OBJECTS)
	@rm *.o

	@echo ""
	@echo "Process finished ..."
	@echo ""
	@echo "To link web library to your program, use the follow syntax"
	@echo "use \"-lpthread -lcurl /usr/local/lib/web/web.a\" with your compilation"
	@echo ""
	@echo "Example usage:"
	@echo "#include <web/http.hpp>"
	@echo "#include <web/crawler.hpp>"
	@echo "g++ -o your_executable your_program.cpp /usr/local/lib/web/web.a -lpthread -lcurl"
	@echo ""

build_object: $(BUILD_FILES)
	@mkdir -p /usr/local/lib/web
	@mkdir -p /usr/include/web
	@cp headers/http.hpp /usr/include/web/http.hpp
	@cp headers/crawler.hpp /usr/include/web/crawler.hpp
	@cp headers/channel.hpp /usr/include/web/channel.hpp
	@echo "Building object files ..."
	@echo ""
	@$(CXX) $(FLAGS) -c $(BUILD_FILES) 

test:
	g++ -o test test.cpp /usr/local/lib/web/web.a $(FLAGS)
	./test
	rm test

build_test: install test

uninstall:
	rm -r /usr/local/lib/web
	rm -r /usr/include/web

reinstall: uninstall install

clean_test: reinstall test

clean:
	rm *.o
