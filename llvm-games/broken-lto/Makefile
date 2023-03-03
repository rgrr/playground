BUILD_DIR      := _build/

C_SOURCE_FILES := main.c mod.c
EXE_FILE       := $(BUILD_DIR)main
LD_FILE        := link.ld
 
CLANG          := clang

CFLAGS         := -emit-llvm -flto -c -Wall
LDFLAGS        := -flto -static $(if $(LD_FILE),-T $(LD_FILE))


.ONESHELL:


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)*
	rmdir $(BUILD_DIR)


.PHONY: all
all: $(BUILD_DIR) $(EXE_FILE)


.PHONY: run
run: all
	$(EXE_FILE)


$(BUILD_DIR):
	mkdir -p $@


$(EXE_FILE): $(addprefix $(BUILD_DIR),$(C_SOURCE_FILES:.c=.bc))
	$(CLANG) $(LDFLAGS) -o $@ $?


$(BUILD_DIR)%.bc : %.c
	$(CLANG) $(CFLAGS) -o $@ $<
