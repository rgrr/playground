# Makefile to build the toolchain for arm-none-eabi and update the build tools.
#
# Most of the tasks below run under cygwin.
#
# Note that the make targets are all very defensive, i.e. they move their results in temporary directories,
# not the actual target directory (except update-toolchain-vs).
# Update of toolchain and build utilities must be done by the developer himself.
#
# Because there were a lot of problems building under a git-subdirectory, build destination can now be found in
# "BUILD_R=~/tmp-llvm-build".  Be sure to provide enough space on this drive.
#
# Build errors might happen depending on selected version.  Nevertheless they are not critical because they
# happen in test scripts which are actually not relevant for the build process.
#
# cmake targets
# =============
# - vs (*) am Besten getestet!
#   - wenn man unter c:/tmp-llvm-build ausführt, bekommt man Fehler mit python skripten.  Da steht der Pfad mit /cygdrive/c drinnen...
#   - wenn man im git-Repo baut, scheint es zu gehen
#   - wenn man unterhalb von $HOME baut, scheint es zu gehen
# - cygwin
#   - innerhalb des git repos kommt es zu Problemen
#   - ansonsten scheint es zu tun
# - msys
#   - egal, was ich mache, es gibt Probleme mit loadable modules
#   - und ich schaffe es nicht, die abzuschalten
# - Ergebnis:
#   - wenn das build-directory unter ~ ist, dann wird in cygwin checkout/cmake/build für vs/gcc ausgeführt.
#
# Targets
# =======
# all                  build toolchain with vs and collect msys tools
# clean                clean up the mess
# rebuild              make clean; make all
# checkout-llvm        checkout required llvm sources
# clean-gcc            clean gcc/cygwin build directories
# cmake-gcc            do cmake for gcc/cygwin
# build-gcc            build with gcc/cygwin
# collect-gcc          collect the built gcc/cygwin files
# clean-vs             clean vs build directories
# cmake-vs             do cmake for vs
# cmake-vs-force       force cmake for vs
# build-vs             build with vs
# build-vs-force       force build with vs
# test-vs              outputs some information
# collect-vs           collect the built vs files
# collect-msys         collect files of msys2 (build tools)
# update-toolchain-vs  copy all the files to the correct place for committing with git
#
# clean-linux          clean gcc/cygwin build directories
# cmake-linux          do cmake for gcc/cygwin
# build-linux          build with gcc/cygwin
# install-linux        install to $(INSTALL_CLANG_LINUX_R)
#
# Directory Structure
# ===================
# $(BUILD_R)
#     llvm-project         (sparse) git repo of llvm
#     build_clang_gcc      directory for building with cygwin/gcc
#     build_clang_vs       directory for building with visual studio
#     build_msys           directory for the fetched msys tools
#     $(VERSION)-gcc       cygwin/gcc generated files for BTSI toolchain
#     $(VERSION)-vs        vs generated files for BTSI toolchain
#
# Building clang with cygwin
# ==========================
# - executed under cygwin
# - cmake from cygwin
# - make for building
#
# Building clang with Visual Studio
# =================================
# - requires the "Visual Studio Build Tools 2019" - no GUI required
# - executed under cygwin
# - requires external cmake and python
#
# Building clang with msys2 (does actually not work)
# ==================================================
# Executed under msys/mingw64
# Install the following packages
# - pacman -S mingw64/mingw-w64-x86_64-cmake
# - pacman -S tar
# - pacman -S mingw64/mingw-w64-x86_64-gcc
# Other
# - fstab must allow path abbreviation, otherwise cmake will not work
# - msys has problems with tar and symbolic links during download (destination must exist)
# - msys has problems with its own drive notation, cmake e.g. mixes c:/ and /c/ which led to unwanted results
#


# llvm version to build
VERSION      ?= 15.0.7
# valid values are "ld.lld" and "lldb"
EXPERIMENTAL ?= ld.lld
# Release / Debug
CONFIG_TYPE  ?= Release

