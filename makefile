all:
	mkdir -p build
	cd build && cmake .. && make

install:
	cd build && make install

uninstall:
	cd build && make uninstall
