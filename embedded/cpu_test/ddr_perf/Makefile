# ----------------------------------------------------------------------------
#
# Copyright (C) 2008 Ingenic Semiconductor Co., Ltd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# Just a invoke entry, and will include each .mak file if needed
#
# Version,	Date, 		Engineer,	Mark
# v1.0, 	2014-04-13,	bliu,		Init version
#
# ----------------------------------------------------------------------------

SIMD_TEST := yes
BUILD_DIR := .

CURDIR	:= $(BUILD_DIR)

SRCTREE	:= $(CURDIR)

CROSS_COMPILE	:= mips-linux-gnu-


CC      := $(CROSS_COMPILE)gcc
CPP     := $(CROSS_COMPILE)g++
OBJCOPY	:= $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
NM	:= $(CROSS_COMPILE)nm

AFLAGS	:= -mips32r2 -O2 -EL -Wall
CFLAGS	:= -mips32r2 -O2 -EL -Wall
CFLAGS  += -fpermissive -std=c++11
LFLAGS  := -lstdc++ -static
SOBJS	:= $(SRCTREE)/memcpy.o $(SRCTREE)/memset.o

ifeq ($(strip $(SIMD_TEST)),yes)
SOBJS += $(SRCTREE)/simd_memcpy.o $(SRCTREE)/simd_memset.o
AFLAGS	+=  -march=xburst2 -mmsa -DSIMD_TEST
CFLAGS += -march=xburst2 -mmsa -DSIMD_TEST
LFLAGS += -march=xburst2
endif

COBJS	:= $(SRCTREE)/ddr.o

SRCS	:= $(addprefix $(src),$(SOBJS:.o=.S) $(COBJS:.o=.cpp))
OBJS	:= $(SOBJS) $(COBJS)

APP	:= ddr_test

all:	$(APP)

$(APP):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)


$(SRCTREE)/%.o: $(SRCTREE)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

$(SRCTREE)/%.o: $(SRCTREE)/%.S
	$(CC)  $(AFLAGS)  -D_ASSEMBLER_ -D__ASSEMBLY__  -o $@ -c $<

clean:
	rm -fr *.o $(APP) $(OBJS)
