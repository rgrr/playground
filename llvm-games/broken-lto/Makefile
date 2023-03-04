BUILD_DIR      := _build/

C_SOURCE_FILES := main.c mod.c
EXE_FILE       := $(BUILD_DIR)broken-opt
LD_FILE        := link-lld.ld
 
CLANG          := clang-15

CFLAGS   := $(P_CFLAGS)
CFLAGS   += -emit-llvm -Wall -g3 -c
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


.PHONY: test-O0-ok
test-O0-ok:
	$(MAKE) rerun P_CFLAGS=-O0


.PHONY: test-O0-fail-link-error
test-O0-fail-link-error:
	$(MAKE) rerun P_CFLAGS="-O0 -DTEST_INLINE_ITER_GET=1"


.PHONY: test-O1-fail
test-O1-fail:
	$(MAKE) rerun P_CFLAGS=-O1


.PHONY: test-O1-ok-with-observer
test-O1-ok-with-observer:
	$(MAKE) rerun P_CFLAGS="-O1 -DTEST_WITH_ONE_OBSERVER_IS_OK=1"


.PHONY: test-O1-ok-with-inline-iter-get
test-O1-ok-with-inline-iter-get:
	$(MAKE) rerun P_CFLAGS="-O1 -DTEST_INLINE_ITER_GET=1 -DTEST_WITH_ONE_OBSERVER_IS_OK=1"


$(BUILD_DIR):
	mkdir -p $@


$(EXE_FILE): $(addprefix $(BUILD_DIR),$(C_SOURCE_FILES:.c=.bc))
	$(CLANG) $(LDFLAGS) -o $@ $^


$(BUILD_DIR)%.bc : %.c
	$(CLANG) $(CFLAGS) -o $@ $<
