# This Makefile allows easy execution of the several unittest steps within eclipse.
# "build" / "exec" also allows parsing of the compiler output into the "Problems" view.
#
# Targets
# -------
# create-makefiles-clang   create the Ninja makefiles for clang
# create-makefiles-gcc     create the Ninja makefiles for gcc
# clean                    clean $(BUILD_DIR)
# build                    build the tests specified by $(TARGET) or all
# exec                     (build and) execute the tests specified by $(TARGET) or all
# coverage-clang           collect coverage information for clang objects
# coverage-gcc-gcovr       collect coverage information for gcc objects
#
# Parameters
# ----------
# cmake_opt     options for cmake execution
# ctest_opt     options for ctest execution
# target        name of the executable to build / execute
#
# Cygwin / MSYS / Linux
# ---------------------
# The above "UNAME" and "MSYS_ROOT" handling is a little bit tricky.  It works at least on "rg"s
# machine which has a primary Cygwin installation and uses the BlueCOnnect_Tools make for building.
# Also the paths in e.g. "RM" avoids some mysterious effects.  So leave this part alone unless
# there are urgent reasons.
#

.ONESHELL:

ifeq '$(findstring ;,$(PATH))' ';'
    UNAME := Windows
else
    UNAME := $(shell uname 2>/dev/null || echo Unknown)
    UNAME := $(patsubst CYGWIN%,Cygwin,$(UNAME))
    UNAME := $(patsubst MSYS%,MSYS,$(UNAME))
    UNAME := $(patsubst MINGW%,MSYS,$(UNAME))
endif

ifeq ($(UNAME),Cygwin)
    MSYS_ROOT        ?= /cygdrive/c/msys64
else
    MSYS_ROOT        ?= /c/msys64
endif
MSYS_ARCH_FOLDER     ?= $(MSYS_ROOT)/mingw32

BUILD_DIR            ?= _build
CMAKE_BUILD_TYPE     ?= Debug
CMAKE_GENERATOR      ?= Ninja

ifeq ($(OS),Windows_NT)
    # compiler: ".exe" required for MSYS2-cmake
    CLANG            := clang.exe
    CLANGXX          := clang++.exe
    GCC              := gcc.exe
    GCCXX            := g++.exe
else
    # Linux?
    CLANG            := clang
    CLANGXX          := clang++
    GCC              := gcc
    GCCXX            := g++
    MSYS_ROOT        :=
    MSYS_ARCH_FOLDER :=
endif

CMAKE                := cmake
CTEST                := ctest
ECHO                 := echo
FIND                 := /usr/bin/find
GCOV                 := gcov
GREP                 := grep
LLVM_COV             := llvm-cov-14
LLVM_PROFDATA        := llvm-profdata-14
RM                   := /bin/rm
XARGS                := /usr/bin/xargs

ifeq ($(UNAME),Linux)
    GCOVR            := python3 -m gcovr
else
    GCOVR            := py -3 -m gcovr
endif

PATH_EXT             := $(MSYS_ARCH_FOLDER)/bin:$(MSYS_ROOT)/bin:$(MSYS_ROOT)/usr/bin

ifeq ($(UNAME),Linux)
    ABS_PWD          := $(shell realpath .)
else
    ABS_PWD          := $(shell /usr/bin/cygpath -ma .)
endif

# create compile_commands.json
CMAKE_BUILD_OPT      := -DCMAKE_EXPORT_COMPILE_COMMANDS=1


#
# set the cmake / ctest options
#
CMAKE_OPT            ?= $(cmake_opt)
CTEST_OPT            ?= $(ctest_opt)
ifneq ($(target),)
    CMAKE_OPT    += --target $(target)
    CTEST_OPT    += -R $(target) -V
endif


# works only if there is a _build.  But otherwise there is nothing to clean...
.PHONY: clean
clean:
	@export PATH=$(PATH_EXT)$$PATH
	$(CMAKE) --build $(BUILD_DIR) --target clean $(CMAKE_OPT) -- -v


.PHONY: clean-real
clean-real:
	@export PATH=$(PATH_EXT)$$PATH
	$(RM) -rfv $(BUILD_DIR)


.PHONY: build
build:
	@export PATH=$(PATH_EXT)$$PATH
	$(CMAKE) --build $(BUILD_DIR) $(CMAKE_OPT) -- -k 0 -v


.PHONY: exec
exec: build
	@export PATH=$(PATH_EXT)$$PATH
	$(CMAKE) --build    $(BUILD_DIR) $(CMAKE_OPT) -- -k 0 -v             &&  \
	$(CTEST) --test-dir $(BUILD_DIR) --output-on-failure $(CTEST_OPT)


.PHONY: create-makefiles-gcc
create-makefiles-gcc:
	@export PATH=$(PATH_EXT)$$PATH
	export CC=$(GCC)
	export CXX=$(GCCXX)
	$(CMAKE) $(CMAKE_BUILD_OPT) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$$CC -DCMAKE_CXX_COMPILER=$$CXX -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)"


.PHONY: coverage-gcc-gcovr
coverage-gcc-gcovr: exec
	@export PATH=$(PATH_EXT):$$PATH
	cd $(BUILD_DIR)
	$(FIND) . -iname "*.gcda" | $(XARGS) $(GCOV) --branch-counts --branch-probabilities --demangled-names --function-summaries
	$(GCOVR) -g --html-details --exclude-unreachable-branches -o index.html -k --decisions -f ".*/games/.*" -f ".*/modules/.*" -f ".*/unittest/[^_].*"
	echo ...
	echo "open file://$(ABS_PWD)/$(BUILD_DIR)/index.html with browser to view coverage results"


.PHONY: create-makefiles-clang
create-makefiles-clang:
	@export PATH=$(PATH_EXT)$$PATH
	export CC=$(CLANG)
	export CXX=$(CLANGXX)
	$(CMAKE) $(CMAKE_BUILD_OPT) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$$CC -DCMAKE_CXX_COMPILER=$$CXX -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)"


.PHONY: coverage-clang
coverage-clang: exec
	@export PATH=$(PATH_EXT):$$PATH
	cd $(BUILD_DIR)
	EXECS_O=$$($(FIND) . -type f -executable | $(GREP) -v CMakeFiles | $(XARGS) -n 1 $(ECHO) -object)
	$(FIND) . -iname "*.profraw" | $(XARGS) $(LLVM_PROFDATA) merge -sparse -o coverage.profdata
	$(LLVM_COV) show -format=html -output-dir=. $$EXECS_O -instr-profile=coverage.profdata -ignore-filename-regex='.*/googletest/.*'
	# --show-branches=count --show-expansions
	echo ...
	echo "open file://$(ABS_PWD)/$(BUILD_DIR)/index.html with browser to view coverage results"


.PHONY: check-clang
check-clang:
	@export PATH=$(PATH_EXT):$$PATH
	cd $(BUILD_DIR)
	run-clang-tidy
