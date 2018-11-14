CC ?= gcc
CFLAGS += -std=c99
LDFLAGS ?= -lhidapi-libusb

####################################################################################################



.PHONY: all
all: CorsairCommanderProControl

.PHONY: install
install: all
	cp CorsairCommanderProControl /bin/set-corsair-commander-colors
	cp set-corsair-commander-colors.service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable set-corsair-commander-colors

.PHONY: clean
clean:
	rm -f CorsairCommanderProControl


CorsairCommanderProControl: corsair.c
	$(CC) corsair.c -lhidapi-libusb -o CorsairCommanderProControl
	

####################################################################################################
