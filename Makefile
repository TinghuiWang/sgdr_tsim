# Compiling Environment Settings
CC=gcc
CFLAGS= -Iinclude -g

sub-dir := src 
obj-y :=

TARGET=sgdr_tsim

include $(addsuffix /Makefile, $(sub-dir))

all: $(TARGET)

help:
	@echo ""
	@echo "**************** SGDR Tomasolu Simulator *******************"
	@echo ""
	@echo "Author: Tinghui WANG <twang3@eecs.wsu.edu>"
	@echo "        Gina Sprint"
	@echo "        Daniel  "
	@echo "        Ryan Kim "
	@echo "-------------------------------------------------------------"
	@echo ""
	@echo "MAKE USAGE"
	@echo "tags:     Create CTAGS for source files"
	@echo "all:      Create simulator binary: sgdr_tsim"
	@echo ""

${TARGET}: ${obj-y}
	${CC} -g $^ -o $@
	ls -l $@

tags:
	ctags `find . -name "*.[c|s|h]"`

clean:
	rm -f ${obj-y} 
	rm -f ${TARGET}

distclean: clean
	rm -f tags 

.PHONY : all
.PHONY : clean
.PHONY : distclean
