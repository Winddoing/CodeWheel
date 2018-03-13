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

#DEBUG	:= -g
CFLAGS	:= -O2 -Wall  -std=c++11 -fpermissive
CFLAGS	+= -mips32r2 -msynci -march=xburst2 $(DEBUG)

LFLAGS   := -lstdc++ -lpthread -march=xburst2 $(DEBUG)
COBJS	:= $(SRCTREE)/Thread.o $(SRCTREE)/Exception.o $(SRCTREE)/rixi.o

SRCS	:= $(addprefix $(src),$(COBJS:.o=.cpp))
OBJS	:= $(SOBJS) $(COBJS)

APP	:= rixitest

all:	$(APP)

$(APP):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)


$(SRCTREE)/%.o: $(SRCTREE)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

$(SRCTREE)/%.o: $(SRCTREE)/%.S
	$(CC)  $(AFLAGS)  -D_ASSEMBLER_ -D__ASSEMBLY__  -o $@ -c $<

clean:
	rm -fr *.o $(APP) $(OBJS)
