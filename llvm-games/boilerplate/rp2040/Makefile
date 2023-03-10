# Makefile to build the boilerplate
#
# The Makefile is a helper construct for Eclipse usage.
#

BUILD_DIR       := _build
PICO_BOARD      := pico_w
PROJECT         := boilerplate_rp2040
USE_CLANG       := 1


CMAKE_FLAGS += -DPROJECT=$(PROJECT)
CMAKE_FLAGS += -DPICO_BOARD=$(PICO_BOARD)
ifeq ($(USE_CLANG),1)
    CMAKE_FLAGS += -DPICO_COMPILER=pico_arm_clang
    #export PICO_TOOLCHAIN_PATH=~/bin/llvm-15.0.2-arm-none-eabi/bin
    # currently pico_arm_clang.cmake has to be patched to make this work
    export PICO_TOOLCHAIN_PATH=~/bin/llvm-arm-none-eabi/bin
endif


.ONESHELL:


.PHONY: clean
clean:
	ninja -C $(BUILD_DIR) -v clean


.PHONY: clean-build
clean-build:
	rm -rf $(BUILD_DIR)


.PHONY: all
all: $(BUILD_DIR)/$(PROJECT).elf
	@echo "--------------------------"
	@arm-none-eabi-size -Ax $(BUILD_DIR)/$(PROJECT).elf | awk '{size=strtonum($$2); addr=strtonum($$3); if (addr>=0x20000000 && addr<0x20040000) ram += size; if (addr>=0x10000000 && addr<0x20000000) rom += size; } END {print "Flash: " rom "  RAM: " ram}'
	@echo "--------------------------"


.PHONY: cmake-create-debug
cmake-create-debug:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $(CMAKE_FLAGS)


.PHONY: cmake-create-release
cmake-create-release:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $(CMAKE_FLAGS)


.PHONY: flash
flash: all
	openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 8000" -c "program {$(BUILD_DIR)/$(PROJECT).elf} verify reset; shutdown;"


.PHONY: $(BUILD_DIR)/$(PROJECT).elf
$(BUILD_DIR)/$(PROJECT).elf:
	ninja -C $(BUILD_DIR) -v all
