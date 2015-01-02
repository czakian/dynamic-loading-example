
all: dynamic_loader loaded_module


dynamic_loader: src/dynamic_loader.c src/dynamic_loader.h
	clang -ldl -ggdb -std=c99	-Wl,-export-dynamic -O3 -o dynamic_loader.exe src/dynamic_loader.c

loaded_module: src/loaded_module.c
	clang -ggdb -std=c99 -shared -fPIC -O3 -o loaded_module.so src/loaded_module.c

clean:
	rm *.so
	rm *.exe

