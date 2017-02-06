.PHONY = build build_object clean test

CXX = g++
FLAGS = --std=c++11 -lpthread -lcurl
BUILD_FILES = http.cpp crawler.cpp
BUILT_OBJECTS = http.o crawler.o

build: build_object $(BUILT_OBJECTS)
	@mkdir -p /usr/local/lib/web
	@echo "Creating archive files ..."
	@ar -cvq /usr/local/lib/web/web.a $(BUILT_OBJECTS)
	@cp http.hpp /usr/local/lib/web/http.hpp
	@cp crawler.hpp /usr/local/lib/web/crawler.hpp
	@rm *.o
	@echo ""
	@echo "Process finished ..."
	@echo ""
	@echo "To link web library to your program, use the follow syntax"
	@echo "use \"-lpthread -lcurl /usr/local/lib/web.a\" with your compilation"
	@echo ""
	@echo "Example usage:"
	@echo "# g++ -o your_executable your_program.cpp /usr/local/lib/web/web.a -lpthread -lcurl"
	@echo ""

build_object: $(BUILD_FILES)
	@echo "Building object files ..."
	@echo ""
	@$(CXX) $(FLAGS) -c $(BUILD_FILES) 

test: build
	g++ -o test test.cpp /usr/local/lib/web/web.a $(FLAGS)
	./test

clean:
	rm *.o