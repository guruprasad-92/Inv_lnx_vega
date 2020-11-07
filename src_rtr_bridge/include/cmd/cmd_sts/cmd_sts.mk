PTH := $(shell find -type f -name cmd_sts.mk | head -1)
OBJS += $(PTH:.mk=.o)