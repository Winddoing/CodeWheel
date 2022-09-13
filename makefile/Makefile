SRC_PATH := $(dir $(lastword $(MAKEFILE_LIST)))
PWD := $(shell pwd)

# version decode: client.major.minor.release
#	client: the proposed client ID
#       major: increase the major number every year
#       		2020=>K.2.2.0 2021=>K.3.2.0 2022=>K.4.2.0
#       minor: increase the minor number every promotion
#       		K.2.1.0=>K.2.2.0=>K.2.3.0=>...=>K.2.255.0
#       release: change the release number with each release
VERSION_FILE = $(SRC_PATH)/VERSION
GCC_VER_GE9 = $(shell echo `gcc -dumpversion | cut -f1-2 -d.`\>=9 | bc)
GCC_VER_GE6 = $(shell echo `gcc -dumpversion | cut -f1-2 -d.`\>=6 | bc)

getver = $(shell grep $1 $(VERSION_FILE) | awk -F'=' '{print $$2}')
ifneq ("", "$(wildcard $(VERSION_FILE))")
  TST_VERSION = $(call getver,client).$(call getver,major).$(call getver,minor).$(call getver,release)
else
  TST_VERSION = staging
endif

$(info $(GCC_VER_GE9))
$(info $(GCC_VER_GE6))
$(info $(TST_VERSION))

ifeq ($(GCC_VER_GE9),1)
	subdir-ccflags-y += -fcf-protection=none
endif

ifeq ($(GCC_VER_GE6),1)
	subdir-ccflags-y += -Wshift-negative-value
endif


all:
	echo "Test"

