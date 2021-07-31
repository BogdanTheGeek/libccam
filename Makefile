IDIR=.
CC=cc
CFLAGS=-I$(IDIR) -lm

all: main.c libccam.o libccam-svg.o
	@$(CC) -o main $^ $(CFLAGS)
	@echo "Built $@"

libccam-svg.o: libccam-svg.c libccam.h libccam-svg.h
	@$(CC) -c -o $@ $< $(CFLAGS)
	@echo "Built $@"

libccam.o: libccam.c libccam.h libccam-svg.h
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
	@rm *.o main out.nc

