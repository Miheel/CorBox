BIN = bin
BUILD = build
SRC = src
SRC_DIR = $(shell find src -type d)
INCL_DIR = $(shell find include -type d)
##add additional includes if needed ex. -I./path/to/include/
INCL =  $(addprefix -I ,$(addsuffix / ,$(INCL_DIR)))

EXE = CorBox.exe

PROJECT_STRUCTURE = $(BIN)/. $(BUILD)/. $(SRC)/. include/.

##SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES = $(strip $(foreach dir,$(SRC_DIR), $(wildcard $(dir)/*.cpp)))
##SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
##HEADER_FILES = $(wildcard $(INCL_DIR)/*.hpp)
##HEADER_FILES = $(shell find $(INCL_DIR) -name "*.hpp")
HEADER_FILES = $(strip $(foreach dir,$(INCL_DIR), $(wildcard $(dir)/*.hpp)))

OBJ_FILES = $(patsubst src/%.cpp,$(BIN)/%.o,$(SRC_FILES))
##add additional libraries if needed folder to find libs in -L./path/to/lib/
libpath =
##Specify libs to use ex. -lsfml-graphics
libs = 
##add DEBUG flag
CXX = g++ -g
LANG_STD = -std=c++17
ERR_FLAGS = -Wall -Wpedantic -Werror 

##Linker flags
LDFLAGS = $(libpath)
#Compiler flags
CXXFLAGS = $(LANG_STD)
##Preprocessor flags
CPPFLAGS = $(ERR_FLAGS) $(INCL) -MMD -MP

COMPILE = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
LINK = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)

all: $(BUILD)/$(EXE)

##Generating base directory structure for project
##and main file
##----------------------------------------------------------
.PHONY: project
project: $(PROJECT_STRUCTURE) $(SRC)/main.cpp
$(PROJECT_STRUCTURE):
	mkdir -p $@
$(SRC)/main.cpp: 
	[ -f $(SRC)/main.cpp ] || echo -e '#include <iostream>\n\nint main()\n{\n\nreturn 0;\n}' > $(SRC)/main.cpp

.SECONDEXPANSION:

##Rule for compiling and generatign objectfiles (.o) files.
##----------------------------------------------------------
objs:$(OBJ_FILES)
$(BIN)/%.o: $(SRC)/%.cpp | $$(@D)/.
	$(COMPILE) $< -o $@

##Rule for creating folder structure in bin folder to mirror that of the src folder.
##----------------------------------------------------------
$(BIN)/%/.:
	mkdir -p $@

##Rule for generating the executable.
##----------------------------------------------------------
exe: $(BUILD)/$(EXE)
$(BUILD)/$(EXE): $(OBJ_FILES) | $$(@D)/.
	$(LINK) $(OBJ_FILES) $(libs) -o $@

##Running the program
##----------------------------------------------------------
run: exe
	./$(BUILD)/$(EXE)

##Clean up the executable and object files
##----------------------------------------------------------
.PHONY: clean
clean:
	rm -rf $(BIN)/* $(BUILD)/$(EXE)

help:
	@echo "===================Commands==================="
	@echo "make [TARGET]"
	@echo "Targets:"
	@echo "run		Run the program"
	@echo "all		Compiling and linking"
	@echo "objs		Generate objectfiles no linking"
	@echo "exe		Linking to generate executable file"
	@echo "clean		Clean objectfiles and executable"	
	@echo "show		Show varialbes and files"	
	@echo "help		Show this message"
	@echo "project		Generate base project structure"
	@echo "==============================================="

show:
	@echo "============Flags and files============"
	@echo "Source files:		"$(SRC_FILES)
	@echo "object files:		"$(OBJ_FILES)
	@echo "Header files:		"$(HEADER_FILES)
	@echo "INCL_DIR:		"$(INCL)
	@echo "SRC_DIR:		"$(SRC_DIR)
	@echo "Compiler:		"$(CXX)
	@echo "Language standard:	"$(LANG_STD)
	@echo "Error flags:		"$(ERR_FLAGS)
	@echo "Compile flags:		"$(COMPILE)
	@echo "Linker flags:		"$(LINK)
	@echo "libs:			"$(libs)
	@echo "======================================="
