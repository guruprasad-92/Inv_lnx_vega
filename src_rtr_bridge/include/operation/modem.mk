PTH := $(shell find -type f -name modem.mk | head -1)
OBJS += $(PTH:.mk=.o)
OBJS += include/operation/mdm_payload.o
OBJS += include/operation/mdm_operation.o
# OBJS += ${PTH}
# OBJS += $(PTH:./=./bin)
# $(info modem.mk : pth : ${PTH})
