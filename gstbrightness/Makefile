PLUGIN_NAME = brightness
PREFIX ?= /usr/
CC = gcc
CFLAGS = -fPIC `pkg-config --cflags gstreamer-1.0 gstreamer-video-1.0`
LDFLAGS = -shared
LIBS = `pkg-config --libs gstreamer-1.0 gstreamer-video-1.0`

SRC = src/gstbrightness.c
OBJ = $(SRC:.c=.o)

all: libgst$(PLUGIN_NAME).so

libgst$(PLUGIN_NAME).so: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) libgst$(PLUGIN_NAME).so

install: libgst$(PLUGIN_NAME).so
	mkdir -p $(PREFIX)/lib/gstreamer-1.0
	cp libgst$(PLUGIN_NAME).so $(PREFIX)/lib/aarch64-linux-gnu/gstreamer-1.0/

uninstall:
	rm -f $(PREFIX)/lib/aarch64-linux-gnu/gstreamer-1.0/libgst$(PLUGIN_NAME).so

#/usr/lib/aarch64-linux-gnu/gstreamer-1.0/