# some tools required for the vs build
CMAKE_VS     ?= c:/b/cmake/bin/cmake
CMAKE_VS_GEN ?= "Visual Studio 16 2019"
PYTHON_VS    ?= $(shell cmd /c ftype | grep -i "python\.file" | sort | head -n 1 | cut -d'=' -f2 | cut -d'"' --fields=2 | cygpath -f- -m)

# msys directory required for collect-msys
MSYS_ROOT    ?= c:/msys64

########################################################################################################################
#
# From now on everything should be location independent
#

# some tools which must be in the path
# TAR: some directories are excluded, because they use symbolic links.  Contained files are not required.
CMAKE := /usr/bin/cmake
TAR   := /usr/bin/tar xJ -b 1024 --touch --exclude="test/Driver/Inputs"
WGET  := /usr/bin/wget

# build directories
BUILD_R                    := /mnt/d/u/src/llvm
BUILD_DOWNLOAD_P           := llvm-project
BUILD_DOWNLOAD_R           := $(BUILD_R)/$(BUILD_DOWNLOAD_P)
BUILD_CLANG_CYGWIN_R       := $(BUILD_R)/build_clang_gcc
BUILD_CLANG_LINUX_R        := $(BUILD_R)/build_clang_linux
BUILD_CLANG_MSYS2_R        := $(BUILD_R)/build_clang_msys2
BUILD_CLANG_VS_R           := $(BUILD_R)/build_clang_vs
BUILD_MSYS_R               := $(BUILD_R)/build_msys
BUILD_MSYS_P               := $(BUILD_MSYS_R)/usr/lib/perl5/core_perl

INSTALL_CLANG_LINUX_R      := ~/bin/clang-arm-none-eabi

# directories for generated/output files
DST_CLANG_CYGWIN_R         := $(BUILD_R)/$(VERSION)-gcc
SRC_CLANG_CYGWIN_1         := $(BUILD_CLANG_CYGWIN_R)/bin
DST_CLANG_CYGWIN_1         := $(DST_CLANG_CYGWIN_R)/bin
SRC_CLANG_CYGWIN_2         := $(BUILD_CLANG_CYGWIN_R)/lib/clang/$(VERSION)/include
DST_CLANG_CYGWIN_2         := $(DST_CLANG_CYGWIN_R)/lib/clang/$(VERSION)/include
DST_CLANG_LINUX_R          := $(BUILD_R)/$(VERSION)-gcc
SRC_CLANG_LINUX_1          := $(BUILD_CLANG_LINUX_R)/bin
DST_CLANG_LINUX_1          := $(DST_CLANG_LINUX_R)/bin
SRC_CLANG_LINUX_2          := $(BUILD_CLANG_LINUX_R)/lib/clang/$(VERSION)/include
DST_CLANG_LINUX_2          := $(DST_CLANG_LINUX_R)/lib/clang/$(VERSION)/include
DST_CLANG_MSYS2_R          := $(BUILD_R)/$(VERSION)-gcc
SRC_CLANG_MSYS2_1          := $(BUILD_CLANG_MSYS2_R)/bin
DST_CLANG_MSYS2_1          := $(DST_CLANG_MSYS2_R)/bin
SRC_CLANG_MSYS2_2          := $(BUILD_CLANG_MSYS2_R)/lib/clang/$(VERSION)/include
DST_CLANG_MSYS2_2          := $(DST_CLANG_MSYS2_R)/lib/clang/$(VERSION)/include
DST_CLANG_VS_R             := $(BUILD_R)/$(VERSION)-vs
SRC_CLANG_VS_1             := $(BUILD_CLANG_VS_R)/$(CONFIG_TYPE)/bin
DST_CLANG_VS_1             := $(DST_CLANG_VS_R)/bin
SRC_CLANG_VS_2             := $(BUILD_CLANG_VS_R)/$(CONFIG_TYPE)/lib/clang/$(VERSION)/include
DST_CLANG_VS_2             := $(DST_CLANG_VS_R)/lib/clang/$(VERSION)/include

