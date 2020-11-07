src = $(wildcard *.c)
obj = $(src:.c=.o)

SerialComm: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) SerialComm
