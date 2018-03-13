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

BUILD_DIR := .

CURDIR	:= $(BUILD_DIR)

SRCTREE	:= $(CURDIR)

CROSS_COMPILE	:= mips-linux-gnu-


CC      := $(CROSS_COMPILE)gcc
CPP     := $(CROSS_COMPILE)g++
OBJCOPY	:= $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
NM	:= $(CROSS_COMPILE)nm

#AFLAGS	:= -mips32r2 -O2 -EL -Wall
#CFLAGS	:= -mips32r2 -O2 -EL -Wall
CFLAGS	:= -O2 -Wall  -std=c++11 -fpermissive  -O2  -Wall
CFLAGS  += -mips32r2 -EL

LFLAGS   := -static -lstdc++ -lpthread
COBJS	:= $(SRCTREE)/Thread.o $(SRCTREE)/MemBreak.o

SRCS	:= $(addprefix $(src),$(COBJS:.o=.cpp))
OBJS	:= $(SOBJS) $(COBJS)

APP	:= membreak

all:	$(APP)

$(APP):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)


$(SRCTREE)/%.o: $(SRCTREE)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

$(SRCTREE)/%.o: $(SRCTREE)/%.S
	$(CC)  $(AFLAGS)  -D_ASSEMBLER_ -D__ASSEMBLY__  -o $@ -c $<

clean:
	rm -fr *.o $(APP) $(OBJS)
