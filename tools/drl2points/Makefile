IDIR =../../
CC=cc
CFLAGS=-I$(IDIR)

drl2points: drl2points.c 
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo "Done!"

.PHONY: test
test:
	@cp example.drl test.drl
	@./drl2points test.drl
	@diff test.h example.h > /dev/null && echo "Test Succesful" || echo "Test FAILED"
	@rm test.drl test.h

.PHONY: clean
clean:
	@rm drl2points

