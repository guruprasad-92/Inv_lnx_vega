PTH := $(shell find -type f -name cmd_op.mk)
OBJS += $(PTH:.mk=.o)