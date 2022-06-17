IDIR=.
CC=cc
CFLAGS=-I$(IDIR) -lm
ALLHEADERS:=$(wildcard *.h)
SVGHEADERS=libccam-svg.h types.h

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	COLOUR=
else
	COLOUR=--color
endif

# define standard colors
ifneq (,$(findstring 256color,${TERM}))
	FAIL    := $(shell tput -Txterm setaf 1)
	SUCCESS := $(shell tput -Txterm setaf 2)
	WARNING := $(shell tput -Txterm setaf 3)
	JAZZ    := $(shell tput -Txterm setaf 4)
	RESET   := $(shell tput -Txterm sgr0)
else
	FAIL    := ""
	SUCCESS := ""
	WARNING := ""
	JAZZ    := ""
endif

main: main.c libccam.o libccam-svg.o
	@echo "$(WARNING)Building $@ $(RESET)"
	@$(CC) -o main $^ $(CFLAGS)

.PHONY: libccam
libccam: libccam.o libccam-svg.o
	@echo "$(WARNING)Building $@ $(RESET)"

libccam-svg.o: libccam-svg.c $(SVGHEADERS)
	@echo "$(WARNING)Building $@ $(RESET)"
	@$(CC) -c -o $@ $< $(CFLAGS)

libccam.o: libccam.c libccam.h $(ALLHEADERS)
	@echo "$(WARNING)Building $@ $(RESET)"
	@$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: generate
generate: main
	@./main | tee out.nc

.PHONY: show
show: generate
	@cginc out.nc > /dev/null

#testing targers
TESTS:=$(patsubst %.c,%.bin,$(wildcard examples/*.c))
$(TESTS): %.bin: %.c libccam.o libccam-svg.o
	@echo "$(WARNING)Building $@ $(RESET)"
	@$(CC) -o $@ $^ $(CFLAGS)

RUN_TESTS:=$(patsubst %.c,%,$(wildcard examples/*.c))
.PHONY: $(RUN_TESTS)
$(RUN_TESTS): $(TESTS)
	@./$@.bin > $@.temp.nc
	@diff $(COLOUR) $@.temp.nc $@.nc && echo "$(SUCCESS)Test $@ Succesful$(RESET)" || (echo "$(FAIL)Test $@ FAILED$(RESET)" && echo "")

.PHONY: test
test: $(RUN_TESTS)
	@echo "$(JAZZ)Done!$(RESET)"


.PHONY: clean
clean:
	@rm -f *.o main out.nc
	@rm -f examples/*.o examples/*.bin examples/*.temp.nc
	@echo "$(JAZZ)Build files cleaned!$(RESET)"

