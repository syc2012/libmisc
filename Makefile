include build.include

all clean:
	@make -C source $@
	@make -C test $@
