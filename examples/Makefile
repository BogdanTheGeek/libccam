IDIR=libccam/
CC=cc
CFLAGS=-I$(IDIR) -lm
LIBCCAM = libccam/libccam.c libccam/libccam-svg.c libccam/types.h libccam/libccam.h libccam/libccam-svg.h

all: *.c $(LIBCCAM)
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo "Done!"

.PHONY: show
show: all
	./all > out.nc
	cginc out.nc *.stl --msaa

.PHONY: clean
clean:
	@rm -f *.nc all

