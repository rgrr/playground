BUILD_DIR      := _build/

C_SOURCE_FILES := main.c mod.c
EXE_FILE       := $(BUILD_DIR)broken-lto
LD_FILE        := link-lld.ld
 
CLANG          := clang-15

#CFLAGS   += -flto
CFLAGS   += -emit-llvm -O1 -Wall -g3 -c
#LDFLAGS  += -flto -Wl,--lto-O2
LDFLAGS  += -fuse-ld=lld-15 -g3 -static $(if $(LD_FILE),-T $(LD_FILE))
LDFLAGS  += -Wl,--gc-sections -Wl,-znostart-stop-gc
LDFLAGS  += -Wl,-O2 -Wl,--icf=safe


.ONESHELL:


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)*
	rmdir -p $(BUILD_DIR)


.PHONY: all
all: $(BUILD_DIR) $(EXE_FILE)


.PHONY: run
run: all
	$(EXE_FILE)


.PHONY: rerun
rerun:
	$(MAKE) clean
	$(MAKE) all
	$(EXE_FILE)


$(BUILD_DIR):
	mkdir -p $@


$(EXE_FILE): $(addprefix $(BUILD_DIR),$(C_SOURCE_FILES:.c=.bc))
	$(CLANG) $(LDFLAGS) -o $@ $^


$(BUILD_DIR)%.bc : %.c
	$(CLANG) $(CFLAGS) -o $@ $<
