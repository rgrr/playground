#!/usr/bin/env python3
#
# Example showing
# - upload of ELF image
# - execution of main() until return address
# - the coverage output of the image goes to 'default.profraw'
# - analysis:
#   - ~/bin/clang-arm-none-eabi/bin/llvm-profdata merge -sparse default.profraw -o main.profdata
#   - ~/bin/clang-arm-none-eabi/bin/llvm-cov      show  ../../llvm-games/profiling/_build/profiling.elf -instr-profile=main.profdata
# - symbols:
#   - ~/bin/llvm-arm-none-eabi/bin/llvm-objdump --syms ../../llvm-games/profiling/_build/profiling.elf
#

from pyocd.core.helpers import ConnectHelper
from pyocd.core.target import Target
from pyocd.debug.elf.symbols import ELFSymbolProvider
from pyocd.flash.file_programmer import FileProgrammer
import time

import traceback
import logging
logging.basicConfig(level=logging.INFO)


image_name = "../../llvm-games/RTT/_build/RTT.elf"


def wait_for_halt(target):
    go_on = True
    while go_on:
        state = target.get_state()
        if state == Target.State.HALTED:
            go_on = False
        else:
            time.sleep(0.01)

    
session = ConnectHelper.session_with_chosen_probe(unique_id = "E6616407E3646B29", options = {"frequency": 4000000, 
                                                                                             "target_override": "nrf52840"})

with session:
    target = session.target
    target_context = target.get_target_context()

    # Load firmware into device.
    FileProgrammer(session).program(image_name)
    
    # Set ELF file on target.
    target.elf = image_name
    provider = ELFSymbolProvider(target.elf)
    
    # Look up address of main().
    addr = provider.get_symbol_value("main")
    print("main() address: 0x%X" % addr)

    # Set breakpoint.
    print("set breakpoint to entry of main()")
    target.set_breakpoint(addr)

    target.reset_and_halt()
    print("execute")
    target.resume()

    # Wait until breakpoint is hit.
    wait_for_halt(target)

    # Print PC.
    pc = target.read_core_register("pc")
    print("  pc: 0x%X" % pc)
    assert pc == addr & ~0x01                    # mask off LSB
    target.remove_breakpoint(addr)

    # set breakpoint to return address and execute til there
    lr = target.read_core_register("lr")
    print("  lr: 0x%X (return address)" % lr)
    print("set breakpoint to return of main()")
    target.set_breakpoint(lr)
    print("execute")
    target.resume()
    wait_for_halt(target)

    pc = target.read_core_register("pc")
    print("  pc: 0x%X" % pc)
    assert pc == lr & ~0x01                      # mask off LSB
    target.remove_breakpoint(lr)

