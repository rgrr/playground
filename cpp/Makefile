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
#
# Parameters
# ----------
# cmake_opt     options for cmake execution
# ctest_opt     options for ctest execution
# target        name of the executable to build / execute
#
# Cygwin / MSYS
# -------------
# The above "UNAME" and "MSYS_ROOT" handling is a little bit tricky.  It works at least on "rg"s
# machine which has a primary Cygwin installation and uses the BlueCOnnect_Tools make for building.
# Also the paths in e.g. "RM" avoids some mysterious effects.  So leave this part alone unless
# there are urgent reasons.
#

.ONESHELL:

ifeq '$(findstring ;,$(PATH))' ';'
    UNAME := Windows
else
    _UNAME := $(shell /bin/mount 2>/dev/null || echo Unknown)
    ifeq ($(_UNAME),Unknown)
        UNAME := Unknown
    endif
    ifeq ($(UNAME),)
        ifneq ($(findstring cygdrive,$(_UNAME)),)
            UNAME := Cygwin
        endif
    endif
    ifeq ($(UNAME),)
        UNAME := MSYS
    endif
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

CLANG                := clang
CLANGXX              := clang++
GCC                  := gcc
GCCXX                := g++

CMAKE                := cmake
CTEST                := ctest
RM                   := /bin/rm

PATH_EXT             := $(MSYS_ARCH_FOLDER)/bin:$(MSYS_ROOT)/bin:$(MSYS_ROOT)/usr/bin

#
# set the cmake / ctest options
#
CMAKE_OPT            ?= $(cmake_opt)
CTEST_OPT            ?= $(ctest_opt)
ifneq ($(target),)
    CMAKE_OPT    += --target $(target)
    CTEST_OPT    += -R $(target) -V
endif


.PHONY: aaa
aaa:
	@echo $(SHELL)
	echo $(UNAME)
	echo $(PATH_EXT)
	echo $(MSYS_ROOT)
	echo $(MSYS_ARCH_FOLDER)


.PHONY: create-makefiles-clang
create-makefiles-clang:
	@export PATH=$(PATH_EXT):$$PATH
	$(RM) -rf $(BUILD_DIR)
	$(CMAKE) $(CMAKE_BUILD_OPT) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$(CLANG) -DCMAKE_CXX_COMPILER=$(CLANGXX) -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)"


.PHONY: create-makefiles-gcc
create-makefiles-gcc:
	@export PATH=$(PATH_EXT):$$PATH
	$(RM) -rf $(BUILD_DIR)
	$(CMAKE) $(CMAKE_BUILD_OPT) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$(GCC)   -DCMAKE_CXX_COMPILER=$(GCCXX)   -B $(BUILD_DIR) -G "$(CMAKE_GENERATOR)"


# works only if there is a _build.  But otherwise there is nothing to clean...
.PHONY: clean
clean:
	@export PATH=$(PATH_EXT):$$PATH
	$(CMAKE) --build $(BUILD_DIR) --target clean $(CMAKE_OPT) -- -v


.PHONY: build
build:
	@export PATH=$(PATH_EXT):$$PATH
	$(CMAKE) --build $(BUILD_DIR) $(CMAKE_OPT) -- -k 0 -v


.PHONY: exec
exec:
	@export PATH=$(PATH_EXT):$$PATH
	$(CMAKE) --build    $(BUILD_DIR) $(CMAKE_OPT) -- -k 0 -v                                      &&  \
	$(CTEST) --test-dir $(BUILD_DIR) --output-on-failure $(CTEST_OPT)
