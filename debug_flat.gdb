# GDB Script for Debugging Flat Binary init.bin
# Usage: gdb -x debug_flat.gdb

# Setup for flat binary debugging
set architecture i386:x86-64
set confirm off

# Load symbols from the ELF version if available
symbol-file init.elf

# Target remote for QEMU if debugging remotely
# target remote :1234

printf "Setting up flat binary debugging environment...\n"

# Define a command to check for zero RIP condition
define check_rip_zero
    if $rip == 0
        printf "\\n*** ERROR: RIP is 0x0 - Invalid Opcode likely caused by jumping to NULL ***\\n"
        info registers
        x/10i $pc-10
        x/10i $pc
        bt
        # Stop execution to analyze
        interrupt
    end
end

# Define command to examine the entry point
define examine_entry_point
    printf "Examining entry point at 0x400000:\n"
    x/20i 0x400000
    printf "\nExamining stack at 0x400000:\n"
    x/32gx 0x400000
end

# Define command to examine current instruction context
define examine_context
    printf "Current RIP: 0x%016x\n", $rip
    printf "Current RSP: 0x%016x\n", $rsp
    printf "Current RBP: 0x%016x\n", $rbp
    printf "Current CS: 0x%x\n", $cs
    printf "Current RFLAGS: 0x%016x\n", $eflags
    printf "\nInstructions around current RIP:\n"
    x/10i $rip-10
    printf "\nCurrent instruction:\n"
    x/5i $rip
    printf "\nStack dump (top 16 values):\n"
    x/16gx $rsp
end

# Define command to search for potential NULL function pointers
define search_null_ptrs
    printf "Searching for potential NULL function pointers near entry point...\n"
    x/50gx 0x400000
    # This would look for 0x0000000000000000 values which could be NULL function pointers
end

# Set up common breakpoints
break *_start
break *main
break *0x400000  # Entry point based on linker script

# Catch hardware exceptions that might occur
catch throw
catch catch

# Enable detailed disassembly
set disassembly-flavor intel

# Set to stop on signal
handle SIGSEGV stop
handle SIGILL stop
handle SIGFPE stop

printf "Setup complete. Ready to debug flat binary.\n"
printf "Useful commands:\n"
printf "  run                   - Start execution\n"
printf "  continue             - Continue execution\n"
printf "  examine_context      - Show current CPU context\n"
printf "  examine_entry_point  - Show entry point disassembly\n"
printf "  check_rip_zero       - Check if RIP is zero\n"
printf "  search_null_ptrs     - Search for potential NULL pointers\n"
printf "\n"
printf "When debugging with QEMU, use: target remote :1234\n"
printf "before running the binary.\n"