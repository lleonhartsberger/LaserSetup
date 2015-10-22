all: tools src 

tools::
	$(MAKE) -C $@
src::
	$(MAKE) -C $@

clean:
	(cd tools; make clean)
	(cd src; make clean)
	(rm -f lib/* bin/*)

