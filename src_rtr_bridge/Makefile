#
#	Author : Guruprasad
#
SHELL := /bin/bash
ARCH :=
T_BS := rtr_bridge
# T_BS := sim_sel
ifeq (${ARCH},arm)
$(info ARCH=${ARCH})
CC := arm-linux-gnueabi-gcc
TARGET := ${T_BS}.arm.bin
else
$(info ARCH=${ARCH})
CC := gcc
TARGET := ${T_BS}.bin
endif

# CC := arm-linux-gnueabi-gcc
# CC := gcc
$(info CC = ${CC})
TOP_DIR := ./
LIB := -lpthread

INCLUDES:= -I ./include/ -I ./include/cmd/ -I ./include/operation 
# LIB := -lpthread
# OBJS := main_exp.o
OBJS := main_1.o
MK := $(shell find -type f -name *.mk)
include ${MK}


# $(info MK : )
# $(info ${MK})
# $(info -----)
# $(info BOJS : ${OBJS})

${TARGET}:${OBJS}
	@echo "	CC	"$<
	@${CC} ${INCLUDES} -o $@ ${OBJS} ${LIB}
	@echo Finished building target : $@
	@size ${TARGET}

clean:
	@echo "  Cleaning all object files ..."
	@echo "  Cleaning the binary file... "
	@echo "  Cleaning the elf files..."
	@find . -name "*.o" -delete
	@find . -name "*.bin" -delete
	@find . -name "*.elf" -delete

clean-object:
	@find . -name "*.o" -delete

.c.o:
	@echo "	CC	"$<
	@${CC} ${INCLUDES} ${LIB} -c -o $@ $<

