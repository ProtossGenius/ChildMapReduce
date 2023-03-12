##Tail
prebuild:
	smdcatalog
debug:

qrun:
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j$(nproc) && ./unit_test  && ./master

test:
	cd build && cmake .. && make -j$(nproc)	&& ctest

install:
clean:

