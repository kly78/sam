all:
	gcc -c -Wall -Werror -fpic library.c
	gcc -shared -o libsam.so library.o -lcurl
	gcc -o prog main.c -L. -lsam -Wl,-rpath=.