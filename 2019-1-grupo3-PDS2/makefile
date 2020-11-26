CC=g++
STD=c++11

TARGET_NAME=twitter
BUILD_PATH=./build
SOURCE_PATH=./src
LIB_PATH=./lib
TEST_PATH=./test
DATA_PATH=./data

SOURCE_EXT := cpp
SOURCES := $(shell find $(SOURCE_PATH) -maxdepth 2 -type f -name *.cpp ! -name "main.cpp")
OBJECTS := $(patsubst $(SOURCE_PATH)/%,$(BUILD_PATH)/%,$(SOURCES:.$(SOURCE_EXT)=.o))

TEST_SOURCES := $(shell find $(TEST_PATH) -type f -name *.cpp)
TEST_OBJECTS := $(patsubst $(TEST_PATH)/%,$(BUILD_PATH)/%,$(TEST_SOURCES:.$(SOURCE_EXT)=.o))

all: $(TARGET_NAME)

$(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.cpp
	mkdir -p $(@D)
	$(CC) -c -o $@ $<

$(BUILD_PATH)/test.o: $(TEST_PATH)/test.cpp $(OBJECTS)
	$(CC) -c -o $@ $<

$(BUILD_PATH)/test_0_hashtag.o: $(TEST_PATH)/test_0_hashtag.cpp $(OBJECTS)
	$(CC) -c -o $@ $<

$(BUILD_PATH)/test_1_user.o: $(TEST_PATH)/test_1_user.cpp $(OBJECTS)
	$(CC) -c -o $@ $<

$(BUILD_PATH)/test_2_tweet.o: $(TEST_PATH)/test_2_tweet.cpp $(OBJECTS)
	$(CC) -c -o $@ $<

$(BUILD_PATH)/test_3_reply.o: $(TEST_PATH)/test_3_reply.cpp $(OBJECTS)
	$(CC) -c -o $@ $<

$(TARGET_NAME): $(OBJECTS)
	$(CC) -o $(BUILD_PATH)/$(TARGET_NAME) $(SOURCE_PATH)/main.cpp $(OBJECTS)

clean:
	rm -rf $(BUILD_PATH)/*

run:
	$(BUILD_PATH)/twitter

mem:
	valgrind --leak-check=full --show-leak-kinds=all $(BUILD_PATH)/twitter

tests: $(TEST_OBJECTS) $(OBJECTS)
	$(CC) -o $(BUILD_PATH)/test $(BUILD_PATH)/test.o $(OBJECTS)
	mkdir -p $(DATA_PATH)/backup
	mv -f $(DATA_PATH)/*.json $(DATA_PATH)/backup
	./build/test --order-by=file
	rm -rf $(DATA_PATH)/*.json
	mv -f $(DATA_PATH)/backup/*.json $(DATA_PATH)
	rm -f -R $(DATA_PATH)/backup
