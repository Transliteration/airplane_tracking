LDFLAGS_OPENCV := `pkg-config --libs opencv4`
LDFLAGS_PTHREAD := -pthread -lX11
INCLUDE_OPENCV := `pkg-config --cflags opencv4`
CXXFLAGS := -std=gnu++2a -O3 -Werror -Wall -Wextra 

TARGET_EXEC ?= app

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX)  $(OBJS) -o $@ $(LDFLAGS_OPENCV) $(LDFLAGS_PTHREAD) 

$(BUILD_DIR)/%.cpp.o: %.cpp %.hpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(INCLUDE_OPENCV) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p