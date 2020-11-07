PTH := $(shell find -type f -name cmd_rst.mk | head -1)
OBJS += $(PTH:.mk=.o)