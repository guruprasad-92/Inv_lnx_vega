NM := vega_sock
PTH := $(shell find -type d -name ${NM} | head -1)
OBJ_VG += ${PTH}/${NM}.o
INCLUDE_VG += -I ${PTH}