# these files indicate a "successful" target run
CMAKE_CLANG_CYGWIN_OK      := $(BUILD_CLANG_CYGWIN_R)/Makefile
BUILD_CLANG_CYGWIN_OK      := $(SRC_CLANG_CYGWIN_1)/opt.exe
CMAKE_CLANG_LINUX_OK       := $(BUILD_CLANG_LINUX_R)/build.ninja
BUILD_CLANG_LINUX_OK       := $(SRC_CLANG_LINUX_1)/opt
CMAKE_CLANG_MSYS2_OK       := $(BUILD_CLANG_MSYS2_R)/Makefile
BUILD_CLANG_MSYS2_OK       := $(SRC_CLANG_MSYS2_1)/opt.exe
CMAKE_CLANG_VS_OK          := $(BUILD_CLANG_VS_R)/LLVM.sln
BUILD_CLANG_VS_OK          := $(SRC_CLANG_VS_1)/opt.exe

# see https://llvm.org/docs/HowToCrossCompileLLVM.html
CMAKE_CLANG_COMMON_OPTIONS := -DLLVM_DEFAULT_TARGET_TRIPLE=arm-none-eabi                     \
                              -DLLVM_TARGET_ARCH=arm                                         \
                              -DCMAKE_CROSSCOMPILING=True                                    \
                              -DLLVM_TARGETS_TO_BUILD=ARM                                    \
                              -DLLVM_ENABLE_PIC=False                                        \
                              -DLLVM_APPEND_VC_REV=False                                     \
                              -DCMAKE_BUILD_TYPE=$(CONFIG_TYPE)

LLVM_BINARIES := clang.exe clang-check.exe clang-diff.exe clang-format.exe clang-tidy                                                   \
                 llc.exe llvm-ar.exe llvm-as.exe llvm-cov.exe llvm-diff.exe llvm-dis.exe llvm-lib.exe llvm-link.exe llvm-nm.exe         \
                 llvm-objcopy.exe llvm-objdump.exe llvm-profdata.exe llvm-profgen.exe llvm-size.exe llvm-symbolizer.exe opt.exe
CLANG_HEADERS := __stddef_max_align_t.h float.h inttypes.h limits.h stdalign.h stdarg.h stdbool.h stddef.h stdint.h

#LLVM_PROJECTS := clang clang-tools-extra compiler-rt
#LLVM_RUNTIMES := libc libcxx libcxxabi libunwind
LLVM_PROJECTS := clang clang-tools-extra
LLVM_RUNTIMES := libc libcxx libcxxabi
ifneq ($(findstring $(EXPERIMENTAL),ld.lld),)
    LLVM_BINARIES += ld.lld.exe
    LLVM_PROJECTS += lld
endif
ifneq ($(findstring $(EXPERIMENTAL),lldb),)
    LLVM_BINARIES += lldb.exe liblldb.dll
    LLVM_PROJECTS += lldb
    VS_ADD_OPTION += -DPYTHON_EXECUTABLE=$(PYTHON_VS)
endif
CMAKE_CLANG_COMMON_OPTIONS += -DLLVM_ENABLE_PROJECTS="$(shell echo $(strip $(LLVM_PROJECTS)) | tr ' ' ';')"

# does not work yet
CMAKE_CLANG_COMMON_OPTIONS += -DLLVM_ENABLE_RUNTIMES="$(shell echo $(strip $(LLVM_RUNTIMES)) | tr ' ' ';')"

ifneq ($(findstring $(VERSION),8.0.0 8.0.1),)
    VS_ADD_OPTION += -DLLVM_TEMPORARILY_ALLOW_OLD_TOOLCHAIN=True
endif

########################################################################################################################
#
.PHONY: all
all: collect-vs collect-msys


.PHONY: clean
clean:
    ifeq ($(BUILD_R),/)
	    echo "You don't want this.  Really!"
    else
        ifeq ($(BUILD_R),..)
	        echo "You don't want this neither.  Really!"
        else
            ifeq ($(BUILD_R),.)
	            echo "You don't want this neither too.  Really!"
            else
	            -rm -rf $(BUILD_CLANG_CYGWIN_R) $(BUILD_CLANG_LINUX_R) $(BUILD_CLANG_MSYS2_R) $(BUILD_CLANG_VS_R) $(BUILD_MSYS_R)
				@echo "***"
				@echo "*** git repo in $(BUILD_DOWNLOAD_R) must be removed manually"
				@echo "***"
            endif
        endif
    endif


