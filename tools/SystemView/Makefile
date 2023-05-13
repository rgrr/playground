# Makefile to build the boilerplate
#
# The Makefile is a helper construct for Eclipse usage.
#

BUILD_DIR       := _build
PROJECT         := systemview


CMAKE_FLAGS += -DPROJECT=$(PROJECT)
CMAKE_FLAGS += -DTOOLCHAIN_PATH=$(wildcard ~/bin/llvm-arm-none-eabi/bin)


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
	@arm-none-eabi-size -Ax $(BUILD_DIR)/$(PROJECT).elf | awk '{size=strtonum($$2); addr=strtonum($$3); if (addr>=0x20000000 && addr<0x20040000) ram += size; if (addr>0x00000000 && addr<0x10000000) rom += size; } END {print "Flash: " rom "  RAM: " ram}'
	@echo "--------------------------"


.PHONY: cmake-create-debug
cmake-create-debug:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $(CMAKE_FLAGS)


.PHONY: cmake-create-release
cmake-create-release:
	cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $(CMAKE_FLAGS)


.PHONY: flash
flash: all
	#pyocd flash -t nrf52840 -f 8M $(BUILD_DIR)/$(PROJECT).elf
	openocd -f interface/cmsis-dap.cfg -f target/nrf52.cfg -c "adapter speed 8000" -c "program {$(BUILD_DIR)/$(PROJECT).elf} verify reset; shutdown;"


.PHONY: reset
reset: all
	#pyocd reset -t nrf52840
	openocd -f interface/cmsis-dap.cfg -f target/nrf52.cfg -c "adapter speed 8000" -c "init; reset run; exit"


.PHONY: reset-pyocd
reset-pyocd: all
	pyocd reset -t nrf52840


.PHONY: $(BUILD_DIR)/$(PROJECT).elf
$(BUILD_DIR)/$(PROJECT).elf:
	ninja -C $(BUILD_DIR) -v all
