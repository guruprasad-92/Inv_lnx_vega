PTH := $(shell find -type f -name socket.mk | head -1)
OBJS += $(PTH:.mk=.o)