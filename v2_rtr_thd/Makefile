#
#	Author : Guruprasad
#
SHELL := /bin/bash
ARCH :=
T_BS := prs_mdmV2
ifeq (${ARCH},arm)
$(info ARCH=${ARCH})
# CC := arm-linux-uclibcgnueabi-gcc
CC := arm-linux-gnueabi-gcc
TARGET := ${T_BS}.arm.bin
else
$(info ARCH=${ARCH})
CC := gcc
TARGET := ${T_BS}.bin
endif

$(info CC = ${CC})
TOP_DIR := ./
LIB := -lpthread

INCLUDE_VG := #-I ./${DIR_INCLD}  -I ./${DIR_INCLD}vega_sock/
OBJ_VG := main_exp.o


MK := $(shell find -type f -name *.mk)
include ${MK}

# $(info OBJ_VG = ${OBJ_VG})
# $(info ---------------)
# $(info INCLUDE_VG = ${INCLUDE_VG})

${TARGET}:${OBJ_VG}
	@echo "	CC	"$<
	@${CC} ${INCLUDE_VG} -o $@ ${OBJ_VG} ${LIB}
	@echo Finished building target : $@
	@size ${TARGET}

clean:
	@echo "  Cleaning all object files ..."
	@echo "  Cleaning the binary file... "
	@echo "  Cleaning the elf files..."
	@find . -name "*.o" -delete
	@find . -name "*.bin" -delete
	@find . -name "*.elf" -delete

.c.o:
	@echo "	CC	"$<
	@${CC} ${INCLUDE_VG} ${LIB} -c -o $@ $<