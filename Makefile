INCLUDE_DIR := ./
SRC_C := ${shell find . -name '*.c'}
exe:${SRC_C}
	@rm -rf a.out
	gcc  -gstabs $^ -o $@ -I${INCLUDE_DIR} -lm

.PHNOY: clean
clean:
	rm -rf  exe
.PHNOY: debug
debug:
	@echo ${SRC_C}
	
