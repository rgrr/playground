#!/usr/bin/env python3
from pyocd.core.helpers import ConnectHelper
from pyocd.flash.file_programmer import FileProgrammer
import time

import logging
logging.basicConfig(level=logging.INFO)

session = ConnectHelper.session_with_chosen_probe(unique_id = "E6614C775B333D35",
                                                  options = {"frequency": 4000000, 
                                                             "target_override": "nrf52840"})

with session:
    board = session.board
    target = board.target
    flash = target.memory_map.get_boot_memory()

    # Load firmware into device.
    FileProgrammer(session).program("../../llvm-games/profiling/_build/profiling.bin")

    # Reset, run.
    target.reset_and_halt()
    
    # Read some registers.
    print("pc: 0x%X" % target.read_core_register("pc"))

    target.step()
    print("pc: 0x%X" % target.read_core_register("pc"))

    target.resume()
    time.sleep(0.2)
    target.halt()
    print("pc: 0x%X" % target.read_core_register("pc"))

    target.reset_and_halt()

    print("pc: 0x%X" % target.read_core_register("pc"))
