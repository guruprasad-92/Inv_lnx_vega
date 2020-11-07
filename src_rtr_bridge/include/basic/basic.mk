PTH := $(shell find -type f -name basic.mk | head -1)
OBJS += $(PTH:.mk=.o)
# OBJS += ${PTH}
# OBJS += $(PTH:./=./bin)
# $(info basic.mk : pth : ${PTH})
