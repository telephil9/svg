</$objtype/mkfile

TARG=svg
BIN=/$objtype/bin
LIB=posix/libposix.$O.a
OFILES=svg.$O

</sys/src/cmd/mkone

CFLAGS=-FTVw -p -Iposix

$LIB:V:
	cd posix
	mk

clean nuke:V:
	@{ cd posix; mk $target }
	rm -f *.[$OS] [$OS].out $TARG
