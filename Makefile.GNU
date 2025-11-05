CC 		= cc
CXX 	= c++
AR		= ar
RANLIB 	= ranlib
LD		= gcc

.PHONY: all
all: exe_gui exe_cli

.PHONY: clean
clean:
	rm -rf src_lib_static.o liblibrary_static.a
	rm -rf src_lib_shared.o liblibrary_shared.a
	rm -f exe_gui src_gui.o
	rm -f exe_cli src_cli.o

src_cli.o: src_cli.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

exe_cli: src_cli.o liblibrary_static.a liblibrary_shared.so
	$(LD) -Wl,-rpath,$(PWD) -o $@ $^

src_gui.o: src_gui.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

exe_gui: src_gui.o liblibrary_static.a liblibrary_shared.so
	$(LD) -Wl,-rpath,$(PWD) -o $@ $^

src_lib_static.o: src_lib_static.c src_lib_static.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

liblibrary_static.a: src_lib_static.o
	$(AR) qc $@ $^
	$(RANLIB) $@

src_lib_shared.o: src_lib_shared.c src_lib_shared.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -o $@ -c $<

liblibrary_shared.so: src_lib_shared.o
	$(LD) $(CPPFLAGS) $(LDFLAGS) -shared -o liblibrary_shared.so $^
