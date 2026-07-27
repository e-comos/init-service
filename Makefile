# ============================================================================
# init-service Makefile
# init-service: freestanding 64-bit flat binary loaded at 0x400000
# ebts:         separate binary loaded at runtime to 0x500000
# drivers:      separate binary loaded by GRUB module to 0x700000
# ============================================================================

CC      = gcc
AS      = nasm
LD      = ld
OBJCOPY = objcopy

ARCH = x86_64
BITS = 64

TARGET       = init.bin
TARGET_ELF   = init.elf
EBTS_BIN     = ebts.bin
EBTS_ELF     = ebts.elf
DRIVERS_BIN  = drivers.bin
DRIVERS_ELF  = drivers.elf

SRCDIR  = src
INCDIR  = include
TESTDIR = tests
OBJDIR  = build
EBTS_DIR    = ../ebts
DRIVERS_DIR = ../drivers

# ============================================================================
# init-service sources (freestanding, no stdlib)
# ============================================================================
INIT_C_SRCS = \
    $(SRCDIR)/init.c \
    $(SRCDIR)/init_entry.c \
    $(SRCDIR)/syscalls.c \
    $(SRCDIR)/ebts_loader.c \
    $(SRCDIR)/driver_loader.c \
    $(SRCDIR)/boot_animation.c \
    $(SRCDIR)/boot_splash.c

INIT_ASM_SRCS = $(SRCDIR)/start.asm

INIT_OBJS = \
    $(OBJDIR)/init.o \
    $(OBJDIR)/init_entry.o \
    $(OBJDIR)/syscalls.o \
    $(OBJDIR)/ebts_loader.o \
    $(OBJDIR)/driver_loader.o \
    $(OBJDIR)/boot_animation.o \
    $(OBJDIR)/boot_splash.o \
    $(OBJDIR)/start.o

INIT_CFLAGS = \
    -m$(BITS) \
    -nostdlib \
    -nostdinc \
    -ffreestanding \
    -fno-builtin \
    -fno-stack-protector \
    -mcmodel=kernel \
    -mno-red-zone \
    -I$(INCDIR) \
    -g \
    -O2 \
    -Wall \
    -Wextra

ASFLAGS  = -f elf64
LDFLAGS  = -m elf_$(ARCH) -nostdlib -T link.ld

# ============================================================================
# EBTS sources (normal C, builds as separate binary at 0x500000)
# ============================================================================
EBTS_C_SRCS = \
    $(EBTS_DIR)/src/main.c \
    $(EBTS_DIR)/src/shell/shell.c \
    $(EBTS_DIR)/src/shell/command.c \
    $(EBTS_DIR)/src/shell/object.c \
    $(EBTS_DIR)/src/shell/user.c \
    $(EBTS_DIR)/src/shell/commandbox.c \
    $(EBTS_DIR)/src/utils/memory.c \
    $(EBTS_DIR)/src/utils/string.c

EBTS_OBJS = \
    $(OBJDIR)/ebts_main.o \
    $(OBJDIR)/ebts_shell.o \
    $(OBJDIR)/ebts_command.o \
    $(OBJDIR)/ebts_object.o \
    $(OBJDIR)/ebts_user.o \
    $(OBJDIR)/ebts_commandbox.o \
    $(OBJDIR)/ebts_memory.o \
    $(OBJDIR)/ebts_string.o

EBTS_CFLAGS = \
    -Wall \
    -Wextra \
    -std=c99 \
    -O2 \
    -DECOMOS_BUILD \
    -I$(INCDIR)

EBTS_LDFLAGS = -Wl,-Ttext=0x500000

# ============================================================================
# Drivers sources (EightVGA / system drivers, built at 0x700000)
# ============================================================================
DRIVERS_C_SRCS = \
    $(DRIVERS_DIR)/src/main.c

DRIVERS_OBJS = \
    $(OBJDIR)/drivers_main.o

