#c compiler:
CC = gcc
#directory containing all .h and .c files:
SDIR = src
#directory containing all .o files:
ODIR = bin
#c compiler flags:
CFLAGS = -Wall -std=c11 -O3
#the name of all object files included in the main compalation:
_OBJ = sdpUtil.o binSet.o sdpio.o
#adds the object dirrectory to all object files:
OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))
#flag that tells the compiler where to look for .h files:
IDIR = -I$(SDIR)

ifneq ("$(wildcard $(ODIR))","")
ODIR_CREATE = 
else
ODIR_CREATE = mkdir $(ODIR)
endif

#detects the opperating system
ifeq ($(OS),Windows_NT)
	RMprefix=del /S /Q /F
	RMappend=
else
	RMprefix=find . -name  
	RMappend=-type f
	#TODO: test and add "-delete" flag at end
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
$(ODIR)/%.o: $(SDIR)/%.c $(SDIR)/%.h | $(ODIR)
	$(CC) -c $< -o $@ $(CFLAGS)

#causes "make clean" to work even if there is a file called "clean"
.PHONY: clean
#deletes all .o and .exe files.  Also deletes any .txt files in bin
#typing "make clean" runs this:
clean:
	$(RMprefix) *.o $(RMappend)
	$(RMprefix) *.exe $(RMappend)
	$(RMprefix) $(ODIR)\*.txt $(RMappend)

#if any string is feed to makefile, or is the prerequisite of default and does not
#follow the above target syntax it will run the following
#i.e. "make main" will compile main.c if it is the same directory as this Makefile
#also if the default's prerequisite is "main" then it will also compile
#this works for any string without a file extension, i.e. "test", "m0code", "a", ...
%: lib %.c
	$(CC) $@.c -o $@ $(OBJ) $(CFLAGS) $(IDIR)