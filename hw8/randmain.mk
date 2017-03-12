randmain:
	gcc -c randmain.c randcpuid.c
	gcc randmain.o randcpuid.o -o randmain -ldl -Wl,-rpath=$$PWD

randlibsw.so:
	gcc -c -fPIC randlibsw.c
	gcc -shared randlibsw.o -o randlibsw.so

randlibhw.so:
	gcc -c -fPIC -mrdrnd randlibhw.c
	gcc -shared randlibhw.o -o randlibhw.so