DRIVERS_CFLAGS = \
    -Wall \
    -Wextra \
    -std=c99 \
    -O2 \
    -DECOMOS_BUILD \
    -I$(INCDIR)

DRIVERS_LDFLAGS = -Wl,-Ttext=0x700000

# ============================================================================
# Build targets
# ============================================================================
.PHONY: all clean tests ebts drivers

all: $(OBJDIR) $(TARGET)

ebts: $(OBJDIR) $(EBTS_BIN)

drivers: $(OBJDIR) $(DRIVERS_BIN)

# ---- init-service flat binary ----
$(TARGET): $(TARGET_ELF)
	@echo "[OBJCOPY] Creating flat binary: $@"
	@$(OBJCOPY) -O binary $< $@
	@echo "[INFO] Binary size:" `stat -c %s $@` "bytes"

$(TARGET_ELF): $(INIT_OBJS) link.ld
	@echo "[LD] Linking: $@"
	@$(LD) $(LDFLAGS) -o $@ $(INIT_OBJS)

# ---- EBTS standalone binary ----
$(EBTS_BIN): $(EBTS_ELF)
	@echo "[OBJCOPY] Creating EBTS flat binary: $@"
	@$(OBJCOPY) -O binary $< $@
	@echo "[INFO] EBTS size:" `stat -c %s $@` "bytes"

$(EBTS_ELF): $(EBTS_OBJS)
	@echo "[LD] Linking EBTS: $@"
	@$(CC) $(EBTS_LDFLAGS) -o $@ $(EBTS_OBJS)

# ---- Drivers standalone binary ----
$(DRIVERS_BIN): $(DRIVERS_ELF)
	@echo "[OBJCOPY] Creating Drivers flat binary: $@"
	@$(OBJCOPY) -O binary $< $@
	@echo "[INFO] Drivers size:" `stat -c %s $@` "bytes"

$(DRIVERS_ELF): $(DRIVERS_OBJS)
	@echo "[LD] Linking Drivers: $@"
	@$(CC) $(DRIVERS_LDFLAGS) -o $@ $(DRIVERS_OBJS)

# ---- Compile rules ----
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "[CC] $<"
	@$(CC) $(INIT_CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.asm
	@echo "[AS] $<"
	@$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR)/ebts_main.o: $(EBTS_DIR)/src/main.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_shell.o: $(EBTS_DIR)/src/shell/shell.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_command.o: $(EBTS_DIR)/src/shell/command.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_object.o: $(EBTS_DIR)/src/shell/object.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_user.o: $(EBTS_DIR)/src/shell/user.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_commandbox.o: $(EBTS_DIR)/src/shell/commandbox.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_memory.o: $(EBTS_DIR)/src/utils/memory.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_string.o: $(EBTS_DIR)/src/utils/string.c
	@echo "[CC] $<"
	@$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/drivers_main.o: $(DRIVERS_DIR)/src/main.c
	@echo "[CC] $<"
	@$(CC) $(DRIVERS_CFLAGS) -c $< -o $@

# ---- Tests ----
tests: $(TESTDIR)/simple_test $(TESTDIR)/test_animation $(TESTDIR)/whistler_test

$(TESTDIR)/simple_test: $(TESTDIR)/simple_test.c
	@gcc $< -o $@

$(TESTDIR)/test_animation: $(TESTDIR)/test_animation.c
	@gcc $< -o $@

$(TESTDIR)/whistler_test: $(TESTDIR)/whistler_test.c
	@gcc $< -o $@

# ---- Clean ----
clean:
	@echo "[CLEAN] Removing build files"
	@rm -rf $(OBJDIR) $(TARGET) $(TARGET_ELF) $(EBTS_BIN) $(EBTS_ELF) \
		$(DRIVERS_BIN) $(DRIVERS_ELF) \
		$(TESTDIR)/simple_test $(TESTDIR)/test_animation $(TESTDIR)/whistler_test
