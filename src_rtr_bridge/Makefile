#
#	Author : Guruprasad
#
SHELL := /bin/bash
TOP_DIR := ./
DEF	:= -DTST_MCRO
BIN_DIR := bins/
ARCH :=
LIB_PTH :=
LIB_PTH2 :=
LIB :=

define PYSCR_TEST
def get_ver() :
	print("Running python3");
	fd = open("version.cfg","r");
	i=0;
	str2 = "";
	while True:
		str = fd.readline();
		i += 1;
		str = str.replace("\n","",1);
		str = str.replace("\r","",1);
		str2 += str[str.find("=")+1:];
		if(i==1) :
			str2 += "."
		print(str);
		if (str is None) :
			break;
		elif (i == 2):
			break;
	print (str2);
	fd.close();
	return str2;
get_ver()
endef

export PYSCR_TEST

# RET = $(shell python3 version.py)
# RET = $(shell source version.cfg; echo "${Product}.${Bin}.${Customer}")

include version.cfg
RET = ${Product}.${Bin}.${Customer}

DEF +=  -D APP_VER=\"${RET}\"

$(info DEF = ${DEF})

# T_BS := chk_klg
# T_BS := sim_sel
T_BS := rtr_bridge
# T_BS := rbt

ifeq (${ARCH},arm)
	CC := arm-linux-gnueabi-v7.4.1-gcc
	LIB_PTH := /opt/arm_xcc/gcc-v7.4.1__arm-linux-gnueabi/arm-linux-gnueabi/lib/
	LIB_PTH2 := /opt/arm_xcc/gcc-v7.4.1__arm-linux-gnueabi/lib/
	LIB += -L${LIB_PTH} -L${LIB_PTH2}
	LIB += -Wl,-rpath,${LIB_PTH},-rpath,${LIB_PTH2}
	TARGET := ${T_BS}.arm.bin
else
	CC := gcc
	TARGET := ${T_BS}.bin
endif

$(info ARCH	= ${ARCH})
$(info CC	= ${CC})

CRT := $(shell build_cfg/create_mk.py build_cfg/incld_dir.cfg )

# $(info HERE-----------)
LIB += -lpthread -lmosquitto
# LIB := -lpthread
# LDFLGS := -D

INCLUDES:= -I ./include/ -I ./include/cmd/ -I ./include/operation 
# OBJS := main_exp.o
# OBJS := main_1.o
# OBJS := main_rbt.o
OBJS := prj_init/main_1.o
# OBJS := prj_init/main_del.o
MK := $(shell find -type f -name *.mk)
include ${MK}

	
.PHONY : PY_SCRS ${TARGET} clean-object PY_SCRS 

# 	WITH_PY : | PY_SCRS ${TARGET}

${BIN_DIR}${TARGET} : ${OBJS}
	@echo "	CC	"$<
	@${CC} ${INCLUDES} -o $@ ${OBJS} ${LIB} ${DEF}
	@echo Finished building target : $@
	@size ${BIN_DIR}${TARGET}

clean:
	@echo "  Cleaning all object files ..."
	@echo "  Cleaning the binary file... "
	@echo "  Cleaning the elf files..."
	@find . -name "*.o" -delete
	@find . -name "*.bin" -delete
	@find . -name "*.elf" -delete
	@find . -name "*.mk" -delete
	@python3 -c "$$PYSCR_TEST"
	@python3 version.py
	@echo "return = ${RET}"

clean-object:
	@find . -name "*.o" -delete

.c.o:
	@echo "	CC	"$<
	@${CC} ${INCLUDES} ${LIB} ${DEF} -c -o $@ $<

