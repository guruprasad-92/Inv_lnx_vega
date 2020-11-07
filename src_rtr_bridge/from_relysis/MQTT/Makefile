src = $(wildcard *.c)
obj = $(src:.c=.o)
DEBUG = -DDEBUG

LDFLAGS = -lmosquitto

mqttpubsub: $(obj)
	$(CC) $(DEBUG) -o $@ $^ $(LDFLAGS)

debug: DEBUG = -DDEBUG

debug: diread
.PHONY: clean
clean:
	rm -f $(obj) mqttpubsub
