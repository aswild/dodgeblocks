###########################
##  CS1372 GBA Makefile  ##
##     By Allen Wild     ##
###########################

PRODUCT_NAME		= DodgeBlocks

#List Sources Here
SOURCES				= dodgeblocks.c blocks.c rainbow.c gbalib.c text.c splash.c
HEADERS				= gba.h

OBJECTS				= $(SOURCES:%.c=build/%.o)

###### --- System Settings (Update these for your system)
DKPATH             = /home/allen/software/devkitARM/bin
CCPATH             = /usr/bin
VBASIM             = /usr/bin/VisualBoyAdvance

FIND               = find
COPY               = cp -r
RM                 = rm -f

# --- File Names
ELF_NAME           = build/$(PRODUCT_NAME).elf
ROM_NAME           = build/$(PRODUCT_NAME).gba

# --- GBA Settings
MODEL              = -mthumb-interwork -mthumb
SPECS              = -specs=gba.specs

# --- Archiver
AS                 = $(DKPATH)/arm-eabi-as
ASFLAGS            = -mthumb-interwork

# --- Compiler
CC                 = $(DKPATH)/arm-eabi-gcc
CFLAGS             = $(MODEL) -O2 -Wall -pedantic -Wextra -Werror -Wno-unused-variable -Wno-unused-but-set-variable -ansi -std=c99 -D_ROM=$(ROM_NAME) -D_VBA=$(VBASIM)
#CFLAGS             = $(MODEL) -O2 -Wall -pedantic -Wextra -Werror -ansi -std=c99 -D_ROM=$(ROM_NAME) -D_VBA=$(VBASIM)
CC_WRAP            = $(CCPATH)/gcc
CFLAGS_WRAP        = -O2 -Wall -pedantic -Wextra -Werror -ansi -std=c99 -D_ROM='"$(ROM_NAME)"' -D_VBA='"$(VBASIM)"'

# --- Linker
LD                 = $(DKPATH)/arm-eabi-gcc
LDFLAGS            = $(SPECS) $(MODEL) -lm

# --- Object/Executable Packager
OBJCOPY            = $(DKPATH)/arm-eabi-objcopy
OBJCOPYFLAGS       = -O binary

# --- ROM Fixer
GBAFIX             = $(DKPATH)/gbafix


# ---- Main Build Targets
all : $(ROM_NAME)

# --- Build .elf file into .gba ROM file
$(ROM_NAME) : $(ELF_NAME)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(ELF_NAME) $(ROM_NAME)
	$(GBAFIX) $(ROM_NAME)

# --- Build .o files into .elf file
$(ELF_NAME) : $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

# -- Build .c files into .o files
build/%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	#$(RM) $(ROM_NAME)
	#$(RM) $(ELF_NAME)
	#$(RM) $(BIN_NAME)
	#$(RM) *.o
	#$(RM) build/*.o
	#$(RM) build/test
	#$(RM) build/homework.*

	#$(RM) `ls build`
	$(RM) -r build
	mkdir -m 700 build
	-rm `ls -a | grep "~$""`

run : $(ROM_NAME)
	$(VBASIM) -2 $(ROM_NAME)

test : test.c
	gcc test.c -o build/test
	build/test
