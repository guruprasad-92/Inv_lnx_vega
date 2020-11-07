PTH := $(shell find -type f -name cmd_sim.mk)
OBJS += $(PTH:.mk=.o)