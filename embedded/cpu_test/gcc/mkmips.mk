############### SPEC  for  mips  simulator make ##############

################### defined in respective Makefile ###########
#NUMBER    = 172
#NAME      = vpr
#EXEBASE   = vpr
#BENCHLANG = C
#SOURCES   = *.c ...
#NEED_MATH=yes
######### possible defined #############
# RM_SOURCE 
# 
# EXTRA_...
##############################################################
#Compilation toolchain
PREFIX		= mips-linux-gnu-
#PREFIX		= mipsel-linux-
GCC      	= $(PREFIX)gcc
AS		= $(PREFIX)as
LD		= $(PREFIX)ld
OBJCOPY 	= $(PREFIX)objcopy
OBJDUMP 	= $(PREFIX)objdump
GCC_VER		= 4.5.2

#Simulator correlative -EL
GCC_OPT	= -mips32 -O2 -static 
OBJDUMP_OPT	= -D

TUNE	=  #_mips
EXT	=  #.exe

BENCHMARK     = $(NUMBER).$(NAME)
FINAL_SOURCES = $(filter-out $(RM_SOURCES),$(SOURCES)) $(EXTRA_SOURCES)
FINAL_SOURCES += $(wildcard sim_bug.c)
OBJS          = $(addsuffix $(OBJ),$(basename $(FINAL_SOURCES)))
OBJ = .o

ifdef NEED_MATH
    MATH_LIBS       = -lm
else
    MATH_LIBS       =
endif

LIBS = $(MATH_LIBS)
DEF_MACRO = -DSPEC_CPU2000 -DMIPS_SIM
MACRO_FILE = $(wildcard sim_conf.h)
ifneq ($(MACRO_FILE),)
    MACRO_FLAG = -imacros $(MACRO_FILE)
else
    MACRO_FLAG =
endif
CFLAGS= $(DEF_MACRO) $(MACRO_FLAG)

all:build

build: $(EXTRA_TARGETS) $(EXEBASE)

$(EXEBASE):$(OBJS)
	$(GCC) $(GCC_OPT) -o $@ $(OBJS) $(LIBS)

%$(OBJ): %.c $(INCLUDE)
	$(GCC) $(GCC_OPT) $(CFLAGS) -c $<

.PHONY: clean install
clean:
	rm -rf $(EXEBASE) *$(OBJ) $(EXTRA_RMFILES) core *.err *.out 

install: $(EXEBASE)
	cp $(EXEBASE) ../bin/$(EXEBASE)$(TUNE)$(EXT)
