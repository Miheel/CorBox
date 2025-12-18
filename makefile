# Directories
BIN = bin
LIB = lib
BUILD = build
SRC = src
INCLUDE = include

SRC_DIRS = $(shell find $(SRC) -type d)
INCL_DIRS = $(shell find $(INCLUDE) -type d)
DEP_DIR = $(BIN)/dep
OBJ_DIR = $(BIN)/objs

SRC_SUBDIRS = $(filter-out $(SRC),$(SRC_DIRS))
OBJ_MIRROR_DIRS = $(patsubst $(SRC)/%,$(OBJ_DIR)/%/,$(SRC_SUBDIRS))
DEP_MIRROR_DIRS = $(patsubst $(SRC)/%,$(DEP_DIR)/%/,$(SRC_SUBDIRS))
DEP_MIRROR_DIRS += $(DEP_DIR)/
OBJ_MIRROR_DIRS += $(OBJ_DIR)/

##add additional includes if needed ex. -I./path/to/include/
INCL =  $(addprefix -I,$(addsuffix / ,$(INCL_DIRS)))

# Executable
EXE = CorBox.exe

# Project structure
PROJECT_STRUCTURE = $(BIN)/. $(BUILD)/. $(OBJ_DIR)/. $(DEP_DIR)/. $(SRC)/. $(INCLUDE)/.

# Files
SRC_FILES = $(strip $(foreach dir,$(SRC_DIRS), $(wildcard $(dir)/*.cpp)))
HEADER_FILES = $(strip $(foreach dir,$(INCL_DIRS), $(wildcard $(dir)/*.hpp)))
OBJ_FILES = $(patsubst $(SRC)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(OBJ_FILES:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)

# Compiler and flags
CXX = g++ 
LANG_STD = -std=c++17

#warning and dependency flags
WARNFLAGS = -Wall -Wpedantic -Werror 
DEP_FLAGS = -MMD -MP

#debug and release flags
DBG_FLAGS = -g -fno-omit-frame-pointer
RELEASE_FLAGS = -O2 -DNDEBUG -flto -ffunction-sections -fdata-sections

# Linker flags
DEBUG_LDFLAGS = -g
RELEASE_LDFLAGS = -Wl,-O1 -Wl,--gc-sections -Wl,--as-needed -s

##Preprocessor flags # -I... and -D... only
CPPFLAGS = $(INCL)
#Compiler flags
CXXFLAGS = $(LANG_STD) $(WARNFLAGS)
##Linker flags
LDFLAGS =

ifneq (,$(filter release,$(MAKECMDGOALS)))
	CXXFLAGS += $(RELEASE_FLAGS)
	LDFLAGS += $(RELEASE_LDFLAGS)
endif
ifneq (,$(filter debug,$(MAKECMDGOALS)))
	CXXFLAGS += $(DBG_FLAGS)
	LDFLAGS += $(DEBUG_LDFLAGS)
endif

#compile and link makros
COMPILE = $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEP_FLAGS) -c
LINK = $(CXX) $(LDFLAGS)

##add additional libraries if needed folder to find libs in -L./path/to/lib/
libpath =
##Specify libs to use ex. -lsfml-graphics
LDLIBS = 
LIB_C = -shared -Wl,-soname,libmean.so.1 -o libmean.so.1.0.1  calc_mean.o

# Targets
.PHONY: all debug release run clean project mirror exe objs help show install lib

all: exe

##debug: CXXFLAGS += $(DBG_FLAGS)
debug: exe

##release: CXXFLAGS += $(RELEASE_FLAGS)
release: exe

##Running the program
##----------------------------------------------------------
run: exe
	./$(BUILD)/$(EXE)

##Clean up the executable and object files
##----------------------------------------------------------
clean:
	rm -rf $(OBJ_DIR)/* $(DEP_DIR)/* $(BUILD)/$(EXE)

##Generating base directory structure for project
##and main file
##----------------------------------------------------------
project: $(PROJECT_STRUCTURE) $(SRC)/main.cpp

$(PROJECT_STRUCTURE):
	mkdir -p $@

$(SRC)/main.cpp:
	[ -f $@ ] || printf '%s\n' '#include <iostream>' '' 'int main()' '{' '	std::cout << "Hello World!";' '' '	return 0;' '}' > $@

##Rule for compiling and generatign objectfiles (.o) files.
##----------------------------------------------------------
objs: $(OBJ_FILES) 
$(OBJ_DIR)/%.o: $(SRC)/%.cpp 
	@mkdir -p $(dir $@) $(DEP_DIR)/$(dir $*)
	$(COMPILE) -MF $(DEP_DIR)/$*.d $< -o $@

-include $(DEP_FILES)

##Rule for creating folder structure in bin/dep and bin/objs folders to mirror that of the src folder.
##----------------------------------------------------------
mirror: $(DEP_MIRROR_DIRS) $(OBJ_MIRROR_DIRS)

$(DEP_MIRROR_DIRS):
	@mkdir -p $@

$(OBJ_MIRROR_DIRS):
	@mkdir -p $@

##Rule for generating the executable.
##----------------------------------------------------------
exe: objs $(BUILD)/$(EXE) 
$(BUILD)/$(EXE): $(OBJ_FILES) | $(BUILD)/.
	$(LINK) $(OBJ_FILES) $(LDLIBS) -o $@

help:
	@echo "================= Commands ================="
	@echo "make [TARGET]"
	@echo "Targets:"
	@echo "  all         Compile and link (default)"
	@echo "  debug       Build with debug flags"
	@echo "  release     Build with release flags"
	@echo "  run         Compile, link, and run"
	@echo "  objs        Compile to object files only"
	@echo "  exe         Link to generate executable"
	@echo "  clean       Remove build artifacts"
	@echo "  project     Generate base project structure"
	@echo "  mirror		 Create bin/dep and bin/objs folder structure"
	@echo "  show        Display current variables and files"
	@echo "  help        Show this message"
	@echo "  install     Install binary to /usr/local/bin"
	@echo "  lib         Build shared library (placeholder)"
	@echo "==========================================="

show:
	@echo "============ Build Info ============"
	@echo "EXE:                $(BUILD)/$(EXE)"
	@echo "Sources:            $(wordlist 1, 5, $(SRC_FILES))..."
	@echo "Headers:            $(wordlist 1, 5, $(HEADER_FILES))..."
	@echo "Objects:            $(wordlist 1, 5, $(OBJ_FILES))..."
	@echo "Dependencies:       $(wordlist 1, 5, $(DEP_FILES))..."
	@echo "Source dirs:        $(SRC_DIRS)"
	@echo "Include dirs:       $(INCL)"
	@echo "Object dirs:        $(OBJ_MIRROR_DIRS)"
	@echo "Dependency dirs:    $(DEP_MIRROR_DIRS)"
	@echo "Compiler:           $(CXX)"
	@echo "Lang standard:      $(LANG_STD)"
	@echo "Error flags:        $(WARNFLAGS)" 
	@echo "Debug flags:        $(DBG_FLAGS)"
	@echo "Release flags:      $(RELEASE_FLAGS)"
	@echo "Debug link:         $(DEBUG_LDFLAGS)"
	@echo "Release link:       $(RELEASE_LDFLAGS)"
	@echo "Preprocessor flags: $(CPPFLAGS)"
	@echo "Compiler flags:     $(CXXFLAGS)"
	@echo "Linker flags:       $(LDFLAGS)"
	@echo "Compile command:    $(COMPILE)"
	@echo "Link command:       $(LINK)"
	@echo "Libs:               $(LDLIBS)"
	@echo "===================================="
