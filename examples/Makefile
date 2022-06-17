
LIBCCAM_PATH=libccam
IDIR=$(LIBCCAM_PATH)/

CC=cc
CFLAGS=-I$(IDIR) -lm

SRC=$(filter-out main.c, $(wildcard *.c))
OBJ:=$(patsubst %.c,%.o,$(SRC))

LIBCCAM_SRC=$(filter-out $(LIBCCAM_PATH)/main.c, $(wildcard $(LIBCCAM_PATH)/*.c))
LIBCCAM_H=$(wildcard $(LIBCCAM_PATH)/*.h)
LIBCCAM_OBJ:=$(patsubst %.c,%.o,$(LIBCCAM_SRC))

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

main: main.c $(OBJ) libccam
	@echo "$(WARNING)Building $@ $(RESET)"
	@$(CC) -o $@ $< $(CFLAGS) $(OBJ) $(LIBCCAM_OBJ)

$(OBJ): %.o: %.c libccam
	@echo "$(WARNING)Building $@ $(RESET)"
	@$(CC) -c -o $@ $< $(CFLAGS) $(LIBCCAM_OBJ)

.PHONY: libccam
libccam: $(LIBCCAM_SRC) $(LIBCCAM_H) 
	@make -C $(LIBCCAM_PATH) libccam

.PHONY: generate
generate: main
	@./main | tee out.nc

.PHONY: show
show: generate
	@cginc out.nc > /dev/null

.PHONY: clean
clean:
	@rm -f *.o main out.nc
	@make -C $(LIBCCAM_PATH) clean
	@echo "$(JAZZ)Build files cleaned!$(RESET)"