.PHONY: rebuild
rebuild: clean all


########################################################################################################################
#
# Checkout required llvm files
#
#
$(BUILD_DOWNLOAD_R)/.git/config:
	git init $(BUILD_DOWNLOAD_R)
	git -C $(BUILD_DOWNLOAD_R) remote add origin https://github.com/llvm/llvm-project.git
	git -C $(BUILD_DOWNLOAD_R) fetch --depth=1 origin llvmorg-$(VERSION)
	-git -C $(BUILD_DOWNLOAD_R) tag local-$(VERSION) FETCH_HEAD
	git -C $(BUILD_DOWNLOAD_R) checkout local-$(VERSION)

.PHONY: checkout-llvm
checkout-llvm: | $(BUILD_DOWNLOAD_R)/.git/config

########################################################################################################################
#
# Build the toolchain under Linux with gcc/clang
# This runs under and compiles for Linux
# *** wondering why "|" is not required for Windows builds ***
#
# ATTENTION: there is a project on github which does the build process: https://github.com/ARM-software/LLVM-embedded-toolchain-for-Arm.git
#
.PHONY: clean-linux
clean-linux:
	-rm -rf $(BUILD_CLANG_LINUX_R)


.PHONY: aaa
aaa:
	@echo $(BUILD_DOWNLOAD_R)/.git/config
	@echo $(CMAKE_CLANG_LINUX_OK)
	@echo $(BUILD_CLANG_LINUX_OK)


$(CMAKE_CLANG_LINUX_OK): | checkout-llvm
	-rm -rf $(BUILD_CLANG_LINUX_R)
	mkdir -p $(BUILD_CLANG_LINUX_R)
	(cd $(BUILD_CLANG_LINUX_R); $(CMAKE) -G "Ninja"          \
           $(CMAKE_CLANG_COMMON_OPTIONS)                         \
           -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
           -DCMAKE_INSTALL_PREFIX=$(INSTALL_CLANG_LINUX_R)       \
           ../$(BUILD_DOWNLOAD_P)/llvm)

#            -DLLVM_APPEND_VC_REV=False                            \

.PHONY: cmake-linux
cmake-linux: $(CMAKE_CLANG_LINUX_OK)


$(BUILD_CLANG_LINUX_OK): | $(CMAKE_CLANG_LINUX_OK)
	$(CMAKE) --build $(BUILD_CLANG_LINUX_R)


.PHONY: build-linux
build-linux: $(BUILD_CLANG_LINUX_OK)


.PHONY: install-linux
install-linux: | $(BUILD_CLANG_LINUX_OK)
	ninja -C $(BUILD_CLANG_LINUX_R) install
	ninja -C $(BUILD_CLANG_LINUX_R) install-llvm-headers
	#ninja -C $(BUILD_CLANG_LINUX_R) install/strip
	#ninja -C $(BUILD_CLANG_LINUX_R) install-llvm-headers-stripped
	#$(CMAKE) --install $(BUILD_CLANG_LINUX_R)


########################################################################################################################
#
# Build the toolchain with cygwin/gcc/make
# - must be executed within a cygwin window
#
# Problems mit cygwin-cmake 3.6.2
# - Ninja: can no longer build, because ninja complains
#       ninja: error: '/u/src/BlueConnect_LuPDisplay/BlueConnect_Tools/_tools/_doc/build/download/tools/clang/lib/Basic/C:/u/src/BlueConnect_LuPDisplay/.git/modules/BlueConnect_Tools/logs/HEAD', needed by 'tools/clang/lib/Basic/SVNVersion.inc', missing and no known rule to make it
#   Path contains two files!
#   The ninja rule contains something with git from BlueConnect which is obviously wrong
# - Makefile:
#   - first make aborts
#   - second build
#   - in tools/clang/lib/Basic/CMakeFiles/clangBasic.dir/build.make:60 steht auch so ein seltsamer Pfad mit SVNVersion.h
# cygwin-cmake 3.13.1 (experimental) installiert
#
.PHONY: clean-gcc
clean-gcc:
	-rm -rf $(BUILD_CLANG_CYGWIN_R)


