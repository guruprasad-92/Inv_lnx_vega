PTH := $(shell find -type f -name cmd.mk | head -1)
OBJS += $(PTH:.mk=.o)
# OBJS += ${PTH}
# $(info cmd.mk : pth : ${PTH})
