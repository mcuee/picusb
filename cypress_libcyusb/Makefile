all:
	g++ -fPIC -o lib/libcyusb.o -c lib/libcyusb.cpp
	g++ -shared -Wl,-soname,libcyusb.dll -o lib/libcyusb.dll lib/libcyusb.o -l usb-1.0
	rm -f lib/libcyusb.o
clean:
	rm -f lib/libcyusb.so lib/libcyusb.so.1
help:
	@echo	'make		would compile and create the library and create a link'
	@echo	'make clean	would remove the library and the soft link to the library (soname)'
