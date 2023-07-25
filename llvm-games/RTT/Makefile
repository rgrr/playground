# Makefile to build the boilerplate
#
# The Makefile is a helper construct for Eclipse usage.
#
.ONESHELL:

BUILD_DIR       := _build
PROJECT         := RTT


CMAKE_FLAGS += -DPROJECT=$(PROJECT)
CMAKE_FLAGS += -DTOOLCHAIN_PATH=$(wildcard /c/u/b/llvm-arm-none-eabi-17.0.0/bin)
CMAKE_FLAGS += -DTOOLCHAIN_TARGET_CFG=armv7em_soft_nofp.cfg
CMAKE_FLAGS += -DTOOLCHAIN_TARGET=armv7em-none-eabi

UT_MSYS_ROOT         ?= /c/msys64
UT_MSYS_ARCH_FOLDER  ?= $(UT_MSYS_ROOT)/mingw32
UT_PATH_EXT          := /c/u/b/llvm-arm-none-eabi/bin:$(UT_MSYS_ARCH_FOLDER)/bin:$(UT_MSYS_ROOT)/bin:$(UT_MSYS_ROOT)/usr/bin:/c/windows/system32

SHELL := /c/msys64/usr/bin/bash.exe


.PHONY: aaa
aaa:
	export PATH=$(UT_PATH_EXT)
	which bash
	which cmake
	which clang
	which ninja
	echo $(SHELL)
	echo $(MAKE)
	cmake --version
	clang --version
	ninja --version


.PHONY: clean
clean:
	export PATH=$(UT_PATH_EXT)
	ninja -C $(BUILD_DIR) -v clean


.PHONY: clean-build
clean-build:
	export PATH=$(UT_PATH_EXT)
	rm -rf $(BUILD_DIR)


.PHONY: all
all: $(BUILD_DIR)/$(PROJECT).elf
	@export PATH=$(UT_PATH_EXT)
	echo "--------------------------"
	llvm-size -Ax $(BUILD_DIR)/$(PROJECT).elf | awk '{size=strtonum($$2); addr=strtonum($$3); if (addr>=0x20000000 && addr<0x20040000) ram += size; if (addr>0 && addr<0x10000000) rom += size; } END {print "Flash: " rom "  RAM: " ram}'
	echo "--------------------------"


.PHONY: cmake-create-debug
cmake-create-debug:
	export PATH=$(UT_PATH_EXT)
	cmake -B $(BUILD_DIR) -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $(CMAKE_FLAGS)


.PHONY: cmake-create-release
cmake-create-release:
	export PATH=$(UT_PATH_EXT)
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $(CMAKE_FLAGS)


.PHONY: flash
flash: all
	openocd -f interface/cmsis-dap.cfg -f target/nrf52.cfg -c "adapter speed 8000" -c "program {$(BUILD_DIR)/$(PROJECT).elf} verify reset; shutdown;"


.PHONY: $(BUILD_DIR)/$(PROJECT).elf
$(BUILD_DIR)/$(PROJECT).elf:
	@export PATH=$(UT_PATH_EXT)
	#ninja -C $(BUILD_DIR) -v all
	cmake --build $(BUILD_DIR) -v