$(CMAKE_CLANG_CYGWIN_OK): checkout-llvm
	-rm -rf $(BUILD_CLANG_CYGWIN_R)
	mkdir -p $(BUILD_CLANG_CYGWIN_R)
	(cd $(BUILD_CLANG_CYGWIN_R); $(CMAKE) -G "Unix Makefiles" \
           $(CMAKE_CLANG_COMMON_OPTIONS)                      \
           -DLLVM_APPEND_VC_REV=False                         \
           -DLLVM_PARALLEL_COMPILE_JOBS=4                     \
           ../$(BUILD_DOWNLOAD_P)/llvm)


.PHONY: cmake-gcc
cmake-gcc: $(CMAKE_CLANG_CYGWIN_OK)


$(BUILD_CLANG_CYGWIN_OK): $(CMAKE_CLANG_CYGWIN_OK)
	nice make -j4 -C $(BUILD_CLANG_CYGWIN_R)


.PHONY: build-gcc
build-gcc: $(BUILD_CLANG_CYGWIN_OK)


.PHONY: collect-gcc
collect-gcc: $(BUILD_CLANG_CYGWIN_OK)
	@echo "Deleting $(DST_CLANG_CYGWIN_R)..."
	@rm -rf $(DST_CLANG_CYGWIN_R)
	#
	@mkdir -p $(DST_CLANG_CYGWIN_1)
	@echo "Copying clang binaries to $(DST_CLANG_CYGWIN_1)..."
	@echo $(LLVM_BINARIES) | xargs -n1 echo | xargs -IXX cp -uv $(SRC_CLANG_CYGWIN_1)/XX $(DST_CLANG_CYGWIN_1)/XX
	#
	@mkdir -p $(DST_CLANG_CYGWIN_2)
	@echo "Copying clang header to $(DST_CLANG_CYGWIN_2)..."
	@echo $(CLANG_HEADERS)  | xargs -n1 echo | xargs -IXX cp -uv $(SRC_CLANG_CYGWIN_2)/XX $(DST_CLANG_CYGWIN_2)/XX


########################################################################################################################
#
# Build the toolchain with msys2/gcc/make
#
#
.PHONY: clean-msys2
clean-msys2:
	-rm -rf $(BUILD_CLANG_MSYS2_R)


# LLVM_ENABLE_PLUGINS aus tools/clang/examples/analyzer-plugin/CMakeLists.txt geholt.
# TODO und such da mal nach LLVMHello
#           -DLLVM_INCLUDE_TOOLS=False                                     \
#           -DLLVM_INCLUDE_EXAMPLES=False                                  \
#           -DLLVM_INCLUDE_TESTS=False                                     \
#           -DLLVM_INCLUDE_BENCHMARKS=False                                \
#           -DLLVM_ENABLE_LIBPFM=False                                     \
#           -DLLVM_ENABLE_PLUGINS=False                                    \
#           -DBUILD_SHARED_LIBS=Off                                        \
# cmake/config.guess:812 - handling f�r MSYS ge�ndert
# cmake/modules/HandleLLVMOptions.cmake:870 - LLVM_ENABLE_PLUGINS immer auf OFF
#
$(CMAKE_CLANG_MSYS2_OK): checkout-llvm
	-rm -rf $(BUILD_CLANG_MSYS2_R)
	mkdir -p $(BUILD_CLANG_MSYS2_R)
	(export PATH=/cygdrive/c/msys64/usr/bin; cd $(BUILD_CLANG_MSYS2_R); $(CMAKE) -L -G "Unix Makefiles"           \
           $(CMAKE_CLANG_COMMON_OPTIONS)                                  \
           ../$(BUILD_DOWNLOAD_P)/llvm)
	mkdir -p $(BUILD_CLANG_MSYS2_R)/lib
	touch $(BUILD_CLANG_MSYS2_R)/lib/LLVMHello.dll


