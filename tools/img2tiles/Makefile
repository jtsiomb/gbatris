PREFIX = /usr/local

src = $(wildcard src/*.c)
obj = $(src:.c=.o)
bin = img2tiles

CFLAGS = -pedantic -Wall -g
LDFLAGS = -lpng -lz

$(bin): $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)

.PHONY: install
install: $(bin)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(bin) $(DESTDIR)$(PREFIX)/bin/$(bin)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(bin)
