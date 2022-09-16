# This Makefile allows easy execution of the several unittest steps within eclipse.
# "compile" / "exec" also allows parsing of the compiler output into the "Problems" view.
#
# Targets
# -------
# create-makefiles-clang   create the Ninja makefiles for clang
# create-makefiles-gcc     create the Ninja makefiles for gcc
# clean                    clean $(BUILD_DIR)
# compile                  compile the tests specified by $(TARGET) or all
# exec                     (compile and) execute the tests specified by $(TARGET) or all
#
# Parameters
# ----------
# cmake_opt     options for cmake execution
# ctest_opt     options for ctest execution
# target        select the test target (file)
#

.ONESHELL:

ifneq ($(findstring msys,$(shell $(MAKE) --version)),)
    MSYS_ROOT        ?= /c/msys64
else
    MSYS_ROOT        ?= /cygdrive/c/msys64
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
    CLANG            := clang
    CLANGXX          := clang++
    GCC              := gcc
    GCCXX            := g++
endif

CMAKE                := cmake
CTEST                := ctest
GREP                 := /usr/bin/grep
RM                   := /bin/rm
WHICH                := /usr/bin/which

PATH_EXT             := $(MSYS_ARCH_FOLDER)/bin:$(MSYS_ROOT)/bin:

#
# set the cmake / ctest options
#
CMAKE_OPT            ?= $(cmake_opt)
CTEST_OPT            ?= $(ctest_opt)
ifneq ($(target),)
    CMAKE_OPT    += --target $(target)
    CTEST_OPT    += -R $(target)
endif


.PHONY: create-makefiles-clang
create-makefiles-clang:
	@export PATH=$(PATH_EXT)$$PATH
	export CC=$$($(WHICH)  $(CLANG))
	export CXX=$$($(WHICH) $(CLANGXX))
	$(RM) -rf $(BUILD_DIR)
	$(CMAKE) $(CMAKE_BUILD_OPT) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$$CC -DCMAKE_CXX_COMPILER=$$CXX -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)"


.PHONY: create-makefiles-gcc
create-makefiles-gcc:
	@export PATH=$(PATH_EXT)$$PATH
	export CC=$$($(WHICH)  $(GCC))
	export CXX=$$($(WHICH) $(GCCXX))
	$(RM) -rf $(BUILD_DIR)
	$(CMAKE) $(CMAKE_BUILD_OPT) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$$CC -DCMAKE_CXX_COMPILER=$$CXX -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)"


# works only if there is a _build.  But otherwise there is nothing to clean...
.PHONY: clean
clean:
	@export PATH=$(PATH_EXT)$$PATH
	$(CMAKE) --build $(BUILD_DIR) --target clean $(CMAKE_OPT) -- -k 0 -v


.PHONY: compile
compile:
	@export PATH=$(PATH_EXT)$$PATH
	$(CMAKE) --build $(BUILD_DIR) $(CMAKE_OPT) -- -k 0 -v


.PHONY: exec
exec:
	@export PATH=$(PATH_EXT)$$PATH
	$(CMAKE) --build    $(BUILD_DIR) $(CMAKE_OPT) -- -k 0 -v
	$(CTEST) --test-dir $(BUILD_DIR) --output-on-failure --output-junit "junit.xml" $(CTEST_OPT) -j 8