.PHONY: cmake-msys2
cmake-msys2: $(CMAKE_CLANG_MSYS2_OK)


#
# LLVMHello produces an error -> "make -k" solves (ehm, ignores) this
#
$(BUILD_CLANG_MSYS2_OK): $(CMAKE_CLANG_MSYS2_OK)
	(export PATH=/cygdrive/c/msys64/usr/bin; cd $(BUILD_CLANG_MSYS2_R); nice make -j8 -k)


.PHONY: build-msys2
build-msys2: $(BUILD_CLANG_MSYS2_OK)


.PHONY: collect-msys2
collect-msys2: $(BUILD_CLANG_MSYS2_OK)
	@echo "Deleting $(DST_CLANG_MSYS2_R)..."
	@rm -rf $(DST_CLANG_MSYS2_R)
	#
	@mkdir -p $(DST_CLANG_MSYS2_1)
	@echo "Copying clang binaries to $(DST_CLANG_MSYS2_1)..."
	@echo $(LLVM_BINARIES) | xargs -n1 echo | xargs -IXX cp -uv $(SRC_CLANG_MSYS2_1)/XX $(DST_CLANG_MSYS2_1)/XX
	#
	@mkdir -p $(DST_CLANG_MSYS2_2)
	@echo "Copying clang header to $(DST_CLANG_MSYS2_2)..."
	@echo $(CLANG_HEADERS)  | xargs -n1 echo | xargs -IXX cp -uv $(SRC_CLANG_MSYS2_2)/XX $(DST_CLANG_MSYS2_2)/XX


########################################################################################################################
#
# Build the toolchain with vs 20xx - if not installed get the free express one
#
# Gosh what a fuck... the vs-cmake also requires a vs-python
#
#
.PHONY: clean-vs
clean-vs:
	-rm -rf $(BUILD_CLANG_VS_R)

$(CMAKE_CLANG_VS_OK): checkout-llvm
	-rm -rf $(BUILD_CLANG_VS_R)
	mkdir -p $(BUILD_CLANG_VS_R)
	(export PATH=; cd $(BUILD_CLANG_VS_R); $(CMAKE_VS) -Thost=x64 -G $(CMAKE_VS_GEN)               \
           $(CMAKE_CLANG_COMMON_OPTIONS)                                                           \
	       -DCMAKE_CONFIGURATION_TYPES=$(CONFIG_TYPE)                                              \
	       -DLLVM_COMPILER_JOBS=2                                                                  \
	       $(VS_ADD_OPTION)                                                                        \
	       ../$(BUILD_DOWNLOAD_P)/llvm)


.PHONY: cmake-vs
cmake-vs: $(CMAKE_CLANG_VS_OK)

.PHONY: cmake-vs-force
cmake-vs-force: clean-vs cmake-vs


$(BUILD_CLANG_VS_OK): $(CMAKE_CLANG_VS_OK)
	-(cd $(BUILD_CLANG_VS_R); $(CMAKE_VS) --build . --config $(CONFIG_TYPE) -j8 -- -lowPriority:True -maxCpuCount:8)

.PHONY: build-vs
build-vs: $(BUILD_CLANG_VS_OK)

.PHONY: build-vs-force
build-vs-force: clean-vs build-vs


.PHONY: collect-vs
collect-vs: $(BUILD_CLANG_VS_OK)
	@echo "Deleting $(DST_CLANG_VS_R)..."
	@rm -rf $(DST_CLANG_VS_R)
	#
	@mkdir -p $(DST_CLANG_VS_1)
	@echo "Copying clang binaries to $(DST_CLANG_VS_1)..."
	@echo $(LLVM_BINARIES) | xargs -n1 echo | xargs -IXX cp -uv $(SRC_CLANG_VS_1)/XX $(DST_CLANG_VS_1)/XX
	#
	@mkdir -p $(DST_CLANG_VS_2)
	@echo "Copying clang header to $(DST_CLANG_VS_2)..."
	@echo $(CLANG_HEADERS)  | xargs -n1 echo | xargs -IXX cp -uv $(SRC_CLANG_VS_2)/XX $(DST_CLANG_VS_2)/XX


