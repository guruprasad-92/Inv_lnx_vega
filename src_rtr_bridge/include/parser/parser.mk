PTH := $(shell find -type f -name parser.mk | head -1)
OBJS += $(PTH:.mk=.o)
# OBJS += $(PTH:./=./bin)
# OBJS += ${PTH}
# $(info parser.mk : pth : ${PTH})
