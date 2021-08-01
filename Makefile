IDIR=.
CC=cc
CFLAGS=-I$(IDIR) -lm
ALLHEADERS:=$(wildcard *.h)
SVGHEADERS=libccam-svg.h types.h

all: main.c libccam.o libccam-svg.o
	@$(CC) -o main $^ $(CFLAGS)
	@echo "Built $@"

libccam-svg.o: libccam-svg.c $(SVGHEADERS)
	@$(CC) -c -o $@ $< $(CFLAGS)
	@echo "Built $@"

libccam.o: libccam.c libccam.h $(ALLHEADERS)
	@$(CC) -c -o $@ $< $(CFLAGS)
	@echo "Built $@"

.PHONY: generate
generate: all
	@./main | tee out.nc

.PHONY: show
show: generate
	@cginc out.nc


.PHONY: clean
clean:
	@rm -f *.o main out.nc