.PHONY: test-vs
test-vs:
	@echo python: "$(PYTHON_VS)"
	@echo LLVM_BINARIES: "$(LLVM_BINARIES)"

########################################################################################################################
#
# Get other build tools from an existing msys2 installation
# required packages have to be installed
#
#
MSYS_OTHER_FILES :=                               \
              /usr/bin/ls.exe                     \
              /etc/DIR_COLORS                     \
              /etc/profile.d/bash_completion.sh   \
              /usr/lib/terminfo/78/xterm          \
              /usr/share/terminfo/78/xterm        \
              /usr/share/mintty/themes/xterm

# /etc/fstab, /etc/minttyrc are taken from installation

.PHONY: clean-msys
clean-msys:
	-rm -rf $(BUILD_MSYS_R)

.PHONY: collect-msys
collect-msys:
	cmd.exe /c $(shell cygpath -w $(MSYS_ROOT)/usr/bin/pacman) -Syuu
	@#
	@echo "Creating msys directories..."
	@mkdir -p $(BUILD_MSYS_R)/tmp
	@echo $(sort $(dir $(MSYS_OTHER_FILES))) | xargs -n1 echo | xargs -IXX mkdir -p $(BUILD_MSYS_R)XX
	@#
	@echo "Updating files in /usr/bin..."
	@(/usr/bin/ls -1 ../usr/bin | xargs -IXX cp -uv $(MSYS_ROOT)/usr/bin/XX $(BUILD_MSYS_R)/usr/bin)
	@#
	@echo "Updating other msys files..."
	@echo $(MSYS_OTHER_FILES) | xargs -n1 echo | xargs -IXX cp -uv $(MSYS_ROOT)XX $(BUILD_MSYS_R)XX
	@#
	@echo "Copying perl5 modules..."
	@mkdir -p $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/share/perl5/core_perl/Carp.pm               $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/share/perl5/core_perl/constant.pm           $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/share/perl5/core_perl/Exporter.pm           $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/share/perl5/core_perl/strict.pm             $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/share/perl5/core_perl/XSLoader.pm           $(BUILD_MSYS_P)
	@#
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/Config.pm               $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/Config_heavy.pl         $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/auto/Cwd/Cwd.dll        $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/Cwd.pm                  $(BUILD_MSYS_P)
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/DynaLoader.pm           $(BUILD_MSYS_P)
	@#
	@mkdir -p $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/AmigaOS.pm    $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/Cygwin.pm     $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/Epoc.pm       $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/Functions.pm  $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/Mac.pm        $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/OS2.pm        $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/Unix.pm       $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/VMS.pm        $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec/Win32.pm      $(BUILD_MSYS_P)/File/Spec
	@cp -uv $(MSYS_ROOT)/usr/lib/perl5/core_perl/File/Spec.pm            $(BUILD_MSYS_P)/File
	@#
	@mkdir -p $(BUILD_MSYS_P)/warnings
	@cp -uv $(MSYS_ROOT)/usr/share/perl5/core_perl/warnings/register.pm  $(BUILD_MSYS_P)/warnings
	@cp -uv $(MSYS_ROOT)/usr/share/perl5/core_perl/warnings.pm           $(BUILD_MSYS_P)

########################################################################################################################
#
# update all the files at the correct place
#
# ** THIS IS ACTUALLY A DANGEROUS OPERATION BECAUSE IT REMOVES SUBDIRECTORIES **
#
#
.PHONY: update-toolchain-vs
update-toolchain-vs: collect-vs collect-msys
	@rm -rfv ../usr/*
	@cp -av $(BUILD_MSYS_R)/usr ..
	@cp -av $(BUILD_MSYS_R)/etc ..
	@rm -rfv ../../_tools_new
	@cp -av $(DST_CLANG_VS_R) ../../_tools_new
