PTH := $(shell find -type f -name cmd_sms.mk)
OBJS += $(PTH:.mk=.o)
# PTH := $(subst ./,./bin/,${PTH})
# OBJS += ${PTH}
# OBJS += $(PTH: "./" = "./bin/")
# $(info cmd_sms.mk : pth : ${PTH})
