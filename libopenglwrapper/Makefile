ARCH := $(shell getconf LONG_BIT)
ifeq ($(OS),Windows_NT)
DYN_LIB_EXT := .dll
BUILD_ROOT := Build-Cygwin
else
DYN_LIB_EXT := .so
BUILD_ROOT := Build-Linux
endif

DIAG_FLAG := -fdiagnostics-color=always
#COMPILER := clang++-6.0
COMPILER := g++
CPP_VERSION := -std=c++17
CC := g++ $(CPP_VERSION) $(ARCH_FLAG) $(DIAG_FLAG) \
-fPIC -pedantic -Wall -Wno-ignored-attributes
CC_LINK := g++ $(CPP_VERSION) $(ARCH_FLAG) $(DIAG_FLAG) -shared

DEBUG_FLAGS = -ggdb
RELEASE_FLAGS = -Ofast
PROJECT_NAME = libopenglwrapper
IMP_LIB_NAME = $(PROJECT_NAME).a
DLL_NAME = $(PROJECT_NAME)$(DYN_LIB_EXT)
OUTPUT_DIR := ../$(BUILD_ROOT)
OUTPUT_DIR_DEBUG = $(OUTPUT_DIR)/Debug
OUTPUT_DIR_RELEASE = $(OUTPUT_DIR)/Release

# DEPS
DEPS_HOME := ../deps

# DEPS::SDL2Wrapper
SDL2W_HOME := $(DEPS_HOME)/SDL2Wrapper
SDL2W_LIB_HOME := $(SDL2W_HOME)/libsdl2wrapper
SDL2W_INC := $(SDL2W_LIB_HOME)/inc
SDL2W_INC += -I $(SDL2W_LIB_HOME)/src
SDL2W_DEPS_HOME := $(SDL2W_HOME)/deps
SDL2W_LIB := $(SDL2W_HOME)/$(BUILD_ROOT)
SDL2W_LIB_DEBUG := $(SDL2W_LIB)/Debug/libsdl2wrapper.so
SDL2W_LIB_RELEASE := $(SDL2W_LIB)/Release/libsdl2wrapper.so

# DEPS::SDL2
SDL2_HOME := $(SDL2W_HOME)/deps/SDL2
SDL2_INC := $(SDL2_HOME)/include
SDL2_LIB := $(SDL2_HOME)/$(BUILD_ROOT)/$(PLATFORM)
SDL2_LIB_FILES := $(SDL2_LIB)/libSDL2.a $(SDL2_LIB)/libSDL2main.a
SDL2_LIB_IMPORT := $(SDL2_LIB)/libSDL2-2.0.so.0.7.0

# DEPS::CUL
CUL_HOME := $(SDL2W_DEPS_HOME)/CUL
CUL_INC := $(CUL_HOME)/libcul/inc
CUL_LIB := $(CUL_HOME)/$(BUILD_ROOT)
CUL_LIB_DEBUG := $(CUL_LIB)/Debug/libcul.so
CUL_LIB_RELEASE := $(CUL_LIB)/Release/libcul.so

# HEADERS
HEADERS_INC := -I include
HEADERS_INC += -I src
HEADERS_INC += -I $(CUL_INC)
HEADERS_INC += -I $(SDL2W_INC)
HEADERS_INC += -I $(SDL2_INC)

IMPORT_FILES := $(SDL_LIB_FILES)

OUTPUT_DEBUG = $(OUTPUT_DIR_DEBUG)/$(DLL_NAME)
OUTPUT_RELEASE = $(OUTPUT_DIR_RELEASE)/$(DLL_NAME)
IMP_LIB_DEBUG = $(OUTPUT_DIR_DEBUG)/$(IMP_LIB_NAME)
IMP_LIB_RELEASE = $(OUTPUT_DIR_RELEASE)/$(IMP_LIB_NAME)

IMPORT_GL_LIGBS := -lGLU -lGL -lglut -lGLEW
IMPORT_LIBS_DEBUG := -pthread $(SDL2W_LIB_DEBUG) $(IMPORT_GL_LIGBS)
IMPORT_LIBS_RELEASE := -pthread $(CUL_LIB_RELEASE) $(SDL2W_LIB_RELEASE) $(IMPORT_GL_LIGBS)

CPP_FILES := $(wildcard src/*.cpp)
CPP_FILES += $(wildcard src/*/*.cpp)

OBJ_DEBUG_DIR := obj/debug/
OBJ_DEBUG_FILES := $(addprefix $(OBJ_DEBUG_DIR),$(CPP_FILES:.cpp=.o))

OBJ_RELEASE_DIR := obj/release/
OBJ_RELEASE_FILES := $(addprefix $(OBJ_RELEASE_DIR),$(CPP_FILES:.cpp=.o))

debug:
	@make -C $(SDL2W_LIB_HOME) debug
	@make $(OUTPUT_DEBUG)

release:
	@make -C $(SDL2W_LIB_HOME) release
	@make $(OUTPUT_RELEASE)

$(OUTPUT_DEBUG): $(OBJ_DEBUG_FILES)
	@mkdir -p $(dir $@)
	@echo "HITLER"
	$(CC_LINK) -o $@ $(OBJ_DEBUG_FILES) $(DEBUG_FLAGS) $(IMPORT_LIBS_DEBUG) $(IMPORT_FILES)

$(OUTPUT_RELEASE): $(OBJ_RELEASE_FILES)
	@mkdir -p $(dir $@)
	$(CC_LINK) -o $@ $(OBJ_RELEASE_FILES) $(RELEASE_FLAGS) $(IMPORT_LIBS_RELEASE) $(IMPORT_FILES)

$(OBJ_DEBUG_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(HEADERS_INC) $(COMPILER_FLAGS) $(DEBUG_FLAGS) -c -o $@ $<

$(OBJ_RELEASE_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(HEADERS_INC) $(COMPILER_FLAGS) $(RELEASE_FLAGS) -c -o $@ $<

clean:
	make -C $(SDL2W_LIB_HOME) clean
	rm -rf $(OUTPUT_DIR)

test:
	@echo "All .o files: $(OBJ_DEBUG_FILES)"
	@echo "All .cpp files: $(CPP_FILES)"
	@echo "SDL2: $(HOME_SDL2_LINUX)/"
	@echo "SDL2W_LIB_DEBUG = $(SDL2W_LIB_DEBUG)"
	@echo "CUL_LIB_DEBUG = $(CUL_LIB_DEBUG)"