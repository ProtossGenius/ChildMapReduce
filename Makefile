##Tail
prebuild:
	smdcatalog
debug:

qrun:
	cd build && cmake .. && make && ctest &&	./master
test:

install:
clean:

