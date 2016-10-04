#c compiler:
_CC = gcc
_CC_ARM = arm-none-eabi-gcc
#directory containing all .h and .c files:
SDIR = src
#directory containing all .o files:
_ODIR = bin
_ODIR_ARM = arm_bin
#c compiler flags:
_CFLAGS = -Wall -std=c11 -O3
_CFLAGS_ARM = -Wall -mcpu=cortex-m0 -mthumb -g 

#the name of all object files included in the main compalation:
_OBJ_PI = sdpUtil.o binSet.o sdpio.o sdpSort.o 
_OBJ_ARM = init.o




#detects the opperating system
ifeq ($(OS),Windows_NT)
	RMprefix=del /S /Q /F
	RMappend=
else
	RMprefix=find . -name  
	RMappend=-type f
	#TODO: test and add "-delete" flag at end
endif


#default target device
target = pi
ifeq ("$(target)","pi")
CC = $(_CC)
ODIR = $(_ODIR)
CFLAGS = $(_CFLAGS)
_OBJ = $(_OBJ_PI)
#no hex conversion done
HEX_CONVERSION = 
HEX_CLEAN = 
#setting the target device to anything else defaults to using the arm as a target
else
CC = $(_CC_ARM)
ODIR = $(_ODIR_ARM)
CFLAGS = $(_CFLAGS_ARM)
_OBJ = $(_OBJ_ARM)
#hex file is the desired output so conversionis done
HEX_CONVERSION = objcopy -O ihex  $(_ODIR_ARM)/$@.elf $@.hex
#temp --file-alignment 4096
HEX_CLEAN = $(RMprefix) $@.elf $(RMappend)
endif


#adds the object dirrectory to all object files:
OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))
#flag that tells the compiler where to look for .h files:
IDIR = -I$(SDIR)

LINKER_PATH = "C:\Program Files (x86)\GNU Tools ARM Embedded\5.3 2016q1\lib\gcc\arm-none-eabi\5.3.1\armv6-m"
#when target=arm a linker is used
ifeq ("$(target)","pi")
LINK = $(OBJ)
LINKER_CMD =
LINK_PRE =
TEMP_FIX =
else
LINK =
LINKER_CMD = arm-none-eabi-ld $(ODIR)/init.o $@.elf -L $(LINKER_PATH) -lgcc -T $(SDIR)/linker_script.ld --cref -Map $(ODIR)/$@.map -nostartfiles -o $(ODIR)/$@.elf
LINK_PRE = -c
TEMP_FIX = .elf
endif

ifneq ("$(wildcard $(ODIR))","")
ODIR_CREATE = 
else
ODIR_CREATE = mkdir $(ODIR)
endif




#edit to change the default target; typing "make" will run this:
default: test

#causes "make lib" to work even if there is a file called "lib"
.PHONY: lib
#target that lists all of the object files library files, "make lib" runs this:
lib: $(OBJ)

.PHONY: $(ODIR)
$(ODIR): 
	$(ODIR_CREATE)


#target that comiles a signle object files, called by lib
$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) -c $< -o $@ $(CFLAGS)

#causes "make clean" to work even if there is a file called "clean"
.PHONY: clean
#deletes all .o and .exe files.  Also deletes any .txt files in bin
#typing "make clean" runs this:
clean:
	$(RMprefix) *.o $(RMappend)
	$(RMprefix) *.exe $(RMappend)
	$(RMprefix) *.hex $(RMappend)
	$(RMprefix) *.map $(RMappend)
	$(RMprefix) *.elf $(RMappend)

#if any string is feed to makefile, or is the prerequisite of default and does not
#follow the above target syntax it will run the following
#i.e. "make main" will compile main.c if it is the same directory as this Makefile
#also if the default's prerequisite is "main" then it will also compile
#this works for any string without a file extension, i.e. "test", "m0code", "a", ...
%: lib %.c
	$(CC) $(LINK_PRE) $@.c -o $@$(TEMP_FIX) $(LINK) $(CFLAGS) $(IDIR)
	$(LINKER_CMD)
	$(HEX_CONVERSION)
	$(HEX_CLEAN